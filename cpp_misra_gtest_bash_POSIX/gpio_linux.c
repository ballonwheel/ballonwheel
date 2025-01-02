/**
 * @file gpio_linux.c
 * @brief GPIO access via sysfs
 *
 * $Revision: $
 * $Date: $
 *
 * GPIO access implementation, using the sysfs interface.
 *
 * Author: Balint Viragh <bviragh@dension.com>
 * Copyright (C) 2010 Dension Audio Systems Kft.
 */

/**
 * @addtogroup IRF
 * @{   
 */

/*================================[ INCLUDES ]===============================*/
/* System headers */
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/select.h>
#include <poll.h>

/* external library headers */   

/* Interface of other modules */
#define ENABLE_DEBUG
#include "shared.h"

/* Interface header of this module */
#include "gpio_linux.h"

/*================================[ INTERNAL MACROS ]========================*/

/** root directory of the sysfs GPIO interface */
#define L_ST_GPIO_SYSFS_ROOT        "/sys/class/gpio"

/** sysfs export file */
#define L_ST_GPIO_SYSFS_EXPORT      "/sys/class/gpio/export"

/** maximal length of string represented number */
#define L_NUM_LEN                   4u

/** maximal length of GPIO file path in sysfs */
#define L_GPIO_PATH_LEN             63u

/** maximal length of the string can be read from direction file */
#define L_DIR_LEN                   7u

/** maximal length of the string can be read from direction file */
#define L_ACTIVE_LOW_LEN            7u

/** maximal length of the string can be read from direction file */
#define L_EDGE_LEN                  8u

/** maximal length of the string can be read from a value file */
#define L_VALUE_LEN                 7u

/** name of the direction file in a GPIO access directory */
#define L_ST_DIRECTION              "/direction"

/** input configuration string */
#define L_ST_INPUT                  "in\n"

/** output configuration string */
#define L_ST_OUTPUT                 "out\n"

/** output & low initialize configuration */
#define L_ST_OUTPUT_LOW             "low\n"

/** output & high initialize configuration */
#define L_ST_OUTPUT_HIGH            "high\n"

/** name of the value file in a GPIO access directory */
#define L_ST_VALUE                  "/value"

/** string to configure output high */
#define L_ST_HIGH                   "1\n"

/** string to configure output low */
#define L_ST_LOW                    "0\n"

/** name of active_low file in GPIO access directory */
#define L_ST_ACTIVE_LOW             "/active_low"

/** string to configure value invertion */
#define L_ST_INVERT                 "1\n"

/** string to configure normal values (not inverted) */
#define L_ST_NORMAL                 "0\n"

/** name of edge file in GPIO access directory */
#define L_ST_EDGE                   "/edge"

/** string to disable polling */
#define L_ST_DISABLE                "none\n"

/** string to configure rising edge polling */
#define L_ST_RISING                 "rising\n"

/** string to configure falling edge polling */
#define L_ST_FALLING                "falling\n"

/** string to configure both edge polling */
#define L_ST_BOTH                   "both\n"

/** Short button - long button press threashold for pinchange thread */
#define L_I_SHORT_PRESS_ITVAL		20	/* X * 100 ms = 2 sec */

/** Debounce time interval in ms */
#define L_I_DEBOUNCE_TIME			5

/** Debounce count */
#define L_I_DEBOUNCE_COUNT			5

/** Debounce threshold */
#define L_I_DEBOUNCE_THRESHOLD		3

/*================================[ INTERNAL TYPEDEFS ]======================*/

/*================================[ INTERNAL GLOBALS ]=======================*/

/*================================[ EXTERNAL GLOBALS ]=======================*/

/*================================[ INTERNAL FUNCTION PROTOTYPES ]===========*/

/* check & create GPIO access directory */
static int l_gpio_access_f(char* p_cp_path, uint32_t p_u32_path_len, uint32_t p_u32_pin);
/* check if a directory exists or not */
static int l_dir_exist_f(const char* p_cp_path);
/* create a GPIO access directory */
static int l_export_gpio_f(uint32_t p_u32_pin);
/* write string to file */
static int l_write_str_f(const char* p_cp_path, const char* p_cp_data);
/* read a string from a file */
static int l_read_str_f(const char* p_cp_path, char* p_cp_data, uint32_t p_u32_max_len);
/* pinchenge thread function */
static void * l_pinchange_thread(void * arg);

/*================================[ INTERNAL FUNCTION DEFINITIONS ]==========*/

static int l_gpio_access_f(char* p_cp_path, uint32_t p_u32_path_len, uint32_t p_u32_pin)
{
    int return_value;
    int res;
    
    return_value = snprintf(p_cp_path, p_u32_path_len, L_ST_GPIO_SYSFS_ROOT "/gpio%d",p_u32_pin);
    if(return_value < 0)        /* error handling */
        return return_value;

    /* create access directory if it does not exist */
    res = l_dir_exist_f(p_cp_path);
    if(res < 0)
    {
        DBG(DBG_ERROR,"can not check if %s exists (%d)",p_cp_path, res);
        return res;
    }
    if(0 == res)    /* GPIO pin access directory does not exist */
    {
        res = l_export_gpio_f(p_u32_pin);
        if(res < 0)
        {
            DBG(DBG_ERROR,"Can not create GPIO access directory (pin:%d, res:%d)", p_u32_pin, res);
            return res;
        }
    }

    return return_value;
}

static int l_dir_exist_f(const char* p_cp_path)
{
    int return_value;
    struct stat s_stat;
    int res;

    res = stat(p_cp_path, &s_stat);
    if(res) 
    {
        if((ENOTDIR == errno) || (ENOENT == errno))
            return_value = 0;   /* directory does not exist */
        else
            return_value = -1;  /* error */
    }
    else    /* path exists */
    {
        if(S_ISDIR(s_stat.st_mode))
            return_value = 1;   /* directory exists */
        else
            return_value = -1;  /* item exists but it is not a directory */
    }
    return return_value;
}

static int l_export_gpio_f(uint32_t p_u32_pin)
{
    char st_number[L_NUM_LEN + 1u];
    int res;
    
    /* convert pin number to string */
    res = snprintf(st_number, L_NUM_LEN, "%d", p_u32_pin);
    if(res < 0)
        return res;

    res = l_write_str_f(L_ST_GPIO_SYSFS_EXPORT, st_number);
    if(res < 0)
        return res;

    return 0;
}

static int l_write_str_f(const char* p_cp_path, const char* p_cp_data)
{
    int fd;
    int res;
    uint32_t u32_length;

    /* argument check */
    if((NULL == p_cp_path) || (NULL == p_cp_data))
        return -1;

    /* open file */
    fd = open(p_cp_path, O_WRONLY);
    if(fd < 0)
        return fd;

    u32_length = strlen(p_cp_data);
    /* write */
    res = write(fd, p_cp_data, u32_length);

    /* close */
    (void)close(fd);

    return res;
}

static int l_read_str_f(const char* p_cp_path, char* p_cp_data, uint32_t p_u32_max_len)
{
    int fd;
    int res;

    /* argument check */
    if((NULL == p_cp_path) || (NULL == p_cp_data))
        return -1;

    /* open file */
    fd = open(p_cp_path, O_RDONLY);
    if(fd < 0)
        return fd;
    
    res = read(fd, p_cp_data, p_u32_max_len);

    if(res >= 0)
    {
        p_cp_data[res] = '\0';  /* terminate string */
    }

    (void)close(fd);
    return res;
}

static void * l_pinchange_thread(void * arg)
{
    int ret;
    gpio_s_pinchange_t * l_sp_handler = (gpio_s_pinchange_t *)arg;
    int i;
    int k;
    char st_path[L_GPIO_PATH_LEN + 1u];
    uint32_t u32_length;
    int i_nfd = 0;
    char st_str[L_VALUE_LEN + 1u];

	
    for (;;) 
    {
        pthread_mutex_lock(&l_sp_handler->m_lock);
        if (l_sp_handler->i_pins_num > l_sp_handler->i_fds_num)
        {
            for (i = l_sp_handler->i_fds_num; i < l_sp_handler->i_pins_num; i++)
            {
                l_sp_handler->i_fds[i] = -1;
                /* construct GPIO access directory name of the pin */
                ret = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, l_sp_handler->u32_pins[i]);
                if(ret < 0)
                {
                    DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",ret);
                    continue;
                }
                u32_length = ret;
                /* construct name of the value file of the pin */
                (void)strncpy0(st_path + u32_length, L_ST_VALUE, sizeof(st_path) - u32_length);

                ret = open(st_path, O_RDONLY);
                if (ret < 0) 
                {
                    DBG(DBG_ERROR, "Cannot open value file for reading: %s", st_path);
                    continue;
                }
                l_sp_handler->i_fds[i] = ret;
                l_sp_handler->i_handled[i] = 0;
                l_sp_handler->i_count[i] = 0;
                l_sp_handler->i_count_high[i] = 0;
                l_sp_handler->i_old_pin_values[i] = 0;
                l_sp_handler->i_pin_values[i] = 0;
                

                DBG(DBG_DEVEL, "New PIN to scan fd %d with GPIO pin %u (%s)", l_sp_handler->i_fds[i], l_sp_handler->u32_pins[i], st_path);
            }
            l_sp_handler->i_fds_num = l_sp_handler->i_pins_num;
        }
        
        //pthread_mutex_unlock(&l_sp_handler->m_lock);
        
        i_nfd = 0;        
		for (i = 0; i < l_sp_handler->i_fds_num; i++)
		{
			if (l_sp_handler->i_fds[i] >= 0)
			{
				i_nfd++;			
			}
		}        
		
		//pthread_mutex_lock(&l_sp_handler->m_lock);
		for (k = 0; k < L_I_DEBOUNCE_COUNT; k++)
		{
			ms_sleep(L_I_DEBOUNCE_TIME);
			for (i = 0; i < i_nfd; i++)
			{        
				lseek(l_sp_handler->i_fds[i], 0, SEEK_SET);
				ret = read(l_sp_handler->i_fds[i], st_str, sizeof(st_str) - 1);			
				if (ret < 0) 
				{
					DBG(DBG_WARNING, "Unable to read pinchange value");
					continue;
				}
				st_str[ret] = '\0';
				if (strcmp(st_str, L_ST_HIGH) == 0)
				{
					(l_sp_handler->i_count_high[i])++;
				}			
			}
		}
		for (i = 0; i < i_nfd; i++)
		{
			if (l_sp_handler->i_count_high[i] >= L_I_DEBOUNCE_THRESHOLD)
			{
				l_sp_handler->i_pin_values[i] = 1;
			}
			else
			{
				l_sp_handler->i_pin_values[i] = 0;
			}
			l_sp_handler->i_count_high[i] = 0;
		}
		for (i = 0; i < i_nfd; i++)
		{
			if (l_sp_handler->i_old_pin_values[i] == 0) 			//not pressed yet
			{
				if (l_sp_handler->i_pin_values[i] == 1) 			//already pressed the button
				{
					l_sp_handler->i_old_pin_values[i] = l_sp_handler->i_pin_values[i];
					//DBG(DBG_DEVEL, " ||||| PRESSED DOWN");
					//DBG(DBG_DEVEL, "Event on fd %d with GPIO pin %u", l_sp_handler->i_fds[i], l_sp_handler->u32_pins[i]);
				}
				else if (l_sp_handler->i_pin_values[i] == 0) 			//not pressed
				{	
					//do nothing
					//DBG(DBG_DEVEL, " ||||| NOT PRESSED");
				}
			}
			else if (l_sp_handler->i_old_pin_values[i] == 1)			//already pressed
			{
				if (l_sp_handler->i_pin_values[i] == 1) 				//it's been pressing 
				{
					if (l_sp_handler->i_count[i] >= L_I_SHORT_PRESS_ITVAL && l_sp_handler->i_handled[i] != 1)
					{
						//LONG PRESS
						//DBG(DBG_DEVEL, " ||||| LONG PRESS");
						l_sp_handler->callback(l_sp_handler->u32_pins[i], GPIO_PUSH_LONG);
						l_sp_handler->i_handled[i] = 1;
						
					}
					else
					{
						(l_sp_handler->i_count[i])++;
					}
				}
				else if (l_sp_handler->i_pin_values[i] == 0) 			//released 
				{
					//DBG(DBG_DEVEL, " ||||| RELEASED");	
					if (l_sp_handler->i_handled[i] == 0) 				//was not LONG push
					{
						l_sp_handler->i_count[i] = 0;
						//SHORT PRESS
						//DBG(DBG_DEVEL, " ||||| SHORT PRESS");
						l_sp_handler->callback(l_sp_handler->u32_pins[i], GPIO_PUSH_SHORT);
						l_sp_handler->i_old_pin_values[i] = 0;
					}
					else if (l_sp_handler->i_handled[i] == 1)			//was LONG push
					{
						//DBG(DBG_DEVEL, " ||||| RELEASED AFTER LONG");
						//do nothing but clear variables!
						l_sp_handler->i_count[i] = 0;
						l_sp_handler->i_old_pin_values[i] = 0;
						l_sp_handler->i_handled[i] = 0;
					}
				}
			}				

		}

		pthread_mutex_unlock(&l_sp_handler->m_lock);

    }

    return NULL;
}

/*================================[ EXPORTED FUNCTION DEFINITIONS ]==========*/

/* Function to configure GPIO pin direction (see header for more info) */
int gpio_set_dir_f(uint32_t p_u32_pin, gpio_e_dir_t p_e_dir)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    uint32_t u32_length;
    int res;
    char* cp_dir = NULL;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return -1;
    }
    u32_length = res;

    /* construct name of the direction file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_DIRECTION, sizeof(st_path) - u32_length);

    if(GPIO_DIR_INPUT == p_e_dir)
    {
        cp_dir = L_ST_INPUT;
    }
    else if(GPIO_DIR_OUTPUT == p_e_dir)
    {
        cp_dir = L_ST_OUTPUT;
    }
    else if(GPIO_DIR_OUTPUT_HIGH == p_e_dir)
    {
        cp_dir = L_ST_OUTPUT_HIGH;
    }
    else if(GPIO_DIR_OUTPUT_LOW == p_e_dir)
    {
        cp_dir = L_ST_OUTPUT_LOW;
    }
    else
    {
        DBG(DBG_ERROR, "Invalid GPIO direction argument %d",p_e_dir);
        return -1;
    }

    /* write into the direction configuration file */
    res = l_write_str_f(st_path, cp_dir);
    if(res < 0)
    {
        DBG_PERRNO("GPIO write (%s)",st_path);
        return -1;
    }

    return 0;
}

/* Function to get the configured direction of a GPIO pin (see header for more info) */
gpio_e_dir_t gpio_get_dir_f(uint32_t p_u32_pin)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    uint32_t u32_length;
    int res;
    char st_str[L_DIR_LEN + 1u];
    gpio_e_dir_t return_value = GPIO_DIR_ERROR;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return GPIO_DIR_ERROR;
    }
    u32_length = res;

    /* construct name of the direction file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_DIRECTION, sizeof(st_path) - u32_length);

    /* write into the direction configuration file */
    res = l_read_str_f(st_path, st_str, sizeof(st_str));
    if(res < 0)
    {
        DBG_PERRNO("GPIO read (%s)",st_path);
        return GPIO_DIR_ERROR;
    }

    if(0 == strcmp(st_str,L_ST_INPUT))
    {
        return_value = GPIO_DIR_INPUT;
    }
    else if(0 == strcmp(st_str,L_ST_INPUT))
    {
        return_value = GPIO_DIR_OUTPUT;
    }
    else
    {
        DBG(DBG_ERROR,"Invalid content (%s) in direction file (%s)",st_str, st_path);
        return_value = -1;
    }

    return return_value;
}

/*  The function configures the polling edge of a GPIO pin via sysfs interface. */
int gpio_set_polling_f(uint32_t p_u32_pin, gpio_e_edge_t p_e_edge)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    uint32_t u32_length;
    int res;
    char * cp_edge = NULL;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return -1;
    }
    u32_length = res;

    /* construct name of the direction file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_EDGE, sizeof(st_path) - u32_length);

    if(GPIO_POLLING_NONE == p_e_edge)
    {
        cp_edge = L_ST_DISABLE;
    }
    else if(GPIO_POLLING_RISING == p_e_edge)
    {
        cp_edge = L_ST_RISING;
    }
    else if(GPIO_POLLING_FALLING == p_e_edge)
    {
        cp_edge = L_ST_FALLING;
    }
    else if(GPIO_POLLING_BOTH == p_e_edge)
    {
        cp_edge = L_ST_BOTH;
    }
    else
    {
        DBG(DBG_ERROR, "Invalid GPIO polling argument %d",p_e_edge);
        return -1;
    }

    /* write into the edge configuration file */
    res = l_write_str_f(st_path, cp_edge);
    if(res < 0)
    {
        DBG_PERRNO("GPIO write (%s)",st_path);
        return -1;
    }

    return 0;
}

/* The function returns the configured polling edge of a GPIO pin. */
gpio_e_edge_t gpio_get_polling_f(uint32_t p_u32_pin)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    uint32_t u32_length;
    int res;
    char st_str[L_EDGE_LEN + 1u];
    gpio_e_edge_t return_value = GPIO_POLLING_ERROR;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return GPIO_DIR_ERROR;
    }
    u32_length = res;

    /* construct name of the direction file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_EDGE, sizeof(st_path) - u32_length);

    /* read from the edge configuration file */
    res = l_read_str_f(st_path, st_str, sizeof(st_str));
    if(res < 0)
    {
        DBG_PERRNO("GPIO read (%s)",st_path);
        return GPIO_DIR_ERROR;
    }

    if(0 == strcmp(st_str,L_ST_DISABLE))
    {
        return_value = GPIO_POLLING_NONE;
    }
    else if(0 == strcmp(st_str,L_ST_RISING))
    {
        return_value = GPIO_POLLING_RISING;
    }
    else if(0 == strcmp(st_str,L_ST_FALLING))
    {
        return_value = GPIO_POLLING_FALLING;
    }
    else if(0 == strcmp(st_str,L_ST_BOTH))
    {
        return_value = GPIO_POLLING_BOTH;
    }
    else
    {
        DBG(DBG_ERROR,"Invalid content (%s) in edge file (%s)",st_str, st_path);
        return_value = -1;
    }

    return return_value;
}

/* The function configures the value invertion of a GPIO pin via sysfs interface. */
int gpio_set_invert_f(uint32_t p_u32_pin, int p_invert)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    char* cp_value = NULL;
    int res;
    uint32_t u32_length;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return -1;
    }
    u32_length = res;

    /* construct name of the value file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_ACTIVE_LOW, sizeof(st_path) - u32_length);

    if(p_invert)
    {
        cp_value = L_ST_INVERT;
    }
    else
    {
        cp_value = L_ST_NORMAL;
    }

    res = l_write_str_f(st_path,cp_value);
    if(res < 0)
    {
        DBG_PERRNO("GPIO write (%s)",st_path);
        return -1;
    }

    return 0;
}

/* The function returns the configured value invertion of a GPIO pin. */
int gpio_get_invert_f(uint32_t p_u32_pin)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    uint32_t u32_length;
    int res;
    char st_str[L_ACTIVE_LOW_LEN + 1u];
    int return_value = -1;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return -1;
    }
    u32_length = res;

    /* construct name of the direction file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_ACTIVE_LOW, sizeof(st_path) - u32_length);

    /* write into the direction configuration file */
    res = l_read_str_f(st_path, st_str, sizeof(st_str));
    if(res < 0)
    {
        DBG_PERRNO("GPIO read (%s)",st_path);
        return -1;
    }

    if(0 == strcmp(st_str,L_ST_INVERT))
    {
        return_value = 1;
    }
    else if(0 == strcmp(st_str, L_ST_NORMAL))
    {
        return_value = 0;
    }
    else
    {
        DBG(DBG_ERROR,"invalid content (%s) in GPIO access_low file (%s)",st_str, st_path);
        return_value = -1;
    }

    return return_value;
}

/* Function to set the value of an output pin (see header for more info) */
int gpio_set_pin_f(uint32_t p_u32_pin, int p_b_value)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    char* cp_value = NULL;
    int res;
    uint32_t u32_length;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return -1;
    }
    u32_length = res;

    /* construct name of the value file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_VALUE, sizeof(st_path) - u32_length);

    if(p_b_value)
    {
        cp_value = L_ST_HIGH;
    }
    else
    {
        cp_value = L_ST_LOW;
    }

    res = l_write_str_f(st_path,cp_value);
    if(res < 0)
    {
        DBG_PERRNO("GPIO write (%s)",st_path);
        return -1;
    }

    return 0;
}


/* Function to get the value of an input pin (see header for more info) */
int gpio_get_pin_f(uint32_t p_u32_pin)
{
    char st_path[L_GPIO_PATH_LEN + 1u];
    uint32_t u32_length;
    int res;
    char st_str[L_VALUE_LEN + 1u];
    int return_value = -1;

    /* construct GPIO access directory name of the pin */
    res = l_gpio_access_f(st_path, L_GPIO_PATH_LEN, p_u32_pin);
    if(res < 0)
    {
        DBG(DBG_ERROR, "can not construct GPIO pin base path (%d)",res);
        return -1;
    }
    u32_length = res;

    /* construct name of the direction file of the pin */
    (void)strncpy0(st_path + u32_length, L_ST_VALUE, sizeof(st_path) - u32_length);

    /* write into the direction configuration file */
    res = l_read_str_f(st_path, st_str, sizeof(st_str));
    if(res < 0)
    {
        DBG_PERRNO("GPIO read (%s)",st_path);
        return -1;
    }

    if(0 == strcmp(st_str,L_ST_HIGH))
    {
        return_value = 1;
    }
    else if(0 == strcmp(st_str, L_ST_LOW))
    {
        return_value = 0;
    }
    else
    {
        DBG(DBG_ERROR,"invalid content (%s) in GPIO value file (%s)",st_str, st_path);
        return_value = -1;
    }

    return return_value;
}

int gpio_add_pinchange_f(gpio_s_pinchange_t * p_sp_handler, uint32_t p_u32_pin, gpio_pinchange_fp p_cb_pinchange)
{
    int ret;
    pthread_attr_t l_thattr;

    //DBG(DBG_ERROR, "Adding pinchange to %u", p_u32_pin);

    if (p_sp_handler->i_pins_num >= GPIO_PINCHANGE_MAX)
    {
        DBG(DBG_WARNING, "Maximum number of pinchange pins exceeded");
        return -1;
    }

    if (p_sp_handler->i_thread_started == 0) 
    {
        //DBG(DBG_ERROR, "Adding pinchange to %u - initialize mutex", p_u32_pin);
        pthread_mutex_init(&p_sp_handler->m_lock, NULL);
        //DBG(DBG_ERROR, "Adding pinchange to %u - initialize mutex ok", p_u32_pin);
    }
    
    //DBG(DBG_ERROR, "Adding pinchange to %u - adding pin", p_u32_pin);
    pthread_mutex_lock(&p_sp_handler->m_lock);
    p_sp_handler->u32_pins[(p_sp_handler->i_pins_num)++] = p_u32_pin;
    p_sp_handler->callback = p_cb_pinchange;
    pthread_mutex_unlock(&p_sp_handler->m_lock);
    //DBG(DBG_ERROR, "Adding pinchange to %u - adding pin ok", p_u32_pin);

    /* If not started, start the thread */
    if (p_sp_handler->i_thread_started == 0) 
    {
        //DBG(DBG_ERROR, "Adding pinchange to %u - init thread attrs", p_u32_pin);
        pthread_attr_init(&l_thattr);
        pthread_attr_setdetachstate(&l_thattr, PTHREAD_CREATE_DETACHED);
        //DBG(DBG_ERROR, "Adding pinchange to %u - init thread attrs ok", p_u32_pin);
        //DBG(DBG_ERROR, "Adding pinchange to %u - starting thread", p_u32_pin);
        ret = pthread_create(&p_sp_handler->p_thread, &l_thattr, l_pinchange_thread, p_sp_handler);
        if (ret) 
        {
            DBG(DBG_ERROR, "Unable to start pinchange thread");
            return -1;
        }
        //DBG(DBG_ERROR, "Adding pinchange to %u - starting thread ok", p_u32_pin);
        pthread_attr_destroy(&l_thattr);
        p_sp_handler->i_thread_started = 1;
    }
    
    return 0;
}

/**@}*/
