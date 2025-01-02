/******************************************************************************
 * file: BC_hw_sd.c
 * description: sd handler
 ******************************************************************************/
/******************************************************************************
 * headers
 ******************************************************************************/
#include "hw.h"


/******************************************************************************
 * definitions
 ******************************************************************************/
//#define	SD_ACMD13_REQ
#define LOCK_BLOCK_RESIZE
#define RESET_WITH_PDC
//#define	SPI_MODE_TEST
#define DECODE_CSD
#define DECODE_CID

#define SD_TIMEOUT	(0.6 * HZ)
#if 0
FF0079E50000 : 00F4691E01F4691E02F4691E03F4691E
FF0079E60000 : 00346A1E01346A1E02346A1E03346A1E
FF0079E70000 : 00746A1E01746A1E02746A1E03746A1E
FF00 stopped
#endif


/******************************************************************************
 * declarations
 ******************************************************************************/
static const char id[] = "$Id: BC_hw_sd.c 894 2012-02-29 09:18:29Z dobany_imre $";




static uint8_t spibuff[SECTOR_SIZE + 16];	                // log sector + 16 addtl bytes
const uint8_t bigff[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

	0xff, 0xff,

	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const uint8_t sd_go_idle_state[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_send_op_cond[] = {
	0x40+1, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_send_status[] = {
	0x40+13, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

const uint8_t sd_cmd55[] = {
	0x40+55, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff
/*	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,*/
};
const uint8_t sd_send_estatus[] = {
	0x40+55, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff,
	0x40+13, 0x00, 0x00, 0x00, 0x00, 0x95,
};

// uint8_t params written
uint8_t sd_rd_sector[] = {
	0x40+17, 0x00, 0x00, 0x00, 0x00, 0x95,
};

// uint8_t params written
uint8_t sd_wr_sector[] = {
	0x40+24, 0x00, 0x00, 0x00, 0x00, 0x95,
};


//doc: sd.Simplified_Physical_Layer_Spec.pdf
//---------
//SEND_IF_COND (CMD8) is used to verify SD Memory Card interface operating condition.
const uint8_t sd_cmd8[] = {//page 51.
	0x40+8, 0x00, 0x00, 0x01, 0xaa, 0x87,  // 2.7-3.6V supplied
	//0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	//0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_cmd9[] = {
	0x40+9, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_cmd10[] = {
	0x40+10, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_cmd58[] = {
	0x40+58, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_acmd[] = {
	0x40+55, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff
};

// uint8_t 10 is written
uint8_t sd_acmd41[] = {
	0x40+55, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff,
	//0x40+41, 0x00, 0x00, 0x00, 0x00, 0x95,//host not sdhc
	0x40+41, 0x40, 0x00, 0x00, 0x00, 0x95,//host sdhc
};

// uint8_t 4 is written
uint8_t sd_cmd16[] = {
	0x40+16, 0x00, 0x00, 0x00, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_cmd16_512[] = {
	0x40+16, 0x00, 0x00, 0x02, 0x00, 0x95,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const uint8_t sd_cmd42[] = {
	0x40+42, 0x00, 0x00, 0x00, 0x00, 0x95,
};

/*
 *      crc7.c
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2. See the file COPYING for more details.
 */

#if 0
#include <linux/types.h>
#include <linux/module.h>
#include <linux/crc7.h>
#endif

/* Table for CRC-7 (polynomial x^7 + x^3 + 1) */
const uint8_t crc7_syndrome_table[256] = {
        0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
        0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
        0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
        0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
        0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
        0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
        0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
        0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
        0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
        0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
        0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
        0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
        0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
        0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
        0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
        0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
        0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
        0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
        0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
        0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
        0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
        0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
        0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
        0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
        0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
        0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
        0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
        0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
        0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
        0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
        0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
        0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};
#if 0
EXPORT_SYMBOL(crc7_syndrome_table);
#endif

/**
 * crc7 - update the CRC7 for the data buffer
 * @crc:     previous CRC7 value
 * @buffer:  data pointer
 * @len:     number of bytes in the buffer
 * Context: any
 *
 * Returns the updated CRC7 value.
 */
uint8_t	crc;

/******************************************************************************
 * prototypes
 ******************************************************************************/
#if 0
uint8_t crc7_byte(uint8_t crc, uint8_t data)
{
	return crc7_syndrome_table[(crc << 1) ^ data];
}
#endif

uint8_t crc7(const uint8_t *buffer, size_t len)
{
	while(len--)
//		crc = crc7_byte(crc, *buffer++);
		crc = crc7_syndrome_table[(crc << 1) ^ *buffer++];
	return (crc << 1) + 1;
}

uint8_t *find_spi_answer(uint8_t starter)
{
uint8_t	*p = spibuff;
int	i;

	for(i = 0; i != 16; i++, p++)
		if(*p == starter)
			return p + 1;
	return spibuff;
}

/*******************************************************************************************************
 * spi basic FUNCTION for SD
 *******************************************************************************************************/

#if defined(DEBUG_SD)
int spi_putblk(const char *cmdname, const uint8_t *cmdblk, int cmdlen)
#else
int spi_putblk(const uint8_t *cmdblk, int cmdlen)
#endif
{
#if defined(DEBUG_SD)
int	j;
#endif
int	i;
uint8_t	*dp = spibuff;
uint8_t	*sp = (uint8_t *)cmdblk;

	#if defined(DEBUG_SD)
	debug.puttxt(cmdname);
	#endif
	#if defined(SD_PDC)
	AT91C_BASE_SPI->SPI_TPR = (unsigned int) cmdblk;
	AT91C_BASE_SPI->SPI_TCR = cmdlen;
	AT91C_BASE_SPI->SPI_RPR = (unsigned int) spibuff;
	AT91C_BASE_SPI->SPI_RCR = cmdlen;
	AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX))
			;
	#else
	for(i = 0; i != cmdlen; i++)
		*dp++ = spi_putbyte(*sp++);
	#endif
	#if defined(DEBUG_SD)
	for(j = 0; j != cmdlen; j++)
		debug.putbyte(spibuff[j]);
	debug.putch('\r');
	debug.putch('\n');
	#endif

	return 1;
}



/*******************************************************************************************************
 * SD basic FUNCTIONS
 *******************************************************************************************************/
void getsdstatus()
{
//unsigned j;
#if defined(SD_ACMD13_REQ)
int	r;
#endif

	SPI_PUTBLK("CMD13", sd_send_status, sizeof(sd_send_status));

	#if defined(SD_ACMD13_REQ)
	SPI_PUTBLK("ACMD13", sd_send_estatus, sizeof(sd_send_estatus));

	r = 0xff;
	do{
		r = spi_getbyte();
	}while(r != 0xfe TO?);

	debug.puttxt("est:");
	for(j = 0; j != 64; j++)
		spi_putbyte(0xff);

	spi_putbyte(0xff);	// CRC
	spi_putbyte(0xff);
	debug.putch('\r');
	debug.putch('\n');
	#endif
}

#if defined(SPI_MODE_TEST)
int	spi_mode;
#endif

uint8_t	*bitblock;

#if defined(DECODE_CSD) || defined(DECODE_CID)
typedef struct cs_s {
	char	*descr;
	int		bitlen;
	int		bitstart;
	unsigned *varptr;
} cs_t;

void show_csinfo(cs_t *p)
{
int	bytepos;
int	bitlen;
unsigned	val;
unsigned	bitmask;

	val = 0;
	bitlen = p->bitlen;
	bitmask = 1 << (p->bitstart & 7);
	for(bytepos = (127 - p->bitstart) / 8; bitlen; bitlen--){
		val <<= 1;
		val |= (bitblock[bytepos] & bitmask) ? 1 : 0;
#if 0
		debug.putw(bytepos);
		debug.putch(' ');
		debug.putw(bitmask);
		debug.putch(' ');
		debug.putw(val);
		debug.putch(' ');
		debug.putw(bitlen);
		debug.puttxt("     ");
#endif
		bitmask >>= 1;
		if(! bitmask){
			bytepos++;
			bitmask = 0x80;
		}
	}
	debug.puttxt(p->descr);
	debug.putw(val);
	if(p->varptr != NULL)
		*p->varptr = val;
	debug.crlf();
}
#endif

const int	c_size_tab[] = {
	1, 2, 4, 8, 16, 32, 64, 128,
	256, 512, 1024, 2048
};

unsigned read_bl_len;
unsigned c_size;
unsigned c_size_mult;
int	sdhc;
//sd.Simplified_Physical_Layer_Spec.pdf : 5.3.2 CSD Register (CSD Version 1.0)
#if defined(DECODE_CSD)
const cs_t csd_1_table[] = {
	{ "TAAC               ", 8, 119, NULL },
	{ "NSAC               ", 8, 111, NULL },
	{ "TRAN_SPEED         ", 8, 103, NULL },
	{ "CCC                ", 12, 95, NULL },
	{ "READ_BL_LEN        ", 4, 83, &read_bl_len },
	{ "READ_BL_PARTIAL    ", 1, 79, NULL },
	{ "WRITE_BLK_MISALIGN ", 1, 78, NULL },
	{ "READ_BLK_MISALIGN  ", 1, 77, NULL },
	{ "DSR_IMP            ", 1, 76, NULL },
	{ "C_SIZE             ", 12, 73, &c_size },
	{ "VDD_R_CURR_MIN     ", 3, 61, NULL },
	{ "VDD_R_CURR_MAX     ", 3, 58, NULL },
	{ "VDD_W_CURR_MIN     ", 3, 55, NULL },
	{ "VDD_W_CURR_MAX     ", 3, 52, NULL },
	{ "C_SIZE_MULT        ", 3, 49, &c_size_mult },
	{ "ERASE_BLK_EN       ", 1, 46, NULL },
	{ "SECTOR_SIZE        ", 7, 45, NULL },
	{ "WP_GRP_SIZE        ", 7, 38, NULL },
	{ "WP_GRP_ENABLE      ", 1, 31, NULL },
	{ "R2W_FACTOR         ", 3, 28, NULL },
	{ "WRITE_BL_LEN       ", 4, 25, NULL },
	{ "WRITE_BL_PARTIAL   ", 1, 21, NULL },
	{ "FILE_FORMAT_GRP    ", 1, 15, NULL },
	{ "COPY               ", 1, 14, NULL },
	{ "PERM_WRITE_PROTECT ", 1, 13, NULL },
	{ "TMP_WRITE_PROTECT  ", 1, 12, NULL },
	{ "FILE_FORMAT        ", 2, 11, NULL },
	{ NULL, 0, 0 }
};

const cs_t csd_2_table[] = {
	{ "TAAC               ", 8, 119, NULL },
	{ "NSAC               ", 8, 111, NULL },
	{ "TRAN_SPEED         ", 8, 103, NULL },
	{ "CCC                ", 12, 95, NULL },
	{ "READ_BL_LEN        ", 4, 83, &read_bl_len },
	{ "READ_BL_PARTIAL    ", 1, 79, NULL },
	{ "WRITE_BLK_MISALIGN ", 1, 78, NULL },
	{ "READ_BLK_MISALIGN  ", 1, 77, NULL },
	{ "DSR_IMP            ", 1, 76, NULL },
	{ "C_SIZE             ", 22, 69, &c_size },
	{ "ERASE_BLK_EN       ", 1, 46, NULL },
	{ "SECTOR_SIZE        ", 7, 45, NULL },
	{ "WP_GRP_SIZE        ", 7, 38, NULL },
	{ "WP_GRP_ENABLE      ", 1, 31, NULL },
	{ "R2W_FACTOR         ", 3, 28, NULL },
	{ "WRITE_BL_LEN       ", 4, 25, NULL },
	{ "WRITE_BL_PARTIAL   ", 1, 21, NULL },
	{ "FILE_FORMAT_GRP    ", 1, 15, NULL },
	{ "COPY               ", 1, 14, NULL },
	{ "PERM_WRITE_PROTECT ", 1, 13, NULL },
	{ "TMP_WRITE_PROTECT  ", 1, 12, NULL },
	{ "FILE_FORMAT        ", 2, 11, NULL },
	{ NULL, 0, 0 }
};

void decode_csd()
{
cs_t	*tabp;

	bitblock = find_spi_answer(0xfe);

	switch(bitblock[0] & 0xc0){
	case 0x00 :
		tabp = (cs_t*)csd_1_table;
		sdhc = 0;
		break;
	case 0x40 :
		tabp = (cs_t*)csd_2_table;
		sdhc = 1;
		break;
	default :
		tabp = NULL;
		break;
	}
	if(tabp != NULL){
		while(tabp->descr != NULL){
			show_csinfo(tabp);
			tabp++;
		}
	}
}
#endif

#if defined(DECODE_CID)
cs_t cid_table_bin[] = {
	{ "MID ", 8, 127 },
	{ "PRV ", 8, 63 },
	{ "PSN ", 32, 55 },
	{ "MDTy", 8, 19 },
	{ "MDTm", 4, 11 },
	{ NULL, 0, 0 }
};
cs_t cid_table_ascii[] = {
	{ "OID ", 16, 119 },
	{ "PNM ", 40, 103 },
	{ NULL, 0, 0 }
};

void show_strinfo(cs_t *p)
{
int	bytepos;
int	bitlen;
unsigned	val;
unsigned	bitmask;

	debug.puttxt(p->descr);
	val = 0;
	bitlen = p->bitlen;
	bitmask = 1 << (p->bitstart & 7);
	for(bytepos = (127 - p->bitstart) / 8; bitlen; bitlen--){
		val <<= 1;
		val |= (bitblock[bytepos] & bitmask) ? 1 : 0;
		bitmask >>= 1;
		if(! bitmask){
			bytepos++;
			bitmask = 0x80;
			debug.putch(val);
			val = 0;	/// eeee pfuj
		}
	}
	debug.crlf();
}

void decode_cid()
{
cs_t	*tabp;

	bitblock = find_spi_answer(0xfe);
	tabp = cid_table_bin;
	while(tabp->descr != NULL){
		show_csinfo(tabp);
		tabp++;
	}
	bitblock = find_spi_answer(0xfe);
	tabp = cid_table_ascii;
	while(tabp->descr != NULL){
		show_strinfo(tabp);
		tabp++;
	}
}
#endif

/**************************************************************************
 * ej_operation_err_t SD_init(uint32_t * disksize)
 *
 * parameter: pointer of int where init will load the size of disk 
 *************************************************************************/
ej_operation_err_t SD_init(uint32_t * disksize)
{
int	r;
int	j;
int	i;
unsigned int starttime = systick_not_allowed_to_set;
uint8_t b;
int disksize_gb;

	*disksize = 0;

	debug.puttxt("SD_init() start");

	#if defined(SPI_MODE_TEST)
	spi_mode = 2;
newspimode:
	debug.puttxt("spi_mode: ");
	debug.putw(spi_mode);
	#endif


	/**************************************************************************
	 * 
	 *************************************************************************/
sdinit:
	spi_ini(SPI_INI_RESET | SPI_INI_SD_SLOW);

	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								SPI_NPCSX_SD |
								SPI_CS_DELAY;

	/**************************************************************************
	 * 
	 *************************************************************************/
	debug.puttxt("wait for 0xff on spi bus");
	WATCHDOG_RESET;

	do{
		if(systick_not_allowed_to_set - starttime > 15 * HZ){ //sequence check too! from 'goto sdinit'
			debug.puttxt("SD init timeout\r\n");
			//goto letsgo;
			return E_EJ_SD_RESETFAIL;
		}	
		b = spi_getbyte();
		debug.puthex("b:", b);
		
	//}while(0xff != spi_getbyte());
	}while(0xff != b);


	/**************************************************************************
	 * 
	 *************************************************************************/
	#if defined(RESET_WITH_PDC)
	SPI_PUTBLK("ACMD0", sd_go_idle_state, sizeof(sd_go_idle_state));
	for(j = 0; j != sizeof(sd_go_idle_state) - 2; j++){
	 #if 0
	//wrong bits
		if(spibuff[j] == 0xe0 && spibuff[j + 1] == 0x3f){
			r = 1;
			#if defined(DEBUG_SD)
			debug.puttxt("e03f");
			#endif
			break;
		}
		if(spibuff[j] == 0xc0 && spibuff[j + 1] == 0x7f){
			r = 1;
			#if defined(DEBUG_SD)
			debug.puttxt("c07f");
			#endif
			break;
		}
	 #endif
		if((r = spibuff[j]) != 0xff){
			debug.puttxt("case 2");
			break;
		}
	}
	
	#else
	
	for(j = 0; j != sizeof(sd_go_idle_state); j++){
		r = spi_putbyte(sd_go_idle_state[j]);
		debug.putbyte(r);
		if(r == 1)
			break;

	}
	#endif

	if(r != 1){
		debug.puttxt("case 3");
		goto sdinit;
	}

	/**************************************************************************
	 * 
	 *************************************************************************/
	#if defined(SD_CMD1_RESET)
waitok:
	for(j = 0; j != sizeof(sd_send_op_cond); j++){
		r = spi_putbyte(sd_send_op_cond[j]);
		if(j > 5 && r != 0xff)
			break;

	}
	if(r == 1)	                        // not ready yet
		goto waitok;
	debug.puttxt("waitok: ");
	debug.putbyte(r);
	if(r != 0)
		goto sdinit;
	#endif

	/**************************************************************************
	 * 
	 *************************************************************************/
//letsgo:
	i = 0;
	getsdstatus();

	debug.puttxt("send_if_cond: ");
	#if 0
	debug.puthex("crc7:", crc7(sd_cmd8, 5));
	#endif

	/*crc = 0;
	sd_cmd8[5] = crc7(sd_cmd8, 5);*/
	SPI_PUTBLK("CMD8", sd_cmd8, sizeof(sd_cmd8));
	r = AT91C_BASE_SPI->SPI_RDR & 0xff;
	do{
		r = spi_putbyte(0xff);
		#if defined(DEBUG_SD)
		if(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_OVRES)
			debug.putch('o1');
		#endif
	}while(r == 0xff);
	if(r & 0x04){//response: pdf 7.3.2.6 Format R7 -> illegal command
		sd_acmd41[10] = 0;//host not supp. sdhc
		debug.puttxt("CMD8 is illegal; mode of host no supp. sdhc");
	}
	else{
		sd_acmd41[10] = 0x40;//host supp. sdhc
		debug.puttxt("mode of host supp. sdhc");
	}
	sd_acmd41[14] = crc7(&sd_acmd41[9], 5);
	debug.puthex("sd_acmd41 crc7:", sd_acmd41[14]);
	
	//remaindeer answer
	do{
		r = spi_putbyte(0xff);
		debug.puthex("r: ", r);
	}while(r != 0xff);

	/**************************************************************************
	 * 
	 *************************************************************************/
	debug.puttxt("read_ocr: ");
	//do{
		SPI_PUTBLK("CMD58", sd_cmd58, sizeof(sd_cmd58));
	//} while(! (spibuff[9] & 0x80));

	/**************************************************************************
	 * 
	 *************************************************************************/
	debug.puttxt("send_op_cond: ");

	for(i = 0; i != 512; i++){
		SPI_PUTBLK("ACMD41", sd_acmd41, sizeof(sd_acmd41));
		r = AT91C_BASE_SPI->SPI_RDR & 0xff;
		do{
			r = spi_putbyte(0xff);
			#if defined(DEBUG_SD)
			if(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_OVRES)
				debug.putch('o2');
			#endif
		}while(r == 0xff);
		spi_putbyte(0xff);

		if(r == 0)
			break;
		if(r == 1){	                    // still "idle"
			debug.puthex("idle:(i) ", i);//found:i: 000000EC
		}
		WATCHDOG_RESET;
	}
	debug.puthex("i: ", i);
	if(i == 512){
		if(sd_acmd41[10] == 0x40){
			#if defined(SPI_MODE_TEST)
			debug.puttxt("SDHC start failed, reset to SD mode\r\n");
			sd_acmd41[10] = 0;
			if(spi_mode < 8){
				spi_mode++;
				goto sdinit;
			}
			#else
			debug.puttxt("SDHC start failed\r\n");
			#endif
			return E_EJ_SD_RESETFAIL;
			//while(1)
			//	;
		}
		else{
			debug.puttxt("SD start failed, trying SDHC mode\r\n");
			sd_acmd41[10] = 0x40;
			goto sdinit;
		}
	}

	/**************************************************************************
	 * 
	 *************************************************************************/
	debug.puttxt("read_ocr: ");
	//do{
		SPI_PUTBLK("CMD58", sd_cmd58, sizeof(sd_cmd58));
	//} while(! (spibuff[9] & 0x80));

	/**************************************************************************
	 * 
	 *************************************************************************/
	debug.puttxt("send_csd: ");
	SPI_PUTBLK("CMD9", sd_cmd9, sizeof(sd_cmd9));
	for(j = 0; j != sizeof(sd_cmd9); j++){
		if(spibuff[j] == 0xfe){
			j++;
			switch(spibuff[j] & 0xc0){
			case 0x00 :	                // CSD 1.0
				debug.puttxt("CSD 1.0\r\n");
				break;
			case 0x40 :	                // CSD 2.0
				debug.puttxt("CSD 2.0\r\n");
				break;
			case 0x80 :
				break;
			case 0xc0 :
				break;
			}
			break;	                    // (for)
		}
	}
	if(j == sizeof(sd_cmd9)){
		debug.puttxt("????");
	}
	decode_csd();

	/**************************************************************************
	 * 
	 *************************************************************************/
	debug.puttxt("send_cid: ");
	SPI_PUTBLK("CMD10", sd_cmd10, sizeof(sd_cmd10));

	decode_cid();


	/**************************************************************************
	 * 
	 *************************************************************************/
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
	spi_ini(SPI_INI_CS_DISABLE);
	npcs_to_tick1 = 0;


	#if defined(SPI_MODE_TEST)
	if(spi_mode < 8){
		spi_mode++;
		goto newspimode;
	}
	#endif

	/**************************************************************************
	 * 
	 *************************************************************************/
	disksize_gb = 0;
	if(! sdhc){
		debug.puttxt("card size info: ");
		debug.puttxt(" read_bl_len: ");
		debug.putw(read_bl_len);
		debug.puttxt(" c_size: ");
		debug.putw(c_size);
		debug.puttxt(" c_size_mult: ");
		debug.putw(c_size_mult);
		debug.crlf();
		debug.puttxt("card sectors: ");
		debug.putw((c_size + 1) * c_size_tab[c_size_mult + 2] * c_size_tab[read_bl_len]);
		if((c_size + 1) * c_size_tab[c_size_mult + 2] * c_size_tab[read_bl_len] < 0x40000000L){
			disksize_gb = SD_1G;
		}
		else{
			disksize_gb = SD_2G;
		}
	}
	else{
		debug.puttxt("card size info: ");
		debug.puttxt(" read_bl_len: ");
		debug.putw(read_bl_len);
		debug.puttxt(" c_size: ");
		debug.putw(c_size);
		debug.puttxt("card sectors: ");
		debug.putw((c_size + 1) * 512 * 1024);
		if((c_size + 1) * 512LL * 1024LL <= 0x100000000LL){
			disksize_gb = SD_4G;
		}
		else if((c_size + 1) * 512LL * 1024LL <= 0x200000000LL){
			disksize_gb = SD_8G;	        
		}
		else if((c_size + 1) * 512LL * 1024LL <= 0x400000000LL){
			disksize_gb = SD_16G;	        
		}
		else if ((c_size + 1) * 512LL * 1024LL <= 0x800000000LL){
			disksize_gb = SD_32G;	        
		}
		else{
			disksize_gb = SD_INVALID;	        
		}

	}
	
	
	debug.puthex("card: ", disksize_gb);
	switch(disksize_gb){//disksize from SD_init()
	case SD_2G :
		*disksize = SD_2G_SECTORS / 2 ;			//SD kartya meret   kbyte
		break;
	case SD_4G :
		*disksize = SD_4G_SECTORS / 2;			//SD kartya meret   kbyte
		break;
	case SD_8G :
		*disksize = SD_8G_SECTORS / 2;			//SD kartya meret   kbyte
		break;
	case SD_16G :
		*disksize = SD_16G_SECTORS / 2;			//SD kartya meret   kbyte
		break;
	case SD_32G :
		*disksize = SD_32G_SECTORS / 2;			//SD kartya meret   kbyte
		break;
	case SD_1G :
		debug.puttxt("1G SD card is not supported!\r\n");
		//break;
	default :
		debug.puttxt("Invalid SD card!\r\n");
		*disksize = 0;
		return E_EJ_SD_INVALIDCARD;
		//break;
	}
	
	
	

	return E_EJ_OK;
}

/**************************************************************************
 * function: SD_rd()
 * desc: read a sector (512 byte) from SD
 *       rd time: ~1ms
 *************************************************************************/
ej_operation_err_t SD_rd(void *d, unsigned long offset)	    // watch out for the SDHC mode!
{
int	r;
int	i;
uint8_t	*dp = d;

	#if 0
	debug.puttxt("rd:"); debug.putw(d);
	debug.puttxt(" ro:"); debug.putw(offset);
	#endif

	LED_TOGGLE(LED_ACT_HW);
	//100 usec wait for sure that prev _cs was finished
	//measured: min 500nsec needed
	while(!npcs_to_tick1)//1 * 100usec
		;	



	/**************************************************************************
	 * 
	 *************************************************************************/
	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								SPI_NPCSX_SD |
								SPI_CS_DELAY;
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;
								

	/**************************************************************************
	 * 
	 *************************************************************************/
	if(! sdhc)
		offset <<= 9;

	sd_rd_sector[1] = (uint8_t) ((offset >> 24) & 0xff);
	sd_rd_sector[2] = (uint8_t) ((offset >> 16) & 0xff);
	sd_rd_sector[3] = (uint8_t) ((offset >> 8) & 0xff);
	sd_rd_sector[4] = (uint8_t) (offset & 0xff);

	
	/**************************************************************************
	 * 
	 *************************************************************************/
	sd_to_tick = 0;
	do{
		r = spi_getbyte();
		if(sd_to_tick > SD_TIMEOUT)
			return E_EJ_SD_RDFAIL;
	}while(r != 0xff);

	
	/**************************************************************************
	 * 
	 *************************************************************************/
	//debug.memdump("rdsect: ", sd_rd_sector, 6);
	SPI_PUTBLK("CMD17", sd_rd_sector, sizeof(sd_rd_sector));

	sd_to_tick = 0;
	do{
		#if defined(DEBUG_SD)
		debug.putch('.');
		#endif
		r = spi_getbyte();
		if(sd_to_tick > SD_TIMEOUT)
			return E_EJ_SD_RDFAIL;
	} while(r == 0xff);
	

	if(r != 0x00/* && r != 0xfe*/){
		debug.putbyte(sd_rd_sector[1]);
		debug.putbyte(sd_rd_sector[2]);
		debug.putbyte(sd_rd_sector[3]);
		debug.putbyte(sd_rd_sector[4]);
		debug.puttxt(" : ");
		debug.putbyte(r);
  		debug.puttxt(" F ");
		getsdstatus();
		//return E_EJ_SD_RDFAIL;
		debug_puttxt("SD STOP 1!\r\n");	
		while(1)		//wait for wd //1201262v4.0.7m883
			;			
	}

	/**************************************************************************
	 * 
	 *************************************************************************/
	r = 0xff;
	sd_to_tick = 0;
	do{
		r = spi_getbyte();
		if(sd_to_tick > SD_TIMEOUT)
			return E_EJ_SD_RDFAIL;
	}while(r == 0xff);

	if(r != 0xfe){
		debug.putbyte(sd_rd_sector[1]);
		debug.putbyte(sd_rd_sector[2]);
		debug.putbyte(sd_rd_sector[3]);
		debug.putbyte(sd_rd_sector[4]);
		debug.puttxt(" : ");
		debug.putbyte(r);
  		debug.puttxt(" ? ");
		getsdstatus();
		//return E_EJ_SD_RDFAIL;
		debug_puttxt("SD STOP 2!\r\n");	
		while(1)		//wait for wd //1201262v4.0.7m883
			;			
	}

	/**************************************************************************
	 * 
	 *************************************************************************/
	#if defined(SD_PDC)
	AT91C_BASE_SPI->SPI_TPR = (unsigned int) bigff;
	AT91C_BASE_SPI->SPI_TCR = SECTOR_SIZE;	//sizeof(bigff);
	AT91C_BASE_SPI->SPI_RPR = (unsigned int) d;
	AT91C_BASE_SPI->SPI_RCR = SECTOR_SIZE;	//sizeof(bigff);
	AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
	sd_to_tick = 0;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX)){
		if(AT91C_BASE_SPI->SPI_RCR < SECTOR_SIZE - 2){
			if(offset == 0x600){
				LED_TOGGLE(LED_ACT_HW);
				if(((uint8_t *) d)[0] != 0x55){
					LED_TOGGLE(LED_ACT_HW);
				}
			}
			break;
		}
		if(sd_to_tick > SD_TIMEOUT)
			return E_EJ_SD_RDFAIL;
	}

	sd_to_tick = 0;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX))
		if(sd_to_tick > SD_TIMEOUT)
			return E_EJ_SD_RDFAIL;

	#else
	for(i = 0; i != SECTOR_SIZE; i++)
		*dp++ = spi_getbyte();
	#endif
	/**************************************************************************
	 * 
	 *************************************************************************/
	sd_to_tick = 0;
	do{
		r = spi_getbyte();
		if(sd_to_tick > SD_TIMEOUT)
			return E_EJ_SD_RDFAIL;
	}while(r != 0xff);

	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
	spi_ini(SPI_INI_CS_DISABLE);
	npcs_to_tick1 = 0;

	/**************************************************************************
	 * 
	 *************************************************************************/
	#if defined(DEBUG_SD)
	debug.putch('\r');
	debug.putch('\n');
	//if((offset & 0xfffff) == 0){
		debug.putw(offset);
		debug.puttxt(" : ");
		for(i = 0; i != 16; i++)
			debug.putbyte(sectorbuff.raw[i]);
		debug.putch('\r');
		debug.putch('\n');
	//}
	#endif
	return E_EJ_OK;
}

/**************************************************************************
 * function: SD_wr()
 * desc: write a sector (512 byte) to SD
 *       SD_init() needed before
 *       wr time: ~ 17-200msec; 
 *             depends on auto block replace operation time
 *************************************************************************/
ej_operation_err_t SD_wr(const void *s, unsigned long offset)	    // watch out for the SDHC mode!
{
int	r;
int	i;
uint8_t	*sp = (uint8_t*)s;
ej_operation_err_t ret;
	#if 0
	debug.puthex("ws:", (unsigned int)s);
	debug.puthex("wo:", offset);
	#endif


	LED_TOGGLE(LED_ACT_HW);
	
	//100 usec wait for sure that prev _cs was finished
	//measured: min 500nsec needed
	while(!npcs_to_tick1)//1 * 100 usec
		;	


	/**************************************************************************
	 * 
	 *************************************************************************/
	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								SPI_NPCSX_SD |
								SPI_CS_DELAY;
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;

	/**************************************************************************
	 * 
	 *************************************************************************/
	#if defined(WATCH_FOR_LOW_SD_WRITES)
	if(offset < 0x2000){
		debug.puthex("------------------------------------------------------------------------i\al\al\aegal wrsec: ", offset);
	}
	#endif

	
	//doc: sd.Simplified_Physical_Layer_Spec.pdf
	//Table 7-3: Commands and Arguments
	//note "10. The unit of “data address” in argument is byte for Standard Capacity SD Memory Card and block (512 bytes) for High Capacity SD Memory Card.	"
	if(! sdhc)
		offset <<= 9;

	sd_wr_sector[1] = (uint8_t) ((offset >> 24) & 0xff);
	sd_wr_sector[2] = (uint8_t) ((offset >> 16) & 0xff);
	sd_wr_sector[3] = (uint8_t) ((offset >> 8) & 0xff);
	sd_wr_sector[4] = (uint8_t) (offset & 0xff);
	//debug.memdump("wrsect: ", sd_wr_sector, 6);
	#if 0
	debug.puttxt("wrsect: ");
	debug.putbyte(sd_wr_sector[1]);
	debug.putbyte(sd_wr_sector[2]);
	debug.putbyte(sd_wr_sector[3]);
	debug.putbyte(sd_wr_sector[4]);
  	debug.puttxt("\r\n");
	#endif

	/**************************************************************************
	 * 
	 *************************************************************************/

	sd_to_tick = 0;
	do{
		#if defined(DEBUG_SD)
		debug.putch('.');
		#endif
		r = spi_getbyte();
		if(sd_to_tick > SD_TIMEOUT){
			debug.puttxt("SD_wr() error1!; ");
			debug.putbyte(r);
			ret = E_EJ_SD_WRFAIL;
			goto ret_wr_sect;
		}			
	} while(r != 0xff);



	/**************************************************************************
	 * 
	 *************************************************************************/
	//getsdstatus();
	//debug.memdump("spibuff: ", spibuff, sizeof(spibuff));	
	
	/**************************************************************************
	 * 
	 *************************************************************************/

	SPI_PUTBLK("CMD24", sd_wr_sector, sizeof(sd_wr_sector));

	//poll til to get response 
	sd_to_tick = 0;
	do{
		#if defined(DEBUG_SD)
		debug.putch('.');
		#endif
		r = spi_getbyte();
		if(sd_to_tick > SD_TIMEOUT){
			debug.putbyte(r);
			debug.puttxt("SD_wr() error2!; ");
			ret = E_EJ_SD_WRFAIL;
			goto ret_wr_sect;
		}			
			
	} while(r == 0xff);

	//response token should be zero to show that there is no error in cmd
	//doc: pdf : 7.3.2.1 Format R1
	if(r != 0x00/* && r != 0xfe*/){
		debug.putbyte(sd_wr_sector[1]);
		debug.putbyte(sd_wr_sector[2]);
		debug.putbyte(sd_wr_sector[3]);
		debug.putbyte(sd_wr_sector[4]);
		debug.puttxt(" : ");
		debug.putbyte(r);
  		debug.puttxt(" F ");
		getsdstatus();
		debug.puttxt("SD_wr() error3!; ");
		ret = E_EJ_SD_WRFAIL;
		goto ret_wr_sect;
	}
	


	/**************************************************************************
	 * 
	 *************************************************************************/
	//start token byte
	//doc pdf: 7.3.3.2 Start Block Tokens and Stop Tran Token
	r = spi_putbyte(0xfe);
	#if defined(DEBUG_SD)
	debug.putbyte(r);
	#endif

	//data block
	#if defined(SD_PDC)
	AT91C_BASE_SPI->SPI_TPR = (unsigned int) s;
	AT91C_BASE_SPI->SPI_TCR = SECTOR_SIZE + 2;
	AT91C_BASE_SPI->SPI_RPR = (unsigned int) spibuff;
	AT91C_BASE_SPI->SPI_RCR = SECTOR_SIZE + 2;
	AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
	sd_to_tick = 0;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX))
		if(sd_to_tick > SD_TIMEOUT){
			debug.puttxt("SD_wr() error4!; ");
			ret = E_EJ_SD_RDFAIL;
			goto ret_wr_sect;
		}
	#else
	for(i = 0; i != SECTOR_SIZE + 2; i++){
		spi_putbyte(*sp++);
	}
	#endif

	sd_to_tick = 0;
	do{
		r = spi_getbyte();
		#if defined(DEBUG_SD)
		debug.putbyte(r);
		#endif
		if(sd_to_tick > SD_TIMEOUT){
			debug.puttxt("SD_wr() error5!; ");
			ret = E_EJ_SD_WRFAIL;
			goto ret_wr_sect;
		}
	}while(r != 0x00);


	/**************************************************************************
	 * 
	 *************************************************************************/
	#if defined(WD_TEST)
	while(wd_test & WD_TEST_WRSECT)
		;
	#endif

	ret = E_EJ_OK;

ret_wr_sect:
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
	spi_ini(SPI_INI_CS_DISABLE);
	npcs_to_tick1 = 0;
	
	
	//wrsect()->measured:1msec

	return ret;
}

#if 0
// 2G and 4G cards behave differently
2G Kingston
unlocked card
read
00000000 - FFFFFFFFFFFFFFFF0000FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

unlock
cmd42FF00 -> FFFFFFE50000000000FFst:FFFFFFFFFFFFFFFF0002FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0002FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

set pw
cmd42FF00 -> FFFFFFE5000000000000FFst:FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0000FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

lock
cmd42FF00 -> FFFFFFE50000000000FFst:FFFFFFFFFFFFFFFF0001FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0001FFFFFFFFFFFF00000000 : 04 F st:FFFFFFFFFFFFFF0001FFFFFFFFFFFFFFFFFFFFFFFFFF

unlock
cmd42FF00 -> FFFFFFE50000000000FFst:FFFFFFFFFFFFFFFF0003FFFFFFFFFFFFFFFFFFFFFFFF (failed ? ? ?)
read (this SHOULD WORK!!!!)
00000000 - FFFFFFFFFFFFFFFF0003FFFFFFFFFFFF00000000 : 04 F st:FFFFFFFFFFFFFF0001FFFFFFFFFFFFFFFFFFFFFFFFFF

clear pw
cmd42FF00 -> FFFFFFE50000000000000000FFst:FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0000FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

4G SanDisk
unlocked card
read
00000000 - FFFFFFFFFFFFFFFF0000FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

unlock
cmd42FF00 -> FFFFFFE500000003FFst:FFFFFFFFFFFFFFFF0002FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0002FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

set pw
cmd42FF00 -> FFFFFFE500000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000000000000000000000000000003FFst:
FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0000FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

lock
cmd42FF00 -> FFFFFFE50003FFst:FFFFFFFFFFFFFFFF0001FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0001FFFFFFFFFFFF00000000 : 04 F st:FFFFFFFFFFFFFFC13FFFFFFFFFFFFFFFFFFFFFFFFFFF

unlock
cmd42FF00 -> FFFFFFE50003FFst:FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0000FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

clear pw
cmd42FF00 -> FFFFFFE500000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000
0000000000000000000000000000000000000000000000000000000000000000000000003FFst:FF
FFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFF
read
00000000 - FFFFFFFFFFFFFFFF0000FFFFFFFFFFFF00000000 : 00000000000000000000000000000000

well, and it seems that the 4G card will wake up UNLOCKED!
#endif

ej_operation_err_t lock_sdcard(int lockflag, const char *pw, const char *oldpw)
{
int	r;
int	j;
int	pwl = 0;
int	opwl = 0;

	if(pw != NULL)
		pwl = strlen(pw);
	if(oldpw != NULL)
		opwl = strlen(oldpw);

	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								SPI_NPCSX_SD |
								SPI_CS_DELAY;
	r = AT91C_BASE_SPI->SPI_RDR & 0xff;

#if defined(LOCK_BLOCK_RESIZE)
	sd_cmd16[4] = 1 + 1 + pwl + opwl;
	SPI_PUTBLK("CMD16", sd_cmd16, sizeof(sd_cmd16));
#endif

	SPI_PUTBLK("CMD42", sd_cmd42, sizeof(sd_cmd42));

	for(j = 0; j != 16; j++){
		r = spi_getbyte();
#if defined(DEBUG_SD)
		debug.putbyte(r);
#endif
		if(r != 0xff)
			break;
	}

	if(r == 0xff){	                    // ????
		do{
#if defined(DEBUG_SD)
			debug.putch('.');
#endif
			r = spi_getbyte();
		} while(r == 0xff);
	}

	if(r != 0x00/* && r != 0xfe*/){
		debug.puttxt(" : ");
		debug.putbyte(r);
  		debug.puttxt(" F ");
		getsdstatus();
		return E_EJ_SD_LOCKFAIL;
	}

  	debug.puttxt(" -> ");
	spi_putbyte(0xfe);
	spi_putbyte(lockflag);
	spi_putbyte(pwl + opwl);

	if(pwl){
		AT91C_BASE_SPI->SPI_TPR = (unsigned int) pw;
		AT91C_BASE_SPI->SPI_TCR = pwl;
		AT91C_BASE_SPI->SPI_RPR = (unsigned int) spibuff;
		AT91C_BASE_SPI->SPI_RCR = pwl;
		AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
		while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX))
				;
	}
	if(opwl){
		AT91C_BASE_SPI->SPI_TPR = (unsigned int) oldpw;
		AT91C_BASE_SPI->SPI_TCR = opwl;
		AT91C_BASE_SPI->SPI_RPR = (unsigned int) spibuff;
		AT91C_BASE_SPI->SPI_RCR = opwl;
		AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
		while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX))
				;
	}
#if !defined(LOCK_BLOCK_RESIZE)
	AT91C_BASE_SPI->SPI_TPR = (unsigned int) bigff;
	AT91C_BASE_SPI->SPI_TCR = SECTOR_SIZE - pwl - opwl;
	AT91C_BASE_SPI->SPI_RPR = (unsigned int) spibuff;
	AT91C_BASE_SPI->SPI_RCR = SECTOR_SIZE - pwl - opwl;
	AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX))
			;
#endif
	do{
		WATCHDOG_RESET;
		r = spi_getbyte();
#if defined(DEBUG_SD)
		debug.putbyte(r);
#endif
	}while(r != 0xe5);	                // data accepted

	do{
		WATCHDOG_RESET;
		r = spi_getbyte();
#if defined(DEBUG_SD)
		debug.putbyte(r);
#endif
	}while(r != 0xff);	                // idle

	r = AT91C_BASE_SPI->SPI_RDR;
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
	r = spi_getbyte();
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;

	getsdstatus();

#if defined(LOCK_BLOCK_RESIZE)
	SPI_PUTBLK("CMD16/512", sd_cmd16_512, sizeof(sd_cmd16_512));
#endif

	return E_EJ_OK;
}
