/*
 * HTTP protocol for ffmpeg client
 * Copyright (c) 2000, 2001 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "libavutil/avstring.h"
#include "avformat.h"
#include <unistd.h>
#include <strings.h>
#include "internal.h"
#include "network.h"
#include "http.h"
#include "os_support.h"
#include "httpauth.h"
#include "libavcodec/opt.h"

/* Dension's makro for getting DeviceSerial into http header */
#define DENSION_SERIAL_MAX_ST_LEN 64
#define DENSION_DEVICE_CFG_PATH "mnt/config/device.cfg"

/* XXX: POST protocol is not completely implemented because ffmpeg uses
   only a subset of it. */

#define OFFSET(x) offsetof(HTTPContext, x)
static const AVOption options[] = {
{"chunksize", "use chunked transfer-encoding for posts, -1 disables it, 0 enables it", OFFSET(chunksize), FF_OPT_TYPE_INT64, 0, -1, 0 }, /* Default to 0, for chunked POSTs */
{NULL}
};
static const AVClass httpcontext_class = {
    "HTTP", av_default_item_name, options, LIBAVUTIL_VERSION_INT
};

static int http_connect(URLContext *h, const char *path, const char *hoststr,
                        const char *auth, int *new_location);

void ff_http_set_headers(URLContext *h, const char *headers)
{
    HTTPContext *s = h->priv_data;
    int len = strlen(headers);

    if (len && strcmp("\r\n", headers + len - 2))
        av_log(NULL, AV_LOG_ERROR, "No trailing CRLF found in HTTP header.\n");

    av_strlcpy(s->headers, headers, sizeof(s->headers));
}

void ff_http_set_chunked_transfer_encoding(URLContext *h, int is_chunked)
{
    ((HTTPContext*)h->priv_data)->chunksize = is_chunked ? 0 : -1;
}

void ff_http_init_auth_state(URLContext *dest, const URLContext *src)
{
    memcpy(&((HTTPContext*)dest->priv_data)->auth_state,
           &((HTTPContext*)src->priv_data)->auth_state, sizeof(HTTPAuthState));
}

/* return non zero if error */
static int http_open_cnx(URLContext *h)
{
    const char *path, *proxy_path;
    char hostname[1024], hoststr[1024];
    char auth[1024];
    char path1[1024];
    char buf[1024];
    int port, use_proxy, err, location_changed = 0, redirects = 0;
    HTTPAuthType cur_auth_type;
    HTTPContext *s = h->priv_data;
    URLContext *hd = NULL;

    proxy_path = getenv("http_proxy");
    use_proxy = (proxy_path != NULL) && !getenv("no_proxy") &&
        av_strstart(proxy_path, "http://", NULL);

    /* fill the dest addr */
 redo:
    /* needed in any case to build the host string */
    av_url_split(NULL, 0, auth, sizeof(auth), hostname, sizeof(hostname), &port,
                 path1, sizeof(path1), s->location);
    ff_url_join(hoststr, sizeof(hoststr), NULL, NULL, hostname, port, NULL);

    if (use_proxy) {
        av_url_split(NULL, 0, auth, sizeof(auth), hostname, sizeof(hostname), &port,
                     NULL, 0, proxy_path);
        path = s->location;
    } else {
        if (path1[0] == '\0')
            path = "/";
        else
            path = path1;
    }
    if (port < 0)
        port = 80;

    ff_url_join(buf, sizeof(buf), "tcp", NULL, hostname, port, NULL);
    err = url_open(&hd, buf, URL_RDWR);
    if (err < 0)
        goto fail;

    s->hd = hd;
    cur_auth_type = s->auth_state.auth_type;
    if (http_connect(h, path, hoststr, auth, &location_changed) < 0)
        goto fail;
    if (s->http_code == 401) {
        if (cur_auth_type == HTTP_AUTH_NONE && s->auth_state.auth_type != HTTP_AUTH_NONE) {
            url_close(hd);
            goto redo;
        } else
            goto fail;
    }
    if ((s->http_code == 302 || s->http_code == 303) && location_changed == 1) {
        /* url moved, get next */
        url_close(hd);
        if (redirects++ >= MAX_REDIRECTS)
            return AVERROR(EIO);
        location_changed = 0;
        goto redo;
    }
    return 0;
 fail:
    if (hd)
        url_close(hd);
    s->hd = NULL;
    return AVERROR(EIO);
}

static int http_open(URLContext *h, const char *uri, int flags)
{
    HTTPContext *s = h->priv_data;

    h->is_streamed = 1;

    s->filesize = -1;
    av_strlcpy(s->location, uri, sizeof(s->location));

    return http_open_cnx(h);
}
static int http_getc(HTTPContext *s)
{
    int len;
    if (s->buf_ptr >= s->buf_end) {
        len = url_read(s->hd, s->buffer, BUFFER_SIZE);
        if (len < 0) {
            return AVERROR(EIO);
        } else if (len == 0) {
            return -1;
        } else {
            s->buf_ptr = s->buffer;
            s->buf_end = s->buffer + len;
        }
    }
    return *s->buf_ptr++;
}

static int http_get_line(HTTPContext *s, char *line, int line_size)
{
    int ch;
    char *q;

    q = line;
    for(;;) {
        ch = http_getc(s);
        if (ch < 0)
            return AVERROR(EIO);
        if (ch == '\n') {
            /* process line */
            if (q > line && q[-1] == '\r')
                q--;
            *q = '\0';

            return 0;
        } else {
            if ((q - line) < line_size - 1)
                *q++ = ch;
        }
    }
}

static int process_line(URLContext *h, char *line, int line_count,
                        int *new_location)
{
    HTTPContext *s = h->priv_data;
    char *tag, *p, *end;

    /* end of header */
    if (line[0] == '\0')
        return 0;

    p = line;
    if (line_count == 0) {
        while (!isspace(*p) && *p != '\0')
            p++;
        while (isspace(*p))
            p++;
        s->http_code = strtol(p, &end, 10);

        dprintf(NULL, "http_code=%d\n", s->http_code);

        /* error codes are 4xx and 5xx, but regard 401 as a success, so we
         * don't abort until all headers have been parsed. */
        if (s->http_code >= 400 && s->http_code < 600 && s->http_code != 401) {
            end += strspn(end, SPACE_CHARS);
            av_log(NULL, AV_LOG_WARNING, "HTTP error %d %s\n",
                   s->http_code, end);
            return -1;
        }
    } else {
        while (*p != '\0' && *p != ':')
            p++;
        if (*p != ':')
            return 1;

        *p = '\0';
        tag = line;
        p++;
        while (isspace(*p))
            p++;
        if (!strcmp(tag, "Location")) {
            strcpy(s->location, p);
            *new_location = 1;
        } else if (!strcmp (tag, "Content-Length") && s->filesize == -1) {
            s->filesize = atoll(p);
        } else if (!strcmp (tag, "Content-Range")) {
            /* "bytes $from-$to/$document_size" */
            const char *slash;
            if (!strncmp (p, "bytes ", 6)) {
                p += 6;
                s->off = atoll(p);
                if ((slash = strchr(p, '/')) && strlen(slash) > 0)
                    s->filesize = atoll(slash+1);
            }
            h->is_streamed = 0; /* we _can_ in fact seek */
        } else if (!strcmp (tag, "Transfer-Encoding") && !strncasecmp(p, "chunked", 7)) {
            s->filesize = -1;
            s->chunksize = 0;
        } else if (!strcmp (tag, "icy-metaint")) {
            s->md_int = atoi (p);
        } else if (!strcmp (tag, "WWW-Authenticate")) {
            ff_http_auth_handle_header(&s->auth_state, tag, p);
        } else if (!strcmp (tag, "Authentication-Info")) {
            ff_http_auth_handle_header(&s->auth_state, tag, p);
        } else if (!strcmp (tag, "Connection")) {
            if (!strcmp(p, "close"))
                s->willclose = 1;
        }
    }
    return 1;
}

static inline int has_header(const char *str, const char *header)
{
    /* header + 2 to skip over CRLF prefix. (make sure you have one!) */
    return av_stristart(str, header + 2, NULL) || av_stristr(str, header);
}

static int GetDensionDeviceSerial(char * st_DensionDeviceSerial, uint32_t max_len)
{
	FILE * fp;
	int i;
	char st[max_len + 50]; /* max lenght of a line of config file: serial number + 50 char */
	char *p;
	int ret = 0;

    /* ini for working parameter: st_DensionDeviceSerial */
	memset(st_DensionDeviceSerial, 0, max_len);
	strcpy(st_DensionDeviceSerial, "DeviceSerial/");
	p = st_DensionDeviceSerial + strlen(st_DensionDeviceSerial);


    /* get info file */
    fp = fopen(DENSION_DEVICE_CFG_PATH, "r");
    if(fp == NULL){
        ret = -1;
        goto ret_;
    }

    /* get '10000yyyy' from 'serial = "10000yyyy"' */
    while( fgets(st, sizeof(st), fp) != NULL ){
        if(strstr(st, "serial")){
			i = 0;
			while(st[i]){
				if(st[i++] == '\"'){
					break;
				}
			}
			if(!st[i]){ /* there is no '\"' or after that there is '\0' */
				ret = -1;
				goto ret_;
			}
			while(st[i]){
				if(st[i] == '\"'){ /* can be empty: "" , dont care*/
					goto ret_;
				}
				*p++ = st[i++];
			}
			if(!st[i]){ /* there is no '\"' */
				ret = -1;
				goto ret_;
			}
        }
    }
    ret = -1;
ret_:
    fclose(fp);
    return ret;
}

static int http_connect(URLContext *h, const char *path, const char *hoststr,
                        const char *auth, int *new_location)
{
    HTTPContext *s = h->priv_data;
    int post, err;
    char line[1024];
    char headers[1024] = "";
    char *authstr = NULL;
    int64_t off = s->off;
    int len = 0;
    char st_DensionDeviceSerial[DENSION_SERIAL_MAX_ST_LEN+1];

    /* send http header */
    post = h->flags & URL_WRONLY;
    authstr = ff_http_auth_create_response(&s->auth_state, auth, path,
                                        post ? "POST" : "GET");

    /* set default headers if needed */
    GetDensionDeviceSerial(st_DensionDeviceSerial, sizeof(st_DensionDeviceSerial));
    if (!has_header(s->headers, "\r\nUser-Agent: "))
       len += av_strlcatf(headers + len, sizeof(headers) - len,
                          "User-Agent: %s %s\r\n", LIBAVFORMAT_IDENT, st_DensionDeviceSerial);
    if (!has_header(s->headers, "\r\nAccept: "))
        len += av_strlcpy(headers + len, "Accept: */*\r\n",
                          sizeof(headers) - len);
    if (!has_header(s->headers, "\r\nRange: "))
        len += av_strlcatf(headers + len, sizeof(headers) - len,
                           "Range: bytes=%"PRId64"-\r\n", s->off);
    if (!has_header(s->headers, "\r\nIcy-MetaData:1"))
	len += av_strlcatf(headers + len, sizeof(headers) - len, "Icy-MetaData:1\r\n");
    if (!has_header(s->headers, "\r\nConnection: "))
        len += av_strlcpy(headers + len, "Connection: close\r\n",
                          sizeof(headers)-len);
    if (!has_header(s->headers, "\r\nHost: "))
        len += av_strlcatf(headers + len, sizeof(headers) - len,
                           "Host: %s\r\n", hoststr);

    /* now add in custom headers */
    av_strlcpy(headers+len, s->headers, sizeof(headers)-len);

    snprintf(s->buffer, sizeof(s->buffer),
             "%s %s HTTP/1.1\r\n"
             "%s"
             "%s"
             "%s"
             "\r\n",
             post ? "POST" : "GET",
             path,
             post && s->chunksize >= 0 ? "Transfer-Encoding: chunked\r\n" : "",
             headers,
             authstr ? authstr : "");

    av_freep(&authstr);
    if (url_write(s->hd, s->buffer, strlen(s->buffer)) < 0)
        return AVERROR(EIO);

    /* init input buffer */
    s->buf_ptr = s->buffer;
    s->buf_end = s->buffer;
    s->line_count = 0;
    s->off = 0;
    s->filesize = -1;
    s->willclose = 0;
    s->md_icy[0] = '\0';
    s->md_int = 0;
    s->md_byte_ctr = 0;
    if (post) {
        /* Pretend that it did work. We didn't read any header yet, since
         * we've still to send the POST data, but the code calling this
         * function will check http_code after we return. */
        s->http_code = 200;
        return 0;
    }
    s->chunksize = -1;

    /* wait for header */
    for(;;) {
        if (http_get_line(s, line, sizeof(line)) < 0)
            return AVERROR(EIO);

        dprintf(NULL, "header='%s'\n", line);

        err = process_line(h, line, s->line_count, new_location);
        if (err < 0)
            return err;
        if (err == 0)
            break;
        s->line_count++;
    }

    return (off == s->off) ? 0 : -1;
}


static int http_read(URLContext *h, uint8_t *buf, int size)
{
    HTTPContext *s = h->priv_data;
    int len;

    if (s->chunksize >= 0) {
        if (!s->chunksize) {
            char line[32];

            for(;;) {
                do {
                    if (http_get_line(s, line, sizeof(line)) < 0)
                        return AVERROR(EIO);
                } while (!*line);    /* skip CR LF from last chunk */

                s->chunksize = strtoll(line, NULL, 16);

                dprintf(NULL, "Chunked encoding data size: %"PRId64"'\n", s->chunksize);

                if (!s->chunksize)
                    return 0;
                break;
            }
        }
        size = FFMIN(size, s->chunksize);
    }

    if (s->md_int) {
	/* read metadata */
        if (s->md_byte_ctr == s->md_int) {
            uint16_t a;
            uint8_t md_len, rem;
            uint8_t * md_buff, * curr;

            md_buff = av_mallocz(255 * 16);
            if (md_buff == NULL)
                return 0;
            curr = md_buff;

            /* get metadata section length */
            if (url_read(s->hd, &md_len, 1) != 1) {
                av_free (md_buff);
                return 0;
            }
            /* read metadata body */
            for (a = 0; a < md_len; ++a) {
                rem = 16;
                while (rem) {
                    len = url_read(s->hd, curr, rem);
                    if (len < 0) {
                        av_free(md_buff);
                        return 0;
                    }
                    curr += len;
                    rem -= len;
                }
            }
            s->md_byte_ctr = 0;
            av_strlcpy(s->md_icy, md_buff, sizeof(s->md_icy));
            av_free(md_buff);
        }

        /* calculate maximal read length */
        if (s->md_byte_ctr + size > s->md_int)
            size = s->md_int - s->md_byte_ctr;
    }

    /* read bytes from input buffer first */
    len = s->buf_end - s->buf_ptr;
    if (len > 0) {
        if (len > size)
            len = size;
        memcpy(buf, s->buf_ptr, len);
        s->buf_ptr += len;
    } else {
        if (!s->willclose && s->filesize >= 0 && s->off >= s->filesize)
            return AVERROR_EOF;
        len = url_read(s->hd, buf, size);
    }
    if (len > 0) {
        s->off += len;
        s->md_byte_ctr += len;
        if (s->chunksize > 0)
            s->chunksize -= len;
    }
    return len;
}

/* used only when posting data */
static int http_write(URLContext *h, const uint8_t *buf, int size)
{
    char temp[11] = "";  /* 32-bit hex + CRLF + nul */
    int ret;
    char crlf[] = "\r\n";
    HTTPContext *s = h->priv_data;

    if (s->chunksize == -1) {
        /* non-chunked data is sent without any special encoding */
        return url_write(s->hd, buf, size);
    }

    /* silently ignore zero-size data since chunk encoding that would
     * signal EOF */
    if (size > 0) {
        /* upload data using chunked encoding */
        snprintf(temp, sizeof(temp), "%x\r\n", size);

        if ((ret = url_write(s->hd, temp, strlen(temp))) < 0 ||
            (ret = url_write(s->hd, buf, size)) < 0 ||
            (ret = url_write(s->hd, crlf, sizeof(crlf) - 1)) < 0)
            return ret;
    }
    return size;
}

static int http_close(URLContext *h)
{
    int ret = 0;
    char footer[] = "0\r\n\r\n";
    HTTPContext *s = h->priv_data;

    /* signal end of chunked encoding if used */
    if ((h->flags & URL_WRONLY) && s->chunksize != -1) {
        ret = url_write(s->hd, footer, sizeof(footer) - 1);
        ret = ret > 0 ? 0 : ret;
    }

    if (s->hd)
        url_close(s->hd);
    return ret;
}

static int64_t http_seek(URLContext *h, int64_t off, int whence)
{
    HTTPContext *s = h->priv_data;
    URLContext *old_hd = s->hd;
    int64_t old_off = s->off;
    uint8_t old_buf[BUFFER_SIZE];
    int old_buf_size;

    if (whence == AVSEEK_SIZE)
        return s->filesize;
    else if ((s->filesize == -1 && whence == SEEK_END) || h->is_streamed)
        return -1;

    /* we save the old context in case the seek fails */
    old_buf_size = s->buf_end - s->buf_ptr;
    memcpy(old_buf, s->buf_ptr, old_buf_size);
    s->hd = NULL;
    if (whence == SEEK_CUR)
        off += s->off;
    else if (whence == SEEK_END)
        off += s->filesize;
    s->off = off;

    /* if it fails, continue on old connection */
    if (http_open_cnx(h) < 0) {
        memcpy(s->buffer, old_buf, old_buf_size);
        s->buf_ptr = s->buffer;
        s->buf_end = s->buffer + old_buf_size;
        s->hd = old_hd;
        s->off = old_off;
        return -1;
    }
    url_close(old_hd);
    return off;
}

static int
http_get_file_handle(URLContext *h)
{
    HTTPContext *s = h->priv_data;
    return url_get_file_handle(s->hd);
}

URLProtocol http_protocol = {
    "http",
    http_open,
    http_read,
    http_write,
    http_seek,
    http_close,
    .url_get_file_handle = http_get_file_handle,
    .priv_data_size = sizeof(HTTPContext),
    .priv_data_class = &httpcontext_class,
};
