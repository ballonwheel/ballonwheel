/**
 * @file vioctrl.h
 * @brief VIOC & HDMI input handling
 *
 * Functions and command line utility to handle the 
 *  Video Input Output Contoller (VIOC) periphery
 *  and the HDMI input chip
 *
 * Author: Balint Viragh <bviragh@dension.com>
 * Copyright (C) 2013 Dension Audio Systems Kft.
 */

/**
 * @addtogroup VIOCTRL
 * @{   
 */

/*================================[ INCLUDES ]===============================*/
/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

/* external library headers */   

/* Interface of other modules */
#include "scale_calc.h"

/* Interface header of this module */
#include "vioctrl.h"

/*================================[ INTERNAL MACROS ]========================*/

/** path of the used I2C bus device */
#define L_ST_I2C_DEV                    "/dev/i2c-2"

/** I2C slave address of the HDMI input chip */
#define L_U8_HDMICHIP_I2C_ADDR          0x3C

/** Address of the Timing Registers */
#define L_U8_HDMICHIP_TIMING_REGS_ADDR  0x3B

/** Number of Timing Registers */
#define L_U8_HDMICHIP_TIMING_REGS_LEN   13

/** Address of the AVI InfoFrame Registers */
#define L_U8_HDMICHIP_AVIINFO_REGS_ADDR 0x2A

/** Number of AVI InfoFrame Registers */
#define L_U8_HDMICHIP_AVIINFO_REGS_LEN  15

/** Address of the ADO InfoFrame Registers */
#define L_U8_HDMICHIP_ADOINFO_REGS_ADDR 0x2B

/** Number of ADO InfoFrame Registers */
#define L_U8_HDMICHIP_ADOINFO_REGS_LEN  7

/** Address of the EDID Data Registers */
#define L_U8_HDMICHIP_EDID_REGS_ADDR    0xFF

/** Number of EDID Data Registers */
#define L_U32_HDMICHIP_EDID_REGS_LEN    256

/** Address of the RX PHY control register */
#define L_U8_HDMICHIP_RXPHY_REG_ADDR    0x49

/** Address of the Control 1 register */
#define L_U8_HDMICHIP_CONTROL1_REG_ADDR 0x41

/** Address of the Status 1 register */
#define L_U8_HDMICHIP_STATUS1_REG_ADDR  0x3D

/** Default value for the RX PHY hdmi input chip register */
#define L_U8_DEFAULT_HDMICHIP_RXPHY     0x01

/** Base Address for HDCP keys */
#define L_U8_HDMICHIP_HDCP_KEYS_BASE_ADDR   0x00

/** Address for HDCP KSV */
#define L_U8_HDMICHIP_HDCP_KSV_ADDR     0x28

/** LINK_ON bit of the status 1 register */
#define L_U8_HDMICHIP_LINK_ON           (1 << 7)

/** DE_VALID bit of the status 1 register */
#define L_U8_HDMICHIP_DE_VALID          (1 << 6)

/** memory device path */
#define L_ST_DEV_MEM                    "/dev/mem"

/** Base address of the GPIO registers */
#define L_U32_GPIO_MEM_BASE             0x74200000

/** Size of the GPIO register address range */
#define L_U32_GPIO_MEM_SIZE             0x260

/** Base address of the VIOC registers */
#define L_U32_VIOC_MEM_BASE             0x72000000

/** Size of the VIOC register address range */
#define L_U32_VIOC_MEM_SIZE             0x10210

/** Offset for the GPBEN register */
#define L_U32_GPIO_GPBEN                0x044

/** Offset for the GPEEN register */
#define L_U32_GPIO_GPEEN                0x104

/** Offset for the GPGEN register */
#define L_U32_GPIO_GPGEN                0x184

/** Offset for the GPBFN0 register */
#define L_U32_GPIO_GPBFN0               0x070

/** Offset for the GPBFN1 register */
#define L_U32_GPIO_GPBFN1               0x074

/** Offset for the GPBFN2 register */
#define L_U32_GPIO_GPBFN2               0x078

/** Offset for the GPBFN3 register */
#define L_U32_GPIO_GPBFN3               0x07C

/** GPIO function mode 0 value */
#define L_U32_GPIO_MODE0                0x00000000

/** GPIO function mode 5 value */
#define L_U32_GPIO_MODE5                0x55555555

/** Offset for VIN0 VIOC component */
#define L_U32_VIOC_BASE_VIN0            0x04000

/** Offset for VIN1 VIOC component */
#define L_U32_VIOC_BASE_VIN1            0x05000

/** Offset for VIN2 VIOC component */
#define L_U32_VIOC_BASE_VIN2            0x06000

/** Offset for VIN3 VIOC component */
#define L_U32_VIOC_BASE_VIN3            0x07000

/** Offset for RDMA00 VIOC component */
#define L_U32_VIOC_BASE_RDMA00          0x00400

/** Offset for RDMA01 VIOC component */
#define L_U32_VIOC_BASE_RDMA01          0x00500

/** Offset for RDMA02 VIOC component */
#define L_U32_VIOC_BASE_RDMA02          0x00600

/** Offset for RDMA03 VIOC component */
#define L_U32_VIOC_BASE_RDMA03          0x00700

/** Offset for RDMA04 VIOC component */
#define L_U32_VIOC_BASE_RDMA04          0x00800

/** Offset for RDMA05 VIOC component */
#define L_U32_VIOC_BASE_RDMA05          0x00900

/** Offset for RDMA06 VIOC component */
#define L_U32_VIOC_BASE_RDMA06          0x00A00

/** Offset for RDMA07 VIOC component */
#define L_U32_VIOC_BASE_RDMA07          0x00B00

/** Offset for RDMA12 VIOC component */
#define L_U32_VIOC_BASE_RDMA12          0x01000

/** Offset for RDMA13 VIOC component */
#define L_U32_VIOC_BASE_RDMA13          0x01100

/** Offset for RDMA14 VIOC component */
#define L_U32_VIOC_BASE_RDMA14          0x01200

/** Offset for RDMA15 VIOC component */
#define L_U32_VIOC_BASE_RDMA15          0x01300

/** Offset for RDMA16 VIOC component */
#define L_U32_VIOC_BASE_RDMA16          0x01400

/** Offset for RDMA17 VIOC component */
#define L_U32_VIOC_BASE_RDMA17          0x01500

/** Offset for RDMA12 VIOC component */
#define L_U32_VIOC_BASE_RDMA12          0x01000

/** Offset for WDMA00 VIOC component */
#define L_U32_VIOC_BASE_WDMA00          0x02800

/** Offset for WDMA01 VIOC component */
#define L_U32_VIOC_BASE_WDMA01          0x02900

/** Offset for WDMA03 VIOC component */
#define L_U32_VIOC_BASE_WDMA03          0x02B00

/** Offset for WDMA04 VIOC component */
#define L_U32_VIOC_BASE_WDMA04          0x02C00

/** Offset for WDMA05 VIOC component */
#define L_U32_VIOC_BASE_WDMA05          0x02D00

/** Offset for WDMA06 VIOC component */
#define L_U32_VIOC_BASE_WDMA06          0x02E00

/** Offset for WDMA07 VIOC component */
#define L_U32_VIOC_BASE_WDMA07          0x02F00

/** Offset for WDMA08 VIOC component */
#define L_U32_VIOC_BASE_WDMA08          0x03000

/** Offset for WMIX0 VIOC component */
#define L_U32_VIOC_BASE_WMIX0           0x01800

/** Offset for WMIX1 VIOC component */
#define L_U32_VIOC_BASE_WMIX1           0x01900

/** Offset for WMIX3 VIOC component */
#define L_U32_VIOC_BASE_WMIX3           0x01B00

/** Offset for WMIX4 VIOC component */
#define L_U32_VIOC_BASE_WMIX4           0x01C00

/** Offset for WMIX5 VIOC component */
#define L_U32_VIOC_BASE_WMIX5           0x01D00

/** Offset for SC0 VIOC component */
#define L_U32_VIOC_BASE_SC0             0x02000

/** Offset for SC1 VIOC component */
#define L_U32_VIOC_BASE_SC1             0x02100

/** Offset for SC2 VIOC component */
#define L_U32_VIOC_BASE_SC2             0x02200

/** Offset for the CFG_PATH_SC0 register */
#define L_U32_VIOC_CFG_PATH_SC0         0x00A044

/** Offset for the CFG_PATH_SC1 register */
#define L_U32_VIOC_CFG_PATH_SC1         0x00A048

/** Offset for the CFG_PATH_SC2 register */
#define L_U32_VIOC_CFG_PATH_SC2         0x00A04C

/** Offset of the VIN_CTRL register */
#define L_U32_VIOC_VIN_CTRL             0x000

/** Offset of the VIN_SIZE register */
#define L_U32_VIOC_VIN_SIZE             0x010

/** Offset of the SCCTRL regsiter */
#define L_U32_VIOC_SCCTRL               0x00

/** Offset of the SCDSIZE regsiter */
#define L_U32_VIOC_SCDSIZE              0x0C

/** Offset of the SCOPOS regsiter */
#define L_U32_VIOC_SCOPOS               0x10

/** Offset of the SCOSIZE regsiter */
#define L_U32_VIOC_SCOSIZE              0x14

/** Default Video input control register value */
#define L_U32_DEFAULT_VIN_CTRL          0x00090600

/** Default RDMA mode/control register value */
#define L_U32_DEFAULT_RDMACTRL          0x1181040C

/** HDMI Stop mode RDMA mode/control register value */
#define L_U32_STOP_RDMACTRL             0x00010000

/** Default WDMA mode/control register value */
#define L_U32_DEFAULT_WDMACTRL          0x1081000C

/** HDMI stop mode WDMA mode/control register value */
#define L_U32_STOP_WDMACTRL             0x00010000

/** Default WMIX layer priority / control register value */
#define L_U32_DEFAULT_WMIX_MCTRL        0x00010001

/** HDMI Stop mode WMIX layer priority / control register value */
//#define L_U32_STOP_WMIX_MCTRL           0x00010000
#define L_U32_STOP_WMIX_MCTRL           ((uint32_t)(L_U32_HIDE_WMIX_MCTRL))

/** Hide HDMI input WMIX layer priority / control register value */
#define L_U32_HIDE_WMIX_MCTRL           0x00010003

/** Default Scaler control register value */
#define L_U32_DEFAULT_SCCTRL            0x00010000

/** ByPass Scaler control register value */
#define L_U32_BYPASS_SCCTRL             0x00010001

/** macro to get the SEL field of a Scaler Path Configuration register value */
#define L_GET_PATHSC_SEL(p_u32_reg)     ((uint32_t)((p_u32_reg) & 0xFF))

/** macro to get the STS field of a Scaler Path Configuration register value */
#define L_GET_PATHSC_STS(p_u32_reg)     ((uint32_t)(((p_u32_reg) >> 16) & 0x03))

/** DISCONNECTED scaler status value */
#define L_U32_SCALER_DISCONNECTED       0

/** CONNECTED scaler status value */
#define L_U32_SCALER_CONNECTED          2

/** timeout counter for scaler connecting/disconnecting wait */
#define L_SCALER_WAIT_TIMEOUT_CNT       10u

/** sleep time while waiting for scaler conencting/disconnecting in [ms] */
#define L_SCALER_WAIT_SLEEP_MS          100

/** by default number of bytes per pixel */
#define L_U32_AUTO_BYTES_PER_PIXEL      4

/** physical address of the DMA buffer */
#define L_U32_AUTO_DMABUF_ADDR          0x87000000

/** maximal width */
#define L_U32_AUTO_MAX_WIDTH            1920

/** maximal Height */
#define L_U32_AUTO_MAX_HEIGHT           1080

/** ID of the RDMA used for HDMI input overlay */
#define L_U32_AUTO_RDMA_ID              5

/** ID of the WDMA used for HDMI input overlay */
#define L_U32_AUTO_WDMA_ID              7

/** ID of the VIN used for HDMI input overlay */
#define L_U32_AUTO_VIN_ID               1

/** ID of the WMIX used for HDMI input overlay */
#define L_U32_AUTO_WMIX_ID              1

/** Channel of the WMIX used for HDMI input overlay */
#define L_U32_AUTO_WMIX_CH              1

/** ID of the Scaler used for HDMI input overlay */
#define L_U32_AUTO_SC_ID                0

/** RDMASCALE value for 1:1 scaling */
#define L_U8_RDMA_UPSCALE_1     0x0
/** RDMASCALE value for 1:2 scaling */
#define L_U8_RDMA_UPSCALE_2     0x1
/** RDMASCALE value for 1:3 scaling */
#define L_U8_RDMA_UPSCALE_3     0x2
/** RDMASCALE value for 1:4 scaling */
#define L_U8_RDMA_UPSCALE_4     0x3
/** RDMASCALE value for 1:8 scaling */
#define L_U8_RDMA_UPSCALE_8     0x7

/** Downscale ration during Scaler initialization */
#define L_F32_SCINIT_DOWNSCALE  1.1

/** Path of /proc/pmap */
#define L_ST_PROC_PMAP          "/proc/pmap"
/** name of the used memory area */
#define L_ST_PMAP_EXTCAM        "ext_camera"
/** minimal size of the memory area */
#define L_U32_MIN_MEMSIZE       ((uint32_t)(1920 * 1080 * 4))
/** delimiter used during pmap file parsing */
#define L_ST_DELIM              " \t\n"

/** bytes per pixels of the scrolled image */
#define L_U32_SCROLL_BYTES_PER_PIXEL 4
/** RDMA control register value during scrolling */
#define L_U32_SCROLL_RDMACTRL   0x11815c0c

/** debug parameter */
#define L_ST_DEBUG              "debug"

/** 1080p full HD width */
#define L_U32_1080P_WIDTH       1920
/** 1080p full HD height */
#define L_U32_1080P_HEIGHT      1080

/** HDCP KSV (Key Sector Vector) length in bytes (40 bit -> 5 bytes) */
#define L_HDCP_KSV_LEN          5
/** HDCP Key length in bytes (56 bit -> 7 bytes) */
#define L_HDCP_KEY_LEN          7
/** Number of HDCP Keys */
#define L_HDCP_KEY_NUM          40

/*================================[ INTERNAL TYPEDEFS ]======================*/

/** Structure to store Timing values read from timing registers */
typedef struct {
    uint32_t u32_appl;      /**< Active Pixels Per Line (Width) */
    uint32_t u32_hfp;       /**< Horizontal Front Porch */
    uint32_t u32_hbp;       /**< Horizontal Back Porch */
    uint32_t u32_hpw;       /**< Horizontal Pulse Width */
    uint32_t u32_alpf;      /**< Active Lines Per Frame (Height) */
    uint32_t u32_vfp;       /**< Vertical Front Porch */
    uint32_t u32_vbp;       /**< Vertical Back Porch */
    uint32_t u32_vpw;       /**< Vertical Pulse Width */
    int b_intl;             /**< Interlaced / Progressive flag */
} l_s_timings_t;

/** Structure to store AVI InfoFrame values read from hdmichip regsiters */
typedef struct {
    uint8_t au8_data[L_U8_HDMICHIP_AVIINFO_REGS_LEN]; /**< raw data */
} l_s_avi_info_t;

/** Structure to store ADO InfoFrame (audio) values read from hdmichip regsiters */
typedef struct {
    uint8_t au8_data[L_U8_HDMICHIP_ADOINFO_REGS_LEN]; /**< raw data */
} l_s_ado_info_t;

/** structure to store a HDCP key */
typedef struct {
    uint8_t au8_ksv[L_HDCP_KSV_LEN];    /**< HDCP Key Selection Vector */
    uint8_t au8_keys[L_HDCP_KEY_NUM][L_HDCP_KEY_LEN];   /**< 40 HDCP Keys*/
} l_s_hdcp_key_t;

/*================================[ INTERNAL FUNCTION PROTOTYPES ]===========*/

static void l_usage_f(const char* p_st_arg0);
static void l_hdmichip_cmd_f(int argc, char* argv[]);
static void l_pins_cmd_f(int argc, char* argv[]);
static void l_vin_cmd_f(int argc, char* argv[]);
static void l_dma_cmd_f(int argc, char* argv[]);
static void l_wmix_cmd_f(int argc, char* argv[]);
static void l_sc_cmd_f(int argc, char* argv[]);
static void l_auto_cmd_f(int argc, char* argv[]);
static void l_load_save_cmd_f(int argc, char* argv[]);
static void l_scroll_cmd_f(int argc, char* argv[]);
static int l_i2c_init_f(void);
static int l_i2c_get_f(uint8_t p_u8_addr, uint8_t* p_u8p_value);
static int l_i2c_set_f(uint8_t p_u8_addr, uint8_t p_u8_value);
static int l_get_regs_timing_f(l_s_timings_t* p_sp_timings);
static int l_get_regs_avi_f(l_s_avi_info_t* p_sp_avi_info);
static int l_get_regs_ado_f(l_s_ado_info_t* p_sp_ado_info);
//static int l_get_regs_edid_f(uint8_t* p_au8_buf);
static int l_set_regs_edid_f(const uint8_t* p_au8_buf);
static int l_set_regs_hdcp_f(const l_s_hdcp_key_t* p_sp_hdcp_key);
static void l_dump_info_timing_f(void);
static void l_dump_info_avi_f(void);
static void l_dump_info_audio_f(void);
static void* l_mem_init_f(int* p_fdp_mem, uint32_t p_u32_base, uint32_t* p_u32p_size);
static int l_mem_close_f(int p_fd_mem, void* p_u32p_base, uint32_t p_u32_size);
static int l_configure_pins_f(int p_b_mode);
static int l_configure_vin_f(uint32_t p_u32_id, uint32_t p_u32_width, 
                             uint32_t p_u32_height, int p_b_start);
static int l_configure_dma_f(int p_b_write, uint32_t p_u32_address, 
                             uint32_t p_u32_id, uint32_t p_u32_width, 
                             uint32_t p_u32_height, uint32_t p_u32_upscale_x,
                             uint32_t p_u32_upscale_y, uint32_t p_u32_bpl,
                             uint32_t p_u32_mode);
static int l_configure_wmix_f(uint32_t p_u32_id, uint32_t p_u32_ch, 
                             uint32_t p_u32_pos_x, uint32_t p_u32_pos_y, 
                             uint32_t p_u32_layer_prio);
static int l_configure_sc_f(uint32_t p_u32_id, uint32_t p_u32_rdma, 
                             uint32_t p_u32_width, uint32_t p_u32_height,
                             uint32_t p_u32_out_width, uint32_t p_u32_out_height,
                             uint32_t p_u32_pos_x, uint32_t p_u32_pos_y);
static int l_stop_sc_f(uint32_t p_u32_id);
/* convert upscale factor to register value */
static inline int l_get_rdma_upscale_f(uint32_t p_u32_factor);
static inline uint32_t l_get_scaler_sel_rdma_f(uint32_t p_u32_rdma);
static inline uint32_t l_get_scaler_sel_wdma_f(uint32_t p_u32_rdma);
static int l_init_sc_f(const scale_s_calc_in_t* p_sp_in);
static int l_stop_f(void);
static int l_scroll_loop_f(uint32_t p_u32_id, uint32_t p_u32_base, 
                           uint32_t p_u32_ctrl, uint32_t u32_sleep_ms,
                           uint32_t p_u32_text_w, uint32_t p_u32_text_h, 
                           uint32_t p_u32_win_w, uint32_t p_u32_win_h);
static int l_hex_to_bin_f(const char* p_cp_hex, uint8_t* p_au8_bin, uint32_t p_u32_size);
static int l_parse_hdcp_key_file_f(l_s_hdcp_key_t* p_sp_hdcp_key, const char* p_cp_file);

/*================================[ INTERNAL GLOBALS ]=======================*/

static uint32_t l_u32_dmabuf_addr = 0x0;
// original EDID, comes from ADV SDK
const uint8_t APP_edid_table[256]=
{
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00, // 8 bytes, start of EDID data, page 97
0x06,0x96,                               // manufacturer, EISA ID, page 98
0x48,0x44,                               // product code
0x01,0x00,0x00,0x00,                     // serial number
0x25,                                    // Week of Manufacture
0x10,                                    // Year of Manufacture / ModelYear
0x01,                                    // EDID version
0x03,                                    // EDID revision
0x80,0x31,0x1C,0xA0,0x2A,                // Basic Display Parameters and Features Block, page 101
0xAA,0x33,0xA4,0x55,0x48,0x93,0x25,0x10,0x45,0x47, // Color Characteristics Block
0xFF,0xFF,0x80,                          // Established Timings Block, page 102
0x81,0x8F,0x81,0x99,0xA9,0x40,0x61,0x59, // Standard Timing ID Block, page 103
0x45,0x59,0x31,0x59,0x71,0x4A,0x81,0x40,

0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40, // First Detailed Timing Descriptor Block, page 104
0x46,0x28,0x55,0x00,0xE8,0x12,0x11,0x00,
0x00,0x18,

0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,0x20, // Second Detailed Timing Descriptor Block, page 105
0x46,0x28,0x55,0x00,0xE8,0x12,0x11,0x00,
0x00,0x18,

0x00,0x00,0x00,0xFC,0x00,0x48,0x44,0x4D, // First Monitor Descriptor Block
0x49,0x20,0x54,0x56,0x0A,0x20,0x20,0x20,
0x20,0x20,

0x00,0x00,0x00,0xFD,0x00,0x1D,0x56,0x0F, // Second Monitor Descriptor Block
0x6F,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,

0x01,                                    // Extension flag
0xAB,                                    // chksum

0x02,                                    // Tag
0x03,                                    // revision 
0x21,                                    // offset d where 18-byte descriptors begin
0x71,                                    // Total number of Detailed Timing, page 86
0x4D,                                    // Start of data block collection

0x90,0x04,0x03,0x12,0x01,0x13,0x11,0x07,
0x05,0x06,0x22,0x14,0x15,0x23,0x0F,0x57,
0x07,0x83,0x4F,0x00,0x00,0x66,0x03,0x0C,
0x00,0x10,0x00,
0x80,0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,
0x20,0x6E,0x28,0x55,0x00,0xE8,0x12,0x11,
0x00,0x00,0x1E,0x8C,0x0A,0xD0,0x90,0x20,
0x40,0x31,0x20,0x0C,0x40,0x55,0x00,0xE8,
0x12,0x11,0x00,0x00,0x18,0x8C,0x0A,0xD0,
0x8A,0x20,0xE0,0x2D,0x10,0x10,0x3E,0x96,
0x00,0xE8,0x12,0x11,0x00,0x00,0x18,0x01,
0x1D,0x80,0x18,0x71,0x1C,0x16,0x20,0x58,
0x28,0x25,0x00,0xE8,0x12,0x11,0x00,0x00,
0x98,0x01,0x1D,0x00,0xBC,0x52,0xD0,0x1E,
0x20,0xB8,0x28,0x55,0x40,0xE8,0x12,0x11,
0x00,0x00,0x1E,0x00,0x00,0x00,0x00,0x4F,
};

// clone from pioneer HU
const uint8_t APP_edid_table_pioneer_sph_da100[256]=
{
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x41,0x2F,0x03,0x80,0x00,0x00,0x00,0x00,
0x00,0x16,0x01,0x03,0x80,0x10,0x08,0x78,0x0A,0xC4,0x55,0xA3,0x56,0x4D,0xA1,0x28,
0x0F,0x50,0x54,0x20,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,0x20,0x6E,0x28,
0x55,0x00,0x9C,0x52,0x00,0x00,0x00,0x1E,0x8C,0x0A,0xD0,0x8A,0x20,0xE0,0x2D,0x10,
0x10,0x3E,0x96,0x00,0x9C,0x52,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0xFC,0x00,0x53,
0x50,0x48,0x2D,0x44,0x41,0x31,0x30,0x30,0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFD,
0x00,0x3B,0x3D,0x1F,0x2E,0x08,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,0x01,0xAE,
0x02,0x03,0x18,0x70,0x44,0x04,0x03,0x02,0x01,0x23,0x09,0x1F,0x07,0x83,0x01,0x00,
0x00,0x66,0x03,0x0C,0x00,0x10,0x00,0x00,0xD6,0x09,0x80,0xA0,0x20,0xE0,0x2D,0x10,
0x10,0x60,0xA2,0x00,0x9C,0x52,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76
};


// 16:9 display only, 16x8cm
// 60fps, 720x480 only
// 44khz 16bit audio only
const uint8_t APP_edid_table_720x480_60fps_44khz16bitstereo_dension[256]=
{
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x11, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x01, 0x17, 0x01, 0x03, 0x80, 0x10, 0x09, 0x78, 0x02, 0xC4, 0x55, 0xA3, 0x56, 0x4D, 0xA1, 0x28, 
0x0F, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 
0x96, 0x00, 0xB4, 0x5A, 0x00, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 
0x10, 0x3E, 0x96, 0x00, 0xA0, 0x5A, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x53, 
0x6D, 0x61, 0x72, 0x74, 0x4C, 0x69, 0x6E, 0x6B, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
0x00, 0x3A, 0x3D, 0x1E, 0x2E, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xF6, 
0x02, 0x03, 0x14, 0x70, 0x41, 0x83, 0x23, 0x09, 0x02, 0x01, 0x83, 0x01, 0x00, 0x00, 0x65, 0x03, 
0x0C, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 
};

// 16:9 display only, 16x8cm
// 50fps, 720x576 for first
// 60fps, 720x480 for second
// 44khz 16bit audio only
const uint8_t APP_edid_table_720x576_50fps_44khz16bitstereo_dension[256]=
{
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x11, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x01, 0x17, 0x01, 0x03, 0x80, 0x10, 0x09, 0x78, 0x02, 0xC4, 0x55, 0xA3, 0x56, 0x4D, 0xA1, 0x28, 
0x0F, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 
0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x8C, 0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20, 0x0C, 0x40, 
0x55, 0x00, 0xA0, 0x5A, 0x00, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 
0x10, 0x3E, 0x96, 0x00, 0xA0, 0x5A, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x53, 
0x6D, 0x61, 0x72, 0x74, 0x4C, 0x69, 0x6E, 0x6B, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD, 
0x00, 0x31, 0x3D, 0x1D, 0x2E, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xDD, 
0x02, 0x03, 0x15, 0x71, 0x42, 0x92, 0x03, 0x23, 0x09, 0x02, 0x01, 0x83, 0x01, 0x00, 0x00, 0x65, 
0x03, 0x0C, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 
};

static const uint8_t* au8p_default_edid_tables[] = {
    APP_edid_table,
    APP_edid_table_pioneer_sph_da100,
    APP_edid_table_720x480_60fps_44khz16bitstereo_dension,
    APP_edid_table_720x576_50fps_44khz16bitstereo_dension,
};

#define L_DEFAULT_EDID_TABLES_NUM  ((uint32_t)((sizeof(au8p_default_edid_tables)) / (sizeof(au8p_default_edid_tables[0]))))

/*================================[ EXTERNAL GLOBALS ]=======================*/

/*================================[ INTERNAL FUNCTION DEFINITIONS ]==========*/

static void l_usage_f(const char* p_st_arg0)
{
    fprintf(stderr, "Usage: \n");
    fprintf(stderr, "%s hdmichip get <addr>\n", p_st_arg0);
    fprintf(stderr, "%s hdmichip set <addr> <value>\n", p_st_arg0);
    fprintf(stderr, "%s hdmichip info [all|avi|audio|ms|timing]=all\n", p_st_arg0);
//    fprintf(stderr, "%s hdmichip status\n", p_st_arg0);
//    fprintf(stderr, "%s hdmichip read <bitfield_name>\n", p_st_arg0);
//    fprintf(stderr, "%s hdmichip write <bitfield_name> <value>\n", p_st_arg0);
    fprintf(stderr, "%s hdmichip load <edid|hdcp> <file>\n", p_st_arg0);
    fprintf(stderr, "%s pins <cif|gpio>\n", p_st_arg0);
    fprintf(stderr, "%s vin <id> <width> <height> [start]=1\n", p_st_arg0);
    fprintf(stderr, "%s wdma <id> <address> <width> <height> <bytes-in-a-line> [mode]\n", p_st_arg0);
    fprintf(stderr, "%s rdma <id> <address> <width> <height> <bytes-in-a-line> [mode]\n", p_st_arg0);
    fprintf(stderr, "%s wmix <id> <ch> <x_pos> <y_pos> [layer_prio]=1\n", p_st_arg0);
    fprintf(stderr, "%s sc <id> <rdma> <scaled_width> <scaled_height>\n", p_st_arg0);
    fprintf(stderr, "%s start <width> <height> <x_pos> <y_pos> [TOPLEFT|CENTER|SCALE|CROPFIT|ALLFIT(default)]\n", p_st_arg0);
    fprintf(stderr, "%s stop\n", p_st_arg0);
    fprintf(stderr, "%s load <address> <file>\n", p_st_arg0);
    fprintf(stderr, "%s save <address> <file> <length>\n", p_st_arg0);
    fprintf(stderr, "%s scroll <sleep_ms> <file> <text_width> <text_height> <x_pos> <y_pos> <win_width> <win_height>\n", p_st_arg0);
}

static void l_hdmichip_cmd_f(int argc, char* argv[])
{
    const char* cp_cmd = argv[2];
    const char* cp_addr = NULL;
    const char* cp_value = NULL;
    char* cp_err;
    uint8_t u8_addr;
    uint8_t u8_value;
    int res;
    const char* cp_area = NULL;
    const char* cp_file = NULL;
    FILE* sp_file;
    uint8_t au8_edid_data[L_U32_HDMICHIP_EDID_REGS_LEN];
    l_s_hdcp_key_t s_hdcp_key;

    if(argc < 3) {
        fprintf(stderr, "not enough arguments for hdmichip %s\n", cp_cmd);
        return;
    }

    if(0 == strcasecmp(cp_cmd, "get")) {
        if(argc < 4) {
            fprintf(stderr, "not enough arguments for hdmichip %s\n", cp_cmd);
            return;
        }
        cp_addr = argv[3];
        u8_addr = (uint8_t)strtoul(cp_addr, &cp_err, 16);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_addr);
            return;
        }
        res = l_i2c_get_f(u8_addr, &u8_value);
        if(res) {
            fprintf(stderr, "Can not get I2C [%.2x] (%d)\n", u8_addr, res);
            return;
        }
        printf("HDMICHIP [%.2x]: %.2x\n", u8_addr, u8_value);
    } else if(0 == strcasecmp(cp_cmd, "set")) {
        if(argc < 5) {
            fprintf(stderr, "not enough arguments for hdmichip %s\n", cp_cmd);
            return;
        }
        cp_addr = argv[3];
        cp_value = argv[4];
        u8_addr = (uint8_t)strtoul(cp_addr, &cp_err, 16);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_addr);
            return;
        }
        u8_value = (uint8_t)strtoul(cp_value, &cp_err, 16);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_value);
            return;
        }
        res = l_i2c_set_f(u8_addr, u8_value);
        if(res) {
            fprintf(stderr, "Can not set I2C [%.2x] = %.2x (%d)\n", u8_addr, u8_value, res);
            return;
        }
    } else if(0 == strcasecmp(cp_cmd, "info")) {
        int b_all = 0;

        if(argc >=4 ) {
            const char* cp_info_type = argv[3];

            if(0 == strcasecmp(cp_info_type, "all")) {
                b_all = 1;
            } else if(0 == strcasecmp(cp_info_type, "avi")) {
                l_dump_info_avi_f();
            } else if(0 == strcasecmp(cp_info_type, "audio")) {
                l_dump_info_audio_f();
            } else if(0 == strcasecmp(cp_info_type, "ms")) {
                //l_dump_info_ms_f();
            } else if(0 == strcasecmp(cp_info_type, "timing")) {
                l_dump_info_timing_f();
            } else {
                fprintf(stderr, "Invalid info type %s\n", cp_info_type);
                return;
            }
        } else {
            b_all = 1;
        }
        if(b_all) {
            l_dump_info_timing_f();
            l_dump_info_avi_f();
            l_dump_info_audio_f();
            //l_dump_info_ms_f();
        }
    } else if(0 == strcasecmp(cp_cmd, "load")) {
        /* check arguments */
        if(argc < 5) {
            fprintf(stderr, "not enough arguments for load comamnd\n");
            return;
        }

        cp_area = argv[3];
        cp_file = argv[4];

        if(0 == strcasecmp(cp_area, "edid")) {
            u8_value = (uint8_t)strtoul(cp_file, &cp_err, 10);
            if(cp_err && *cp_err) {
                /* not a number use as filename */
                sp_file = fopen(cp_file, "rb");
                if(NULL == sp_file) {
                    perror("open");
                    fprintf(stderr, "Can not open %s\n", cp_file);
                    return;
                }
                if(1 != fread(au8_edid_data, L_U32_HDMICHIP_EDID_REGS_LEN, 1, sp_file)) {
                    perror("read edid");
                    return;
                }
                if(fclose(sp_file)) {
                    perror("close");
                    return;
                }
            } else {
                /* use the specific default edid table */
                if(u8_value < L_DEFAULT_EDID_TABLES_NUM) {
                    memcpy(au8_edid_data, au8p_default_edid_tables[u8_value], sizeof(au8_edid_data));
                } else {
                    fprintf(stderr, "default EDID table index %d is out of range %d\n", u8_value, L_DEFAULT_EDID_TABLES_NUM);
                    return;
                }
            }
            if(l_set_regs_edid_f(au8_edid_data)) {
                fprintf(stderr, "can not set EDID\n");
                return;
            }
        } else if(0 == strcasecmp(cp_area, "hdcp")) {
            /* parse HDCP key from file */
            if(l_parse_hdcp_key_file_f(&s_hdcp_key, cp_file)) {
                fprintf(stderr, "Can not parse HDCP key file\n");
                return;
            }
            /* load HDCP key */
            if(l_set_regs_hdcp_f(&s_hdcp_key)) {
                fprintf(stderr, "can not set HDCP key\n");
                return;
            }
        } else {
            fprintf(stderr, "Invalid area %s\n", cp_area);
            return;
        }
#if 0        
    } else if(0 == strcasecmp(cp_cmd, "save")) {
        /* check arguments */
        if(argc < 5) {
            fprintf(stderr, "not enough arguments for load comamnd\n");
            return;
        }

        cp_area = argv[3];
        cp_file = argv[4];
        if(0 == strcasecmp(cp_cmd, "edid")) {
            sp_file = fopen(cp_file, "wb");
            if(NULL == sp_file) {
                perror("open");
                fprintf(stderr, "Can not open %s\n", cp_file);
                return;
            }
            if(l_get_regs_edid_f(au8_edid_data)) {
                fprintf(stderr, "can not get EDID\n");
                return;
            }
            if(1 != fwrite(au8_edid_data, L_U32_HDMICHIP_EDID_REGS_LEN, 1, sp_file)) {
                perror("write edid");
                return;
            }
            if(fclose(sp_file)) {
                perror("close");
                return;
            }
        } else {
            fprintf(stderr, "Invalid area %s\n", cp_area);
            return;
        }
#endif        
    } else {
        fprintf(stderr, "Invalid hdmichip command %s\n", cp_cmd);
        return;
    }
}

static void l_pins_cmd_f(int argc, char* argv[])
{
    const char* cp_cmd = argv[2];
    int b_cifmode = 0;

    if(argc < 3) {
        fprintf(stderr, "not enough arguments for pins %s\n", cp_cmd);
        return;
    }

    if(0 == strcasecmp(cp_cmd, "cif")) {
        b_cifmode = 1;
    } else if(0 == strcasecmp(cp_cmd, "gpio")) {
        b_cifmode = 0;
    } else {
        fprintf(stderr, "Invalid pin command %s\n", cp_cmd);
        return;
    }

    if(l_configure_pins_f(b_cifmode)) {
        fprintf(stderr, "Can not configure pins\n");
        return;
    }
}

static void l_vin_cmd_f(int argc, char* argv[]) 
{
    const char* cp_id = NULL;
    const char* cp_width = NULL;
    const char* cp_height = NULL;
    const char* cp_start = NULL;
    char* cp_err;
    uint32_t u32_id;
    uint32_t u32_width;
    uint32_t u32_height;
    int b_start = 1;

    if(argc < 5) {
        fprintf(stderr, "not enough arguments for vin\n");
        return;
    }
    cp_id = argv[2];
    cp_width = argv[3];
    cp_height = argv[4];
    if(argc > 5) 
        cp_start = argv[5];
   
    u32_id = strtoul(cp_id, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_id);
        return;
    }
   

    u32_width = strtoul(cp_width, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_width);
        return;
    }

    u32_height = strtoul(cp_height, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_height);
        return;
    }

    if(cp_start) {
        b_start = atoi(cp_start) ? 1 : 0;
    }

    if(l_configure_vin_f(u32_id, u32_width, u32_height, b_start)) {
        fprintf(stderr, "Can not configure VIN%d to %dx%d (start=%d)\n", 
        u32_id, u32_width, u32_height, b_start);
        return;
    }
}

static void l_dma_cmd_f(int argc, char* argv[])
{
    const char* cp_cmd = NULL;
    const char* cp_id = NULL;
    const char* cp_address = NULL;
    const char* cp_width = NULL;
    const char* cp_height = NULL;
    const char* cp_bpl = NULL;
    const char* cp_mode = NULL;
    char* cp_err;
    int b_write;
    uint32_t u32_id;
    uint32_t u32_address;
    uint32_t u32_width;
    uint32_t u32_height;
    uint32_t u32_bpl;
    uint32_t u32_mode;

    if(argc < 7) {
        fprintf(stderr, "not enough arguments for rdma/wdma\n");
        return;
    }
    cp_cmd = argv[1];
    cp_id = argv[2];
    cp_address = argv[3];
    cp_width = argv[4];
    cp_height = argv[5];
    cp_bpl = argv[6];
    if(argc > 7) 
        cp_mode = argv[7];
   
    /* read or write command */
    if(0 == strcasecmp(cp_cmd, "wdma")) 
        b_write = 1;
    else
        b_write = 0;

    u32_id = strtoul(cp_id, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_id);
        return;
    }

    u32_address = strtoul(cp_address, &cp_err, 16);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_address);
        return;
    }

    u32_width = strtoul(cp_width, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_width);
        return;
    }

    u32_height = strtoul(cp_height, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_height);
        return;
    }

    u32_bpl = strtoul(cp_bpl, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_bpl);
        return;
    }

    if(cp_mode) {
        u32_mode = strtoul(cp_mode, &cp_err, 16);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_mode);
            return;
        }
    } else {
        u32_mode = b_write ? L_U32_DEFAULT_WDMACTRL : L_U32_DEFAULT_RDMACTRL;
    }
    
    if(l_configure_dma_f(b_write, u32_address, u32_id, u32_width, u32_height, 1, 1, u32_bpl, u32_mode)) {
        fprintf(stderr, "Can not configure %cDMA%d <%.8x> %dx%d\n", 
                b_write ? 'W' : 'R', u32_id, u32_address, u32_width, u32_height);
        return;
    }
}

static void l_wmix_cmd_f(int argc, char* argv[])
{
    const char* cp_id = NULL;
    const char* cp_ch = NULL;
    const char* cp_pos_x = NULL;
    const char* cp_pos_y = NULL;
    const char* cp_layer_prio = NULL;
    char* cp_err;
    uint32_t u32_id;
    uint32_t u32_ch;
    uint32_t u32_pos_x;
    uint32_t u32_pos_y;
    uint32_t u32_layer_prio = L_U32_DEFAULT_WMIX_MCTRL;

    if(argc < 6) {
        fprintf(stderr, "not enough arguments for wmix\n");
        return;
    }
    cp_id = argv[2];
    cp_ch = argv[3];
    cp_pos_x = argv[4];
    cp_pos_y = argv[5];
    if(argc > 6) 
        cp_layer_prio = argv[6];
   
    u32_id = strtoul(cp_id, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_id);
        return;
    }
   
    u32_ch = strtoul(cp_ch, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_ch);
        return;
    }
   
    u32_pos_x = strtoul(cp_pos_x, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_pos_x);
        return;
    }

    u32_pos_y = strtoul(cp_pos_y, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_pos_y);
        return;
    }

    if(cp_layer_prio) {
        u32_layer_prio = strtoul(cp_layer_prio, &cp_err, 0);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_layer_prio);
            return;
        }
    }

    if(l_configure_wmix_f(u32_id, u32_ch, u32_pos_x, u32_pos_y, u32_layer_prio)) {
        fprintf(stderr, "Can not configure WMIX%d for %d. ch to %d.%d (layer prio: %d)\n", 
        u32_id, u32_ch, u32_pos_x, u32_pos_y, u32_layer_prio);
        return;
    }
}

static void l_sc_cmd_f(int argc, char* argv[])
{
    const char* cp_id = NULL;
    const char* cp_rdma = NULL;
    const char* cp_width = NULL;
    const char* cp_height = NULL;
    char* cp_err;
    uint32_t u32_id;
    uint32_t u32_rdma;
    uint32_t u32_width;
    uint32_t u32_height;

    if(argc < 6) {
        fprintf(stderr, "not enough arguments for sc\n");
        return;
    }
    cp_id = argv[2];
    cp_rdma = argv[3];
    cp_width = argv[4];
    cp_height = argv[5];


    u32_id = strtoul(cp_id, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_id);
        return;
    }
   
    u32_rdma = strtoul(cp_rdma, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_rdma);
        return;
    }

    u32_width = strtoul(cp_width, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_width);
        return;
    }

    u32_height = strtoul(cp_height, &cp_err, 0);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_height);
        return;
    }

    if(l_configure_sc_f(u32_id, u32_rdma, u32_width, u32_height, u32_width, u32_height, 0, 0)) {
        fprintf(stderr, "Can not configure SC%d with RDMA%d to %dx%d\n", 
                u32_id, u32_rdma, u32_width, u32_height);
        return;
    }
}


static void l_auto_cmd_f(int argc, char* argv[])
{
    const char* cp_cmd = argv[1];

    if(0 == strcasecmp(cp_cmd, "start")) {
        const char* cp_width = NULL;
        const char* cp_height = NULL;
        const char* cp_pos_x = NULL;
        const char* cp_pos_y = NULL;
        const char* cp_scale = NULL;
        const char* cp_debug = NULL;
        char* cp_err;
        uint32_t u32_width;
        uint32_t u32_height;
        uint32_t u32_pos_x;
        uint32_t u32_pos_y;
        uint8_t u8_reg;
        l_s_timings_t s_curr_timing;
        uint32_t u32_curr_width;
        uint32_t u32_curr_height;
        uint32_t u32_curr_bpl;
        scale_e_mode_t e_scale_mode = SCALE_MODE_ALLFIT;     /* default scaleing mode */
        scale_s_calc_in_t s_scale_in;
        scale_s_calc_out_t s_scale_out;
        int res;
        uint32_t u32_scaler_sel;
        int b_debug = 0;

        if(argc < 6) {
            fprintf(stderr, "not enough arguments for start comamnd\n");
            return;
        }
        cp_width = argv[2];
        cp_height = argv[3];
        cp_pos_x = argv[4];
        cp_pos_y = argv[5];
        if(argc > 6) 
            cp_scale = argv[6];
        if(argc > 7)
            cp_debug = argv[7];

        u32_width = strtoul(cp_width, &cp_err, 0);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_width);
            return;
        }

        u32_height = strtoul(cp_height, &cp_err, 0);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_height);
            return;
        }

        u32_pos_x = strtoul(cp_pos_x, &cp_err, 0);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_pos_x);
            return;
        }

        u32_pos_y = strtoul(cp_pos_y, &cp_err, 0);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_pos_y);
            return;
        }

        if(cp_scale) {
            e_scale_mode = (scale_e_mode_t)strtoul(cp_scale, &cp_err, 10);
            if(cp_err && *cp_err) {
                /* not a number try string compare */
                if(0 == strcasecmp(cp_scale, "TOPLEFT")) {
                    e_scale_mode = SCALE_MODE_TOPLEFT;
                } else if(0 == strcasecmp(cp_scale, "CENTER")) {
                    e_scale_mode = SCALE_MODE_CENTER;
                } else if(0 == strcasecmp(cp_scale, "SCALE")) {
                    e_scale_mode = SCALE_MODE_SCALE;
                } else if(0 == strcasecmp(cp_scale, "CROPFIT")) {
                    e_scale_mode = SCALE_MODE_CROPFIT;
                } else if(0 == strcasecmp(cp_scale, "ALLFIT")) {
                    e_scale_mode = SCALE_MODE_ALLFIT;
                } else {
                    fprintf(stderr, "invalid scaling mode %s\n", cp_scale);
                    return;
                }
            }
            /* range check */
            if((e_scale_mode < SCALE_MODE_TOPLEFT) || (e_scale_mode > SCALE_MODE_ALLFIT)) {
                fprintf(stderr, "scaling mode %d is outp of [%d..%d] range\n", 
                        e_scale_mode, SCALE_MODE_TOPLEFT, SCALE_MODE_ALLFIT);
                return;
            }
        }

        /* check for extra debug argument */
        if(cp_debug) {
            if(0 == strcasecmp(cp_debug, L_ST_DEBUG)) {
                b_debug = 1;
            }
        }
        
        /* first stop */
        if(l_stop_f()) {
            fprintf(stderr, "stop returned error");
            return;
        }

        /* initialize pins */
        if(l_configure_pins_f(1)) {
            fprintf(stderr, "Can not configure CIF port pins\n");
            return;
        }
        /* configure hdmichip EQ - RXPHY */
        if(l_i2c_set_f(L_U8_HDMICHIP_RXPHY_REG_ADDR, L_U8_DEFAULT_HDMICHIP_RXPHY)) {
            fprintf(stderr, "Can not configure RX PHY hdmichip reg (%x) to %x\n", 
                    L_U8_HDMICHIP_RXPHY_REG_ADDR, L_U8_DEFAULT_HDMICHIP_RXPHY);
            return;
        }

        /* check lock signal */
        if(l_i2c_get_f(L_U8_HDMICHIP_STATUS1_REG_ADDR, &u8_reg)) {
            fprintf(stderr, "Can not read status1 hdmichip reg\n");
            return;
        }
        if((0 == (u8_reg & L_U8_HDMICHIP_LINK_ON)) || 
           (0 == (u8_reg & L_U8_HDMICHIP_DE_VALID))) {
            fprintf(stderr, "No valid HDMI signal (%x)!\n", u8_reg);
            return;
        }

        /* configure active HS/VS signals */
        if(l_i2c_get_f(L_U8_HDMICHIP_CONTROL1_REG_ADDR, &u8_reg)) {
            fprintf(stderr, "Can not read control1 hdmichip reg\n");
            return;
        }
        if(l_i2c_set_f(L_U8_HDMICHIP_CONTROL1_REG_ADDR, u8_reg)) {
            fprintf(stderr, "Can not read control1 hdmichip reg\n");
            return;
        }
        /* determine timing parameters */
        if(l_get_regs_timing_f(&s_curr_timing)) {
            fprintf(stderr, "Can not read timing registers from hdmichip\n");
            return;
        }
        u32_curr_width = s_curr_timing.u32_appl;
        u32_curr_height = s_curr_timing.u32_alpf;
        u32_curr_bpl = u32_curr_width * L_U32_AUTO_BYTES_PER_PIXEL;

        /* check input resolution */
#ifdef DISABLE_1080P_HDMI_INPUT
        /* check is bypassed in debug mode */
        if(!b_debug) {
            if((L_U32_1080P_HEIGHT == u32_curr_height) &&
               (L_U32_1080P_WIDTH == u32_curr_width)) {
                fprintf(stderr, "1080p input is disabled\n");
                return;
            }
        }
#endif

        /* calculate scaling parameters */
        s_scale_in.e_mode = e_scale_mode;
        s_scale_in.s_in.u32_x = u32_curr_width;
        s_scale_in.s_in.u32_y = u32_curr_height;
        s_scale_in.s_out.u32_x = u32_width;
        s_scale_in.s_out.u32_y = u32_height;
        s_scale_in.s_win.u32_x = u32_pos_x;
        s_scale_in.s_win.u32_y = u32_pos_y;
        s_scale_in.u32_bytes_per_pixel = L_U32_AUTO_BYTES_PER_PIXEL;
        res = scale_calc_f(&s_scale_in, &s_scale_out);
        if(res) {
            fprintf(stderr, "Can not calculate scaling parameters (%d)\n", res);
            return;
        }

        /* initialize vin - start video in */
        if(l_configure_vin_f(L_U32_AUTO_VIN_ID, 
                             u32_curr_width, u32_curr_height, 1)) {
            fprintf(stderr, "Can not configure VIN\n");
            return;
        }

        /* initialize scaler first */
        if(l_init_sc_f(&s_scale_in)) {
            fprintf(stderr, "Can not initialize scaler\n");
            return ;
        }

        if(0 == l_u32_dmabuf_addr) {
            l_u32_dmabuf_addr = vioctrl_get_memarea_f(L_ST_PMAP_EXTCAM, L_U32_MIN_MEMSIZE);
            if(0 == l_u32_dmabuf_addr) {
                fprintf(stderr, "Can not get DMA buffer\n");
                return ;
            }
        }

        /* initialize rdma - enable layer */
        if(l_configure_dma_f(0, l_u32_dmabuf_addr + s_scale_out.u32_dma_offs,
                             L_U32_AUTO_RDMA_ID,
                             s_scale_out.s_dma_out.u32_x, 
                             s_scale_out.s_dma_out.u32_y, 
                             //u32_curr_width, u32_curr_height,
                             //L_U32_AUTO_MAX_WIDTH, L_U32_AUTO_MAX_HEIGHT,
                             //u32_curr_bpl,
                             s_scale_out.s_dma_upscale.u32_x,
                             s_scale_out.s_dma_upscale.u32_y,
                             s_scale_out.u32_dma_bpl,
                             L_U32_DEFAULT_RDMACTRL)) {
            fprintf(stderr, "Can not configure RDMA\n");
            return;
        }

        /* initialize wdma */
        if(l_configure_dma_f(1, l_u32_dmabuf_addr,
                             L_U32_AUTO_WDMA_ID,
                             u32_curr_width, u32_curr_height,
                             //L_U32_AUTO_MAX_WIDTH, L_U32_AUTO_MAX_HEIGHT,
                             0, 0, u32_curr_bpl,
                             L_U32_DEFAULT_WDMACTRL)) {
            fprintf(stderr, "Can not configure WDMA\n");
            return;
        }

        if(s_scale_out.b_write_scale) {
            u32_scaler_sel = l_get_scaler_sel_wdma_f(L_U32_AUTO_WDMA_ID);
        } else {
            u32_scaler_sel = l_get_scaler_sel_rdma_f(L_U32_AUTO_RDMA_ID);
        }

        if(s_scale_out.b_isscaled) {
            /* initialize scaler */
            if(l_configure_sc_f(L_U32_AUTO_SC_ID,
                        u32_scaler_sel,
                        s_scale_out.s_scale.u32_x,
                        s_scale_out.s_scale.u32_y,
                        s_scale_out.s_scale_out.u32_x,
                        s_scale_out.s_scale_out.u32_y,
                        s_scale_out.s_scale_pos.u32_x,
                        s_scale_out.s_scale_pos.u32_y)) {
                fprintf(stderr, "Can not configure Scaler\n");
                return;
            }
        } else {
            /* disable scaler */
            if(l_stop_sc_f(L_U32_AUTO_SC_ID)) {
                fprintf(stderr, "Can not Stop Scaler\n");
                return;
            }
        }

        /* intialize wmix - switch layer */
        if(l_configure_wmix_f(L_U32_AUTO_WMIX_ID, L_U32_AUTO_WMIX_CH, 
                              //u32_pos_x, u32_pos_y,
                              s_scale_out.s_win.u32_x,
                              s_scale_out.s_win.u32_y,
                              L_U32_DEFAULT_WMIX_MCTRL)) {
            fprintf(stderr, "Can not configure WMIX\n");
            return;
        }

    } else if(0 == strcasecmp(cp_cmd, "stop")) {
        if(l_stop_f()) {
            fprintf(stderr, "Can not stop HDMI input components\n");
            return;
        }
    } else {
        fprintf(stderr, "Invalid auto command %s\n", cp_cmd);
        return;
    }
}

static void l_load_save_cmd_f(int argc, char* argv[])
{
    const char* cp_cmd = NULL;
    const char* cp_addr = NULL;
    const char* cp_file = NULL;
    const char* cp_len = NULL;
    uint32_t u32_addr;
    uint32_t u32_len;
    char* cp_err;

    /* check arguments */
    if(argc < 4) {
        fprintf(stderr, "not enough arguments for %s comamnd\n", argv[1] ? argv[1] : "null");
        return;
    }

    cp_cmd = argv[1];
    cp_addr = argv[2];
    cp_file = argv[3];
    if(argc > 4) 
        cp_len = argv[4];

    /* convert address */
    u32_addr = (uint32_t)strtoul(cp_addr, &cp_err, 16);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_addr);
        return;
    }
    /* convert length */
    if(cp_len) {
        u32_len = (uint32_t)strtoul(cp_len, &cp_err, 0);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a number\n", cp_len);
            return;
        }
    }

    /* check command */
    if(0 == strcasecmp(cp_cmd, "load")) {
        if(vioctrl_load_file_f(cp_file, u32_addr)) {
            fprintf(stderr, "Can not load %s to memory (0x%.8x)\n", cp_file, u32_addr);
            return;
        }
    } else if(0 == strcasecmp(cp_cmd, "save")) {
        if(NULL == cp_len) {
            fprintf(stderr, "memory length argument is missing for save command\n");
            return;
        }
        if(vioctrl_save_file_f(cp_file, u32_addr, u32_len)) {
            fprintf(stderr, "Can not save memory (0x%.8x + 0x%x) to %s\n", u32_addr, u32_len, cp_file);
            return;
        }
    } else {
        fprintf(stderr, "Invalid command %s\n", cp_cmd);
        return;
    }
}

static void l_scroll_cmd_f(int argc, char* argv[])
{
    const char* cp_cmd = NULL;
    const char* cp_file = NULL;
    const char* cp_text_w = NULL;
    const char* cp_text_h = NULL;
    const char* cp_win_x = NULL;
    const char* cp_win_y = NULL;
    const char* cp_win_w = NULL;
    const char* cp_win_h = NULL;
    const char* cp_sleep_ms = NULL;
    uint32_t u32_text_w;
    uint32_t u32_text_h;
    uint32_t u32_win_x;
    uint32_t u32_win_y;
    uint32_t u32_win_w;
    uint32_t u32_win_h;
    uint32_t u32_sleep_ms;
    char* cp_err;
    uint32_t u32_bpl;

    /* check arguments */
    if(argc < 10) {
        fprintf(stderr, "not enough arguments for scroll comamnd\n");
        return;
    }

    cp_cmd = argv[1];
    cp_sleep_ms = argv[2];
    cp_file = argv[3];
    cp_text_w = argv[4];
    cp_text_h = argv[5];
    cp_win_x = argv[6];
    cp_win_y = argv[7];
    cp_win_w = argv[8];
    cp_win_h = argv[9];

    /* parse numbers */
    u32_text_w = (uint32_t)strtoul(cp_text_w, &cp_err, 10);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_text_w);
        return;
    }
    u32_text_h = (uint32_t)strtoul(cp_text_h, &cp_err, 10);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_text_h);
        return;
    }
    u32_win_x = (uint32_t)strtoul(cp_win_x, &cp_err, 10);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_win_x);
        return;
    }
    u32_win_y = (uint32_t)strtoul(cp_win_y, &cp_err, 10);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_win_y);
        return;
    }
    u32_win_w = (uint32_t)strtoul(cp_win_w, &cp_err, 10);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_win_w);
        return;
    }
    u32_win_h = (uint32_t)strtoul(cp_win_h, &cp_err, 10);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_win_h);
        return;
    }
    u32_sleep_ms = (uint32_t)strtoul(cp_sleep_ms, &cp_err, 10);
    if(cp_err && *cp_err) {
        fprintf(stderr, "%s is not a number\n", cp_sleep_ms);
        return;
    }

    /* check comamnd */
    if(0 != strcasecmp(cp_cmd, "scroll")) {
        fprintf(stderr, "Scroll command is expected\n");
        return;
    }

    /* calculate bytes per line */
    u32_bpl = u32_text_w * L_U32_SCROLL_BYTES_PER_PIXEL;

    /* load file to address */
    if(vioctrl_load_file_f(cp_file, L_U32_AUTO_DMABUF_ADDR)) {
        fprintf(stderr, "can not load %s to 0x%.8x\n", cp_file, L_U32_AUTO_DMABUF_ADDR);
        return;
    }

    /* initialize RDMA */
    if(l_configure_dma_f(0, L_U32_AUTO_DMABUF_ADDR, L_U32_AUTO_RDMA_ID, 
                            u32_win_w, u32_win_h, 1, 1, u32_bpl,
                            L_U32_SCROLL_RDMACTRL)) {
        fprintf(stderr, "Can not configure RDMA\n");
        return;
    }                             


    /* initialize WMIX */
    if(l_configure_wmix_f(L_U32_AUTO_WMIX_ID, L_U32_AUTO_WMIX_CH, 
                            u32_win_x, u32_win_y,
                            L_U32_DEFAULT_WMIX_MCTRL)) {
        fprintf(stderr, "Can not configure WMIX\n");
        return;
    }

    /* start scrolling */
    if(l_scroll_loop_f(L_U32_AUTO_RDMA_ID, L_U32_AUTO_DMABUF_ADDR, L_U32_SCROLL_RDMACTRL, u32_sleep_ms,
                       u32_text_w, u32_text_h, u32_win_w, u32_win_h)) {
        fprintf(stderr, "Can not start scrolling\n");
        return;
    }

}

static int l_i2c_init_f(void)
{
    int fd_i2c;
    int addr;
    int ret;

    /* open I2C device */
    fd_i2c = open(L_ST_I2C_DEV, O_RDWR);
    if(fd_i2c < 0) {
        perror("open " L_ST_I2C_DEV);
        return -1;
    }
    
    /* configure I2C slave address for HDMI input chip */
    addr = L_U8_HDMICHIP_I2C_ADDR;
    ret = ioctl(fd_i2c, I2C_SLAVE, addr);
    if(ret < 0) {
        perror("ioctl I2C SLAVE");
        (void)close(fd_i2c);
        return -1;
    }

    /* return opened file descriptor */
    return fd_i2c;
}

static int l_i2c_get_f(uint8_t p_u8_addr, uint8_t* p_u8p_value)
{
    int fd_i2c;
    struct i2c_smbus_ioctl_data s_ioctl_arg;
    union i2c_smbus_data data;
    int res;

    /* check argument */
    if(NULL == p_u8p_value) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* initialize I2C device (open / set slave address */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C\n");
        return -1;
    }

    s_ioctl_arg.read_write = I2C_SMBUS_READ;
    s_ioctl_arg.command = p_u8_addr;
    s_ioctl_arg.size = I2C_SMBUS_BYTE_DATA;
    s_ioctl_arg.data = &data;

    res = ioctl(fd_i2c, I2C_SMBUS, &s_ioctl_arg);
    if(res) {
        perror("I2C ioctl get");
        (void)close(fd_i2c);
        return -1;
    }

    *p_u8p_value = (uint8_t)(data.byte & 0xFF);

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return 0;
}

static int l_i2c_set_f(uint8_t p_u8_addr, uint8_t p_u8_value)
{
    int fd_i2c;
    struct i2c_smbus_ioctl_data s_ioctl_arg;
    union i2c_smbus_data data;
    int res;

    /* initialize I2C device (open / set slave address */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C (%d)\n", fd_i2c);
        return -1;
    }

    data.byte = p_u8_value;

    s_ioctl_arg.read_write = I2C_SMBUS_WRITE;
    s_ioctl_arg.command = p_u8_addr;
    s_ioctl_arg.size = I2C_SMBUS_BYTE_DATA;
    s_ioctl_arg.data = &data;

    res = ioctl(fd_i2c, I2C_SMBUS, &s_ioctl_arg);
    if(res) {
        perror("I2C ioctl set");
        (void)close(fd_i2c);
        return -1;
    }

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return 0;
}

static int l_get_regs_timing_f(l_s_timings_t* p_sp_timings) 
{
    int fd_i2c;
    struct i2c_smbus_ioctl_data s_ioctl_arg;
    union i2c_smbus_data data;
    uint8_t au8_regs[L_U8_HDMICHIP_TIMING_REGS_LEN];
    int res;
    uint32_t u32_index;
    
    /* check arguments */
    if(NULL == p_sp_timings) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* open I2C device */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C\n");
        return -1;
    }

    data.block[0] = L_U8_HDMICHIP_TIMING_REGS_LEN;
    
    s_ioctl_arg.read_write = I2C_SMBUS_READ;
    s_ioctl_arg.command = L_U8_HDMICHIP_TIMING_REGS_ADDR;
    s_ioctl_arg.size = I2C_SMBUS_I2C_BLOCK_DATA;
    s_ioctl_arg.data = &data;

    res = ioctl(fd_i2c, I2C_SMBUS, &s_ioctl_arg);
    if(res) {
        perror("I2C ioctl block read");
        (void)close(fd_i2c);
        return -1;
    }

    /* arange registers */
    for(u32_index = 0; u32_index < L_U8_HDMICHIP_TIMING_REGS_LEN; ++u32_index) 
        au8_regs[u32_index] = data.block[u32_index + 1];

    /* parse register values */
    memset(p_sp_timings, 0, sizeof(l_s_timings_t));
    /* Active Pixels Per Line */
    p_sp_timings->u32_appl = au8_regs[0];
    p_sp_timings->u32_appl |= (((uint32_t)(au8_regs[1])) << 8);
    /* Horizontal Front Porch */
    p_sp_timings->u32_hfp = au8_regs[2];
    p_sp_timings->u32_hfp |= (((uint32_t)(au8_regs[3])) << 8);
    /* Horizontal Back Porch */
    p_sp_timings->u32_hbp = au8_regs[4];
    p_sp_timings->u32_hbp |= (((uint32_t)(au8_regs[5])) << 8);
    /* Horizontal Pulse Width */
    p_sp_timings->u32_hpw = au8_regs[6];
    p_sp_timings->u32_hpw |= (((uint32_t)(au8_regs[7])) << 8);
    /* Active Lines Per Frame */
    p_sp_timings->u32_alpf = au8_regs[8];
    p_sp_timings->u32_alpf |= (((uint32_t)(au8_regs[9])) << 8);
    /* Vertical Front Porch */
    p_sp_timings->u32_vfp = au8_regs[10];
    /* Vertical Back Porch */
    p_sp_timings->u32_vbp = au8_regs[11];
    /* Vertical Pulse Width */
    p_sp_timings->u32_vbp = au8_regs[12] & 0xEF;
    /* Interlaced */
    p_sp_timings->b_intl = (au8_regs[12] >> 7) & 0x01;

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return 0;
}

static int l_get_regs_avi_f(l_s_avi_info_t* p_sp_avi_info)
{
    int fd_i2c;
    struct i2c_smbus_ioctl_data s_ioctl_arg;
    union i2c_smbus_data data;
    uint8_t au8_regs[L_U8_HDMICHIP_AVIINFO_REGS_LEN];
    int res;
    uint32_t u32_index;
    
    /* check arguments */
    if(NULL == p_sp_avi_info) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* open I2C device */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C\n");
        return -1;
    }

    data.block[0] = L_U8_HDMICHIP_AVIINFO_REGS_LEN;
    
    s_ioctl_arg.read_write = I2C_SMBUS_READ;
    s_ioctl_arg.command = L_U8_HDMICHIP_AVIINFO_REGS_ADDR;
    s_ioctl_arg.size = I2C_SMBUS_I2C_BLOCK_DATA;
    s_ioctl_arg.data = &data;

    res = ioctl(fd_i2c, I2C_SMBUS, &s_ioctl_arg);
    if(res) {
        perror("I2C ioctl block read");
        (void)close(fd_i2c);
        return -1;
    }

    /* arange registers */
    for(u32_index = 0; u32_index < L_U8_HDMICHIP_AVIINFO_REGS_LEN; ++u32_index) 
        au8_regs[u32_index] = data.block[u32_index + 1];

    /* parse register values */
    memset(p_sp_avi_info, 0, sizeof(l_s_avi_info_t));
    memcpy(p_sp_avi_info->au8_data, au8_regs, L_U8_HDMICHIP_AVIINFO_REGS_LEN);

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return 0;
}

static int l_get_regs_ado_f(l_s_ado_info_t* p_sp_ado_info)
{
    int fd_i2c;
    struct i2c_smbus_ioctl_data s_ioctl_arg;
    union i2c_smbus_data data;
    uint8_t au8_regs[L_U8_HDMICHIP_ADOINFO_REGS_LEN];
    int res;
    uint32_t u32_index;
    
    /* check arguments */
    if(NULL == p_sp_ado_info) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* open I2C device */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C\n");
        return -1;
    }

    data.block[0] = L_U8_HDMICHIP_ADOINFO_REGS_LEN;
    
    s_ioctl_arg.read_write = I2C_SMBUS_READ;
    s_ioctl_arg.command = L_U8_HDMICHIP_ADOINFO_REGS_ADDR;
    s_ioctl_arg.size = I2C_SMBUS_I2C_BLOCK_DATA;
    s_ioctl_arg.data = &data;

    res = ioctl(fd_i2c, I2C_SMBUS, &s_ioctl_arg);
    if(res) {
        perror("I2C ioctl block read");
        (void)close(fd_i2c);
        return -1;
    }

    /* arange registers */
    for(u32_index = 0; u32_index < L_U8_HDMICHIP_ADOINFO_REGS_LEN; ++u32_index) 
        au8_regs[u32_index] = data.block[u32_index + 1];

    /* parse register values */
    memset(p_sp_ado_info, 0, sizeof(l_s_ado_info_t));
    memcpy(p_sp_ado_info->au8_data, au8_regs, L_U8_HDMICHIP_ADOINFO_REGS_LEN);

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return 0;
}

#if 0
static int l_get_regs_edid_f(uint8_t* p_au8_buf)
{
    int fd_i2c;
    struct i2c_smbus_ioctl_data s_ioctl_arg;
    union i2c_smbus_data data;
    int res;
    uint32_t u32_index;
    
    /* check arguments */
    if(NULL == p_au8_buf) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* open I2C device */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C\n");
        return -1;
    }

    data.block[0] = L_U32_HDMICHIP_EDID_REGS_LEN - 1;
    
    s_ioctl_arg.read_write = I2C_SMBUS_READ;
    s_ioctl_arg.command = L_U8_HDMICHIP_EDID_REGS_ADDR;
    s_ioctl_arg.size = I2C_SMBUS_I2C_BLOCK_DATA;
    s_ioctl_arg.data = &data;

    res = ioctl(fd_i2c, I2C_SMBUS, &s_ioctl_arg);
    if(res) {
        perror("I2C ioctl block read");
        (void)close(fd_i2c);
        return -1;
    }

    /* arange registers */
    for(u32_index = 0; u32_index < L_U32_HDMICHIP_EDID_REGS_LEN; ++u32_index) 
        p_au8_buf[u32_index] = data.block[u32_index + 1];

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return 0;
}
#endif

static int l_set_regs_edid_f(const uint8_t* p_au8_buf)
{
    int fd_i2c;
    int res;
    uint8_t au8_data[L_U32_HDMICHIP_EDID_REGS_LEN+1];
    
    /* check arguments */
    if(NULL == p_au8_buf) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* open I2C device */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C\n");
        return -1;
    }

    au8_data[0] = L_U8_HDMICHIP_EDID_REGS_ADDR;
    memcpy(&au8_data[1], p_au8_buf, L_U32_HDMICHIP_EDID_REGS_LEN);

    res = write(fd_i2c, au8_data, sizeof(au8_data));
    if(res != sizeof(au8_data)) {
        if(res < 0) {
            perror("i2c write, edid");
        } else {
            fprintf(stderr, "i2c EDID write returns %d instead of %d\n", res, sizeof(au8_data));
        }
    } 

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return 0;
}

/** Internal function to load HDCP key into HDMI input chip 
 *
 *  @param  p_sp_hdcp_key   HDCP keys and KSV to be loaded
 *  @return zero in case of success
 *
 *  The function loads the 40 56-bit HDCP keys to $00..$27 registers,
 *  For each HDCP key, bit 7~0 is stored in Byte-0 
 *  and bit 55~48 is stored in Byte-6.
 *  The 40 bit BKSV is stored in $28 register, 
 *  bit 7~0 is stored in Byte 0 and bit 39~32 is stored in Byte-4.
 */
static int l_set_regs_hdcp_f(const l_s_hdcp_key_t* p_sp_hdcp_key)
{
    int fd_i2c;
    int res;
    uint32_t u32_index;
    uint8_t au8_data[L_HDCP_KEY_LEN+1];
    int return_value = 0;

    /* open I2C device */
    fd_i2c = l_i2c_init_f();
    if(fd_i2c < 0) {
        fprintf(stderr, "Can not initialize I2C\n");
        return -1;
    }

    /* store keys */
    for(u32_index = 0; u32_index < L_HDCP_KEY_NUM; ++u32_index) {
        au8_data[0] = L_U8_HDMICHIP_HDCP_KEYS_BASE_ADDR + u32_index;
        memcpy(&au8_data[1], p_sp_hdcp_key->au8_keys[u32_index], L_HDCP_KEY_LEN);
        res = write(fd_i2c, au8_data, sizeof(au8_data));
        if(res != sizeof(au8_data)) {
            if(res < 0) {
                perror("i2c write, edid");
            } else {
                fprintf(stderr, "i2c HDCP write returns %d instead of %d\n", res, sizeof(au8_data));
            }
            return_value = -1;
        } 
    }

    /* store KSV */
    au8_data[0] = L_U8_HDMICHIP_HDCP_KSV_ADDR;
    memcpy(&au8_data[1], p_sp_hdcp_key->au8_ksv, L_HDCP_KSV_LEN);
    res = write(fd_i2c, au8_data, L_HDCP_KSV_LEN+1);
    if(res != L_HDCP_KSV_LEN+1) {
        if(res < 0) {
            perror("i2c write, edid");
        } else {
            fprintf(stderr, "i2c HDCP write returns %d instead of %d\n", res, L_HDCP_KSV_LEN+1);
        }
        return_value = -1;
    } 

    /* close i2c device */
    if(close(fd_i2c)) {
        perror("I2C close");
        return -1;
    }

    return return_value;
}

static void l_dump_info_timing_f(void)
{
    l_s_timings_t s_timings;

    if(l_get_regs_timing_f(&s_timings)) {
        fprintf(stderr, "Can not read timing registers\n");
        return;
    }

    printf("Timing Registers:\n");
    printf("APPL - Active Pixel Per Line   (Width): %u\n", s_timings.u32_appl);
    printf("ALFP - Active Lines Per Frame (Height): %u\n", s_timings.u32_alpf);
    printf("HFP - Horizontal Front Porch: %u\n", s_timings.u32_hfp);
    printf("HBP - Horizontal Back Porch: %u\n", s_timings.u32_hbp);
    printf("HPW - Horizontal Pulse Width: %u\n", s_timings.u32_hpw);
    printf("VFP - Vertical Front Porch: %u\n", s_timings.u32_vfp);
    printf("VBP - Vertical Back Porch: %u\n", s_timings.u32_vbp);
    printf("VPW - Vertical Pulse Width: %u\n", s_timings.u32_vpw);
    printf("INTL - %d (%s)\n", s_timings.b_intl, s_timings.b_intl ? "INTERLACED" : "PROGRESSIVE");
}

static void l_dump_info_avi_f(void) 
{
    l_s_avi_info_t s_avi_info;
    uint32_t u32_index;

    if(l_get_regs_avi_f(&s_avi_info)) {
        fprintf(stderr, "Can not read AVI info registers\n");
        return;
    }
    printf("AVI Info Frame\n");
    for(u32_index = 0; u32_index < L_U8_HDMICHIP_AVIINFO_REGS_LEN; ++u32_index) {
        printf("[%.2d]: %.2x\n", u32_index, s_avi_info.au8_data[u32_index]);
    }
}

static void l_dump_info_audio_f(void) 
{
    l_s_ado_info_t s_ado_info;
    uint32_t u32_index;

    if(l_get_regs_ado_f(&s_ado_info)) {
        fprintf(stderr, "Can not read ADO (audio) info registers\n");
        return;
    }
    printf("ADO Info Frame (audio)\n");
    for(u32_index = 0; u32_index < L_U8_HDMICHIP_ADOINFO_REGS_LEN; ++u32_index) {
        printf("[%.2d]: %.2x\n", u32_index, s_ado_info.au8_data[u32_index]);
    }
}

static void* l_mem_init_f(int* p_fdp_mem, uint32_t p_u32_base, uint32_t* p_u32p_size)
{
    size_t size;
    size_t page_size;
    void* vp_ptr;
    int fd_mem;
    
    /* check arguments */
    if((NULL == p_fdp_mem) || (NULL == p_u32p_size)) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return NULL;
    }

    /* open device */
    fd_mem = open(L_ST_DEV_MEM, O_RDWR);
    if(fd_mem < 0) {
        perror("open " L_ST_DEV_MEM);
        return NULL;
    }
    *p_fdp_mem = fd_mem;
    /* round length to page size */
    size = (size_t)*p_u32p_size;
    page_size = getpagesize();
    if(0 != (size % page_size)) {
        size = ((int)(size / page_size) + 1) * page_size;
    }
    *p_u32p_size = (uint32_t)size;

    /* map registers */
    vp_ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, p_u32_base);
    if(MAP_FAILED == vp_ptr) {
        fprintf(stderr, "Can not map 0x%.8x + 0x%x\n", p_u32_base, size);
        (void)close(fd_mem);
        return NULL;
    }

    return vp_ptr;
}

static int l_mem_close_f(int p_fd_mem, void* p_u32p_base, uint32_t p_u32_size)
{
    if(munmap(p_u32p_base, p_u32_size)) {
        perror("unmap");
        close(p_fd_mem);
        return -1;
    }

    if(close(p_fd_mem)) {
        perror("close");
        return -1;
    }

    return 0;
}

static int l_configure_pins_f(int p_b_mode)
{
    volatile void* vp_map;
    uint32_t u32_size;
    int fd_mem;

    u32_size = L_U32_GPIO_MEM_SIZE;
    vp_map = l_mem_init_f(&fd_mem, L_U32_GPIO_MEM_BASE, &u32_size);
    if(NULL == vp_map) {
        fprintf(stderr, "Can not map GPIO periphery memory\n");
        return 0;
    }

    /* initialize Output directions */

    /* PortB - CIF -> Input */
    VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBEN) = 0;

    /* E18 - A_MUTE -> Input, E22 - Rx_INTB -> Input*/
    VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPEEN) &= ~((1 << 18) | (1 << 22));

    /* G17 - SPDIF -> Input */
    VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPGEN) &= ~(1 << 17);

    if(p_b_mode) {
        /* initialize Port B as CIF */
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN0) = L_U32_GPIO_MODE5;
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN1) = L_U32_GPIO_MODE5;
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN2) = L_U32_GPIO_MODE5;
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN3) = L_U32_GPIO_MODE5;
    } else {
        /* initialize Post B as GPIO */
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN0) = L_U32_GPIO_MODE0;
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN1) = L_U32_GPIO_MODE0;
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN2) = L_U32_GPIO_MODE0;
        VIOCTRL_REG_MACRO(vp_map, L_U32_GPIO_GPBFN3) = L_U32_GPIO_MODE0;
    }
    
    if(l_mem_close_f(fd_mem, (void*)vp_map, u32_size)) {
        fprintf(stderr, "Can not close GPIO mem map\n");
        return -1;
    }

    return 0;
}

static int l_configure_vin_f(uint32_t p_u32_id, uint32_t p_u32_width, 
                             uint32_t p_u32_height, int p_b_start)
{
    volatile void* vp_map;
    uint32_t u32_size;
    int fd_mem;
    uint32_t u32_comp_base;

    /* determine component base address */
    switch(p_u32_id) {
    case 0:
        u32_comp_base = L_U32_VIOC_BASE_VIN0;
        break;
    case 1:
        u32_comp_base = L_U32_VIOC_BASE_VIN1;
        break;
    case 2:
        u32_comp_base = L_U32_VIOC_BASE_VIN2;
        break;
    case 3:
        u32_comp_base = L_U32_VIOC_BASE_VIN3;
        break;
    default:
        fprintf(stderr, "Invalid VIN id %d\n", p_u32_id);
        return -1;
    }

    u32_size = L_U32_VIOC_MEM_SIZE;
    vp_map = l_mem_init_f(&fd_mem, L_U32_VIOC_MEM_BASE, &u32_size);
    if(NULL == vp_map) {
        fprintf(stderr, "Can not map VIOC periphery memory\n");
        return 0;
    }

    /* set VIN_SIZE to the specified resolution */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + L_U32_VIOC_VIN_SIZE) = VIOCTRL_U32_RESOLUTION(p_u32_width, p_u32_height);

    /* set VIN_CTRL */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + L_U32_VIOC_VIN_CTRL) = L_U32_DEFAULT_VIN_CTRL | (p_b_start & 0x1);

    if(l_mem_close_f(fd_mem, (void*)vp_map, u32_size)) {
        fprintf(stderr, "Can not close VIOC mem map\n");
        return -1;
    }
    return 0;
}

static int l_configure_dma_f(int p_b_write, uint32_t p_u32_address, 
                             uint32_t p_u32_id, uint32_t p_u32_width, 
                             uint32_t p_u32_height, uint32_t p_u32_upscale_x,
                             uint32_t p_u32_upscale_y, uint32_t p_u32_bpl,
                             uint32_t p_u32_mode)
{
    volatile void* vp_map;
    uint32_t u32_size;
    int fd_mem;
    uint32_t u32_comp_base;
    uint32_t u32_ctrl_offs;
    uint32_t u32_size_offs;
    uint32_t u32_base0_offs;
    uint32_t u32_offs_offs;
    uint32_t u32_rdma_upscale = 0u;
    int res;

    /* determine component base address */
    if(p_b_write) {
        u32_comp_base = vioctrl_get_wdma_base_f(p_u32_id);
        u32_ctrl_offs = VIOCTRL_U32_VIOC_WDMACTRL;
        u32_size_offs = VIOCTRL_U32_VIOC_WDMASIZE;
        u32_base0_offs = VIOCTRL_U32_VIOC_WDMABASE0;
        u32_offs_offs = VIOCTRL_U32_VIOC_WDMAOFFS;
    } else {
        u32_comp_base = vioctrl_get_rdma_base_f(p_u32_id);
        if(0 == u32_comp_base) {
        }
        u32_ctrl_offs = VIOCTRL_U32_VIOC_RDMACTRL;
        u32_size_offs = VIOCTRL_U32_VIOC_RDMASIZE;
        u32_base0_offs = VIOCTRL_U32_VIOC_RDMABASE0;
        u32_offs_offs = VIOCTRL_U32_VIOC_RDMAOFFS;
    }

    /* calulate upscale value */
    if(!p_b_write) {
        res = l_get_rdma_upscale_f(p_u32_upscale_y);
        if(res < 0) {
            fprintf(stderr, "invalid upscale value %d\n", p_u32_upscale_y);
            return -1;
        }
        u32_rdma_upscale = (res & 0x7) << 16;
        res = l_get_rdma_upscale_f(p_u32_upscale_x);
        if(res < 0) {
            fprintf(stderr, "invalid upscale value %d\n", p_u32_upscale_x);
            return -1;
        }
        u32_rdma_upscale &= 0xFFFF0000;
        u32_rdma_upscale |= res & 0x7;
    }

    u32_size = L_U32_VIOC_MEM_SIZE;
    vp_map = l_mem_init_f(&fd_mem, L_U32_VIOC_MEM_BASE, &u32_size);
    if(NULL == vp_map) {
        fprintf(stderr, "Can not map VIOC periphery memory\n");
        return 0;
    }

    /* set BASE0 register : address */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + u32_base0_offs) = p_u32_address;

    /* set SIZE register : resolution */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + u32_size_offs) = VIOCTRL_U32_RESOLUTION(p_u32_width, p_u32_height);

    /* set OFFS regsiter: bytes per line */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + u32_offs_offs) = p_u32_bpl;

    /* set CTRL register: mode / control */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + u32_ctrl_offs) = p_u32_mode;

    if(!p_b_write) {
        /* set RDMASCALE register */
        VIOCTRL_REG_MACRO(vp_map, u32_comp_base + VIOCTRL_U32_VIOC_RDMASCALE) = u32_rdma_upscale;
    }
    
    if(l_mem_close_f(fd_mem, (void*)vp_map, u32_size)) {
        fprintf(stderr, "Can not close VIOC mem map\n");
        return -1;
    }
    return 0;
}

static int l_configure_wmix_f(uint32_t p_u32_id, uint32_t p_u32_ch, 
                             uint32_t p_u32_pos_x, uint32_t p_u32_pos_y, 
                             uint32_t p_u32_layer_prio)
{
    volatile void* vp_map;
    uint32_t u32_size;
    int fd_mem;
    uint32_t u32_comp_base;
    uint32_t u32_mpos_offs;

    /* determine component base address */
    u32_comp_base = vioctrl_get_wmix_base_f(p_u32_id);
    if(0 == u32_comp_base) {
        fprintf(stderr, "Invalid WMIX id %d\n", p_u32_id);
        return -1;
    }

    /* determine mpos register offset from channel */
    switch(p_u32_ch) {
    case 0:
        u32_mpos_offs = VIOCTRL_U32_VIOC_WMIX_MPOS0;
        break;
    case 1:
        u32_mpos_offs = VIOCTRL_U32_VIOC_WMIX_MPOS1;
        break;
    case 2:
        u32_mpos_offs = VIOCTRL_U32_VIOC_WMIX_MPOS2;
        break;
    case 3:
        u32_mpos_offs = VIOCTRL_U32_VIOC_WMIX_MPOS3;
        break;
    default:
        fprintf(stderr, "Invalid WMIX channel %d\n", p_u32_ch);
        return -1;
    }

    u32_size = L_U32_VIOC_MEM_SIZE;
    vp_map = l_mem_init_f(&fd_mem, L_U32_VIOC_MEM_BASE, &u32_size);
    if(NULL == vp_map) {
        fprintf(stderr, "Can not map VIOC periphery memory\n");
        return 0;
    }

    /* set MPOS register to the specified position */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + u32_mpos_offs) = VIOCTRL_U32_RESOLUTION(p_u32_pos_x, p_u32_pos_y);

    /* set MCTRL register */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + VIOCTRL_U32_VIOC_WMIX_MCTRL) = p_u32_layer_prio;

    if(l_mem_close_f(fd_mem, (void*)vp_map, u32_size)) {
        fprintf(stderr, "Can not close VIOC mem map\n");
        return -1;
    }
    return 0;
}

static int l_configure_sc_f(uint32_t p_u32_id, uint32_t p_u32_rdma, 
                             uint32_t p_u32_width, uint32_t p_u32_height,
                             uint32_t p_u32_out_width, uint32_t p_u32_out_height,
                             uint32_t p_u32_pos_x, uint32_t p_u32_pos_y)
{
    volatile void* vp_map;
    uint32_t u32_size;
    int fd_mem;
    uint32_t u32_comp_base;
    uint32_t u32_cfg_path_sc_offs;
    uint32_t u32_cnt;

    switch(p_u32_id) {
    case 0:
        u32_comp_base = L_U32_VIOC_BASE_SC0;
        u32_cfg_path_sc_offs = L_U32_VIOC_CFG_PATH_SC0;
        break;
    case 1:
        u32_comp_base = L_U32_VIOC_BASE_SC1;
        u32_cfg_path_sc_offs = L_U32_VIOC_CFG_PATH_SC1;
        break;
    case 2:
        u32_comp_base = L_U32_VIOC_BASE_SC2;
        u32_cfg_path_sc_offs = L_U32_VIOC_CFG_PATH_SC2;
        break;
    default:
        fprintf(stderr, "Invalid SC ID %d\n", p_u32_id);
        return -1;
    }

    u32_size = L_U32_VIOC_MEM_SIZE;
    vp_map = l_mem_init_f(&fd_mem, L_U32_VIOC_MEM_BASE, &u32_size);
    if(NULL == vp_map) {
        fprintf(stderr, "Can not map VIOC periphery memory\n");
        return 0;
    }

    /* check if the same scaler input is in use */
    if(L_GET_PATHSC_SEL(VIOCTRL_REG_MACRO(vp_map, u32_cfg_path_sc_offs)) != (p_u32_rdma & 0xFF)) {
        /* stop scaler */
        VIOCTRL_REG_MACRO(vp_map, u32_cfg_path_sc_offs) = 0;
        /* wait for disconencted state */
        for(u32_cnt = 0; u32_cnt < L_SCALER_WAIT_TIMEOUT_CNT; ++u32_cnt) {
            if(L_GET_PATHSC_STS(VIOCTRL_REG_MACRO(vp_map, u32_cfg_path_sc_offs)) == L_U32_SCALER_DISCONNECTED) {
                break;
            }
            (void)usleep(L_SCALER_WAIT_SLEEP_MS * 1000);
        }
        if(u32_cnt >= L_SCALER_WAIT_TIMEOUT_CNT) {
            fprintf(stderr, "Scaler disconnect wait timeout\n");
            (void)l_mem_close_f(fd_mem, (void*)vp_map, u32_size);
            return -1;
        }
        //fprintf(stderr, "Scaler Disconnect Wait returned in %d\n", u32_cnt);
        /* configure to the new path */
        VIOCTRL_REG_MACRO(vp_map, u32_cfg_path_sc_offs) = (p_u32_rdma & 0xFF);
    }

    /* set Scaler Dest Size */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + L_U32_VIOC_SCDSIZE) = VIOCTRL_U32_RESOLUTION(p_u32_width, p_u32_height);

    /* set Scaler Output Size */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + L_U32_VIOC_SCOSIZE) = VIOCTRL_U32_RESOLUTION(p_u32_out_width, p_u32_out_height);

    /* set Scaler Output Position */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + L_U32_VIOC_SCOPOS) = VIOCTRL_U32_RESOLUTION(p_u32_pos_x, p_u32_pos_y);

    /* set Scaler Control */
    VIOCTRL_REG_MACRO(vp_map, u32_comp_base + L_U32_VIOC_SCCTRL) = L_U32_DEFAULT_SCCTRL;

    /* enable scaler path */
    VIOCTRL_REG_MACRO(vp_map, u32_cfg_path_sc_offs) |= (1 << 31);
    /* wait for connected state */
    for(u32_cnt = 0; u32_cnt < L_SCALER_WAIT_TIMEOUT_CNT; ++u32_cnt) {
        if(L_GET_PATHSC_STS(VIOCTRL_REG_MACRO(vp_map, u32_cfg_path_sc_offs)) == L_U32_SCALER_CONNECTED) {
            break;
        }
        (void)usleep(L_SCALER_WAIT_SLEEP_MS * 1000);
    }
    if(u32_cnt >= L_SCALER_WAIT_TIMEOUT_CNT) {
        fprintf(stderr, "Scaler connecting wait timeout\n");
        (void)l_mem_close_f(fd_mem, (void*)vp_map, u32_size);
        return -1;
    }
    //fprintf(stderr, "Scaler Connecting Wait returned in %d\n", u32_cnt);

    if(l_mem_close_f(fd_mem, (void*)vp_map, u32_size)) {
        fprintf(stderr, "Can not close VIOC mem map\n");
        return -1;
    }

    return 0;
}

static int l_stop_sc_f(uint32_t p_u32_id)
{
    volatile void* vp_map;
    uint32_t u32_size;
    int fd_mem;
    uint32_t u32_comp_base;
    uint32_t u32_cfg_path_sc_offs;

    switch(p_u32_id) {
    case 0:
        u32_comp_base = L_U32_VIOC_BASE_SC0;
        u32_cfg_path_sc_offs = L_U32_VIOC_CFG_PATH_SC0;
        break;
    case 1:
        u32_comp_base = L_U32_VIOC_BASE_SC1;
        u32_cfg_path_sc_offs = L_U32_VIOC_CFG_PATH_SC1;
        break;
    case 2:
        u32_comp_base = L_U32_VIOC_BASE_SC2;
        u32_cfg_path_sc_offs = L_U32_VIOC_CFG_PATH_SC2;
        break;
    default:
        fprintf(stderr, "Invalid SC ID %d\n", p_u32_id);
        return -1;
    }

    u32_size = L_U32_VIOC_MEM_SIZE;
    vp_map = l_mem_init_f(&fd_mem, L_U32_VIOC_MEM_BASE, &u32_size);
    if(NULL == vp_map) {
        fprintf(stderr, "Can not map VIOC periphery memory\n");
        return 0;
    }

    /* set Scaler Control */
    //VIOCTRL_REG_MACRO(vp_map, u32_comp_base + L_U32_VIOC_SCCTRL) = L_U32_BYPASS_SCCTRL;

    /* configure SC path register -> Disable */
    VIOCTRL_REG_MACRO(vp_map, u32_cfg_path_sc_offs) = 0;

    if(l_mem_close_f(fd_mem, (void*)vp_map, u32_size)) {
        fprintf(stderr, "Can not close VIOC mem map\n");
        return -1;
    }

    return 0;
}

/** Internal function to determine the register value for differnet upscale factors
 *
 *  @param  p_u32_factor    upscale factor
 *  @return <0 error, else the register value
 *
 *  Valid upscale factors: 1,2,3,4,8
 */
static inline int l_get_rdma_upscale_f(uint32_t p_u32_factor)
{
    switch (p_u32_factor) {
    case 1:
        return L_U8_RDMA_UPSCALE_1;
    case 2:
        return L_U8_RDMA_UPSCALE_2;
    case 3:
        return L_U8_RDMA_UPSCALE_3;
    case 4:
        return L_U8_RDMA_UPSCALE_4;
    case 8:
        return L_U8_RDMA_UPSCALE_8;
    default:
        fprintf(stderr, "Invalid RDMA upscaling factor %d\n", p_u32_factor);
        return -1;
    }
}

/** Internal function to determine scaler select value belongs to an RDMA ID 
 *
 *  @param  p_u32_rdma  RDMA ID
 *  @return scaler selcet value
 *
 *  The returned value van be used in the SEL field of the CFG_PATH_SC register
 */
static inline uint32_t l_get_scaler_sel_rdma_f(uint32_t p_u32_rdma)
{
    switch(p_u32_rdma) {
    case 16:
        return 0x11;
    case 17:
        return 0x13;
    default:
        return p_u32_rdma;
    }
}

/** Internal function to determine scaler select value belongs to a WDMA ID 
 *
 *  @param  p_u32_wdma  WDMA ID
 *  @return scaler selcet value
 *
 *  The returned value van be used in the SEL field of the CFG_PATH_SC register
 */
static inline uint32_t l_get_scaler_sel_wdma_f(uint32_t p_u32_wdma)
{
    return (p_u32_wdma + 0x14);
}

/** Internal function to initialize scaler component
 *
 *  @param  p_sp_in     parameters of the HDMI input
 *  @return zero in case of success
 *
 *  Scaler connected to WDMA wroks only if it was first initialized to RDMA.
 */
static int l_init_sc_f(const scale_s_calc_in_t* p_sp_in) 
{
    uint32_t u32_scaled_x;
    uint32_t u32_scaled_y;

    u32_scaled_x = (uint32_t)(p_sp_in->s_in.u32_x * L_F32_SCINIT_DOWNSCALE);
    u32_scaled_y = (uint32_t)(p_sp_in->s_in.u32_y * L_F32_SCINIT_DOWNSCALE);

    /* check argument */
    if(NULL == p_sp_in) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }
    /* hide */
    if(l_configure_wmix_f(L_U32_AUTO_WMIX_ID, L_U32_AUTO_WMIX_CH, 
                0, 0,
                L_U32_HIDE_WMIX_MCTRL)) {
        fprintf(stderr, "Can not configure WMIX\n");
        return -1;
    }
    if(0 == l_u32_dmabuf_addr) {
        l_u32_dmabuf_addr = vioctrl_get_memarea_f(L_ST_PMAP_EXTCAM, L_U32_MIN_MEMSIZE);
        if(0 == l_u32_dmabuf_addr) {
            fprintf(stderr, "Can not get DMA buffer\n");
            return -1 ;
        }
    }
    /* initialize RDMA */
    if(l_configure_dma_f(0, l_u32_dmabuf_addr, 
                         L_U32_AUTO_RDMA_ID, 
                         p_sp_in->s_in.u32_x,
                         p_sp_in->s_in.u32_y,
                         1, 1, 
                         p_sp_in->s_in.u32_x * p_sp_in->u32_bytes_per_pixel,
                         L_U32_DEFAULT_RDMACTRL)) {
        fprintf(stderr, "Can not initialize RDMA during scaler initialization\n");
        return -1;
    }
    /* initialize WDMA */
    if(l_configure_dma_f(1, l_u32_dmabuf_addr, 
                         L_U32_AUTO_WDMA_ID, 
                         p_sp_in->s_in.u32_x,
                         p_sp_in->s_in.u32_y,
                         1, 1, 
                         p_sp_in->s_in.u32_x * p_sp_in->u32_bytes_per_pixel,
                         L_U32_DEFAULT_RDMACTRL)) {
        fprintf(stderr, "Can not initialize RDMA during scaler initialization\n");
        return -1;
    }
    /* configure a small downscale on RDMA */
    if(l_configure_sc_f(L_U32_AUTO_SC_ID, L_U32_AUTO_RDMA_ID,
                        u32_scaled_x, u32_scaled_y,
                        u32_scaled_x, u32_scaled_y, 0, 0)) {
        fprintf(stderr, "Can not initialize scaler during pre initializateion to %dx%d\n",
                u32_scaled_x, u32_scaled_y);
        return -1;
    }
    /* stop after initialization */
    if(l_stop_f()) {
        fprintf(stderr, "Can not stop after scaler initialization\n");
        return -1;
    }

    return 0;
}

static int l_stop_f(void) 
{
    /* switch layer */
    if(l_configure_wmix_f(L_U32_AUTO_WMIX_ID, L_U32_AUTO_WMIX_CH, 
                0, 0,
                L_U32_STOP_WMIX_MCTRL)) {
        fprintf(stderr, "Can not configure WMIX\n");
        return -1;
    }
    if(0 == l_u32_dmabuf_addr) {
        l_u32_dmabuf_addr = vioctrl_get_memarea_f(L_ST_PMAP_EXTCAM, L_U32_MIN_MEMSIZE);
        if(0 == l_u32_dmabuf_addr) {
            fprintf(stderr, "Can not get DMA buffer\n");
            return -1;
        }
    }
    /* stop RDMA */
    if(l_configure_dma_f(0, l_u32_dmabuf_addr,
                L_U32_AUTO_RDMA_ID,
                0, 0, 1, 1, 0,
                L_U32_STOP_RDMACTRL)) {
        fprintf(stderr, "Can not configure RDMA\n");
        return -1;
    }
    /* stop WDMA */
    if(l_configure_dma_f(1, l_u32_dmabuf_addr,
                L_U32_AUTO_WDMA_ID,
                0, 0, 1, 1, 0,                            
                L_U32_STOP_WDMACTRL)) {
        fprintf(stderr, "Can not configure WDMA\n");
        return -1;
    }
    /* stop SCaler */
    if(l_stop_sc_f(L_U32_AUTO_SC_ID)) {
        fprintf(stderr, "Can not Stop SCaler\n");
        return -1;
    }
    return 0;
}

/* blocking mode scroll function */
static int l_scroll_loop_f(uint32_t p_u32_id, uint32_t p_u32_base, 
                           uint32_t p_u32_ctrl, uint32_t p_u32_sleep_ms,
                           uint32_t p_u32_text_w, uint32_t p_u32_text_h, 
                           uint32_t p_u32_win_w, uint32_t p_u32_win_h)
{
    uint32_t u32_comp_base;
    uint32_t u32_size;
    void* vp_map;
    int fd_mem;
    volatile uint32_t* u32p_base;
    volatile uint32_t* u32p_ctrl;
    uint32_t u32_offs;
    uint32_t u32_end;
    
    u32_comp_base = vioctrl_get_rdma_base_f(p_u32_id);
    if(0 == u32_comp_base) {
        fprintf(stderr, "Invalid RDMA id %d\n", p_u32_id);
        return -1;
    }

    /* map memory */
    u32_size = L_U32_VIOC_MEM_SIZE;
    vp_map = l_mem_init_f(&fd_mem, L_U32_VIOC_MEM_BASE, &u32_size);
    if(NULL == vp_map) {
        fprintf(stderr, "Can not map VIOC periphery memory\n");
        return 0;
    }
    u32p_base = (uint32_t*)(vp_map + u32_comp_base + VIOCTRL_U32_VIOC_RDMABASE0);
    u32p_ctrl = (uint32_t*)(vp_map + u32_comp_base + VIOCTRL_U32_VIOC_RDMACTRL);

    /* start loop */
    u32_offs = 0;
    /* calculate end offset */
    if(p_u32_text_w > p_u32_win_w) 
        u32_end = p_u32_text_w - p_u32_win_w;
    else
        u32_end = p_u32_text_w;
    while(1) {
        /* check end of window */
        if(u32_offs > u32_end) 
            u32_offs = 0;
        /* set offset */
        *u32p_base = p_u32_base + (u32_offs * L_U32_SCROLL_BYTES_PER_PIXEL);
        /* update */
        *u32p_ctrl = p_u32_ctrl;
        /* increment offset */
        ++u32_offs;
        /* sleep 66 ms */
        usleep(p_u32_sleep_ms*1000);
    }

    /* unmap & close */
    if(l_mem_close_f(fd_mem, (void*)vp_map, u32_size)) {
        fprintf(stderr, "Can not close VIOC mem map\n");
        return -1;
    }
    return 0;
}

/** Internal function to convert hexadecimal string to binary array
 *
 *  @param  p_cp_hex    input: hexadecimal number string (MSB)
 *  @param  p_au8_bin   output: array for binary output (LSB)
 *  @return number of converted bytes (length of output array), negative: error
 *
 *  The function converts hexadecimal numbers from the string (MSB) to binary array (LSB).
 */
static int l_hex_to_bin_f(const char* p_cp_hex, uint8_t* p_au8_bin, uint32_t p_u32_size)
{
    uint32_t u32_index;
    uint32_t u32_out;
    uint32_t u32_len;
    char st_hex[3];
    uint8_t u8_num;
    char* cp_err;
    int return_value = 0;
    
    /* check length */
    u32_len = strlen(p_cp_hex);
    
    /* initialize output */
    memset(p_au8_bin, 0, p_u32_size);
    
    for(u32_index = 0; u32_index < u32_len; u32_index+=2) {
        /* read hex number */
        st_hex[0] = p_cp_hex[u32_index];
        st_hex[1] = p_cp_hex[u32_index + 1];
        st_hex[2] = '\0';
        /* convert hex to binary */
        u8_num = (uint8_t)strtoul(st_hex, &cp_err, 16);
        if(cp_err && *cp_err) {
            fprintf(stderr, "%s is not a hex number\n", st_hex);
            return -1;
        }
#if 1        
        /* calculate output index MSB -> LSB */
        u32_out = ((u32_len / 2) - 1) - (u32_index / 2);
#else 
        /* calculate output index : no change */
        u32_out = u32_index/2;
#endif
        /* write to output array */
        if(u32_out < p_u32_size) {
            /* store value in output buffer */
            p_au8_bin[u32_out] = u8_num;
            return_value++;
        } else {
            fprintf(stderr, "output index %d is out of range %d\n", u32_out, p_u32_size);
        }
    }
    return return_value;
}

/** Internal function to parse a HDCP key file 
 *
 *  @param  p_sp_hdcp_key   output argument: parsed HDCP key
 *  @param  p_cp_file       input argument: file to be parsed
 *  @return zero in case success
 *
 *  The expected file format: 
 *  1st line contains 40 bit KSV, then 40 line consequently contains 40 keys, 
 *  one 56 bit length key per line. All keys and the KSV is in hexadecimal format.
 *  Example:
 *    B974AD6521
 *    6811B2B439B065
 *    DA0A3EF379E29B
 * ...
 */
static int l_parse_hdcp_key_file_f(l_s_hdcp_key_t* p_sp_hdcp_key, const char* p_cp_file)
{
    int return_value = 0;
    FILE* sp_file;
    char st_line[128];
    uint32_t u32_index;
    char* cp_nl;
    int res;

    /* check arguments */
    if((NULL == p_sp_hdcp_key) || (NULL == p_cp_file)) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* open file */
    sp_file = fopen(p_cp_file, "r");
    if(NULL == sp_file) {
        perror("open hdcp key file");
        return -1;
    }

    /* read KSV */
    if(NULL == fgets(st_line, sizeof(st_line), sp_file)) {
        fprintf(stderr, "Can not read KSV from HDCP key file %s\n", p_cp_file);
        (void)fclose(sp_file);
        return -1;
    }
    /* terminate line */
    cp_nl = strchr(st_line, '\n');
    if(cp_nl) *cp_nl = '\0';
    cp_nl = strchr(st_line, '\r');
    if(cp_nl) *cp_nl = '\0';
    /* check length */
    if(0 == strlen(st_line)) {
        fprintf(stderr, "HDCP KSV (%s) length is 0\n", st_line);
        (void)fclose(sp_file);
        return -1;
    }
    /* convert KSV */
    res = l_hex_to_bin_f(st_line, p_sp_hdcp_key->au8_ksv, sizeof(p_sp_hdcp_key->au8_ksv));
    if(L_HDCP_KSV_LEN != res) {
        fprintf(stderr, "HDCP KSV %s can not be converted to binary returned %d, expected %d\n", st_line, res, L_HDCP_KSV_LEN);
        (void)fclose(sp_file);
        return -1;
    }
    
    /* read keys */
    for(u32_index = 0; u32_index < L_HDCP_KEY_NUM; ++u32_index) {
        /* read line from file */
        if(NULL == fgets(st_line, sizeof(st_line), sp_file)) {
            fprintf(stderr, "Can not read key %d from HDCP key file %s\n", u32_index, p_cp_file);
            return_value = -1;
            break;
        }
        /* terminate line */
        cp_nl = strchr(st_line, '\n');
        if(cp_nl) *cp_nl = '\0';
        cp_nl = strchr(st_line, '\r');
        if(cp_nl) *cp_nl = '\0';
        /* check length */
        if(0 == strlen(st_line)) {
            fprintf(stderr, "HDCP key %d (%s) length is 0\n", u32_index, st_line);
            return_value = -1;
            break;
        }
        /* convert key */
        res = l_hex_to_bin_f(st_line, p_sp_hdcp_key->au8_keys[u32_index], sizeof(p_sp_hdcp_key->au8_keys[u32_index]));
        if(L_HDCP_KEY_LEN != res) {
            fprintf(stderr, "HDCP key %d (%s) can not be converted to binary, returned %d, expected %d\n", u32_index, st_line, res, L_HDCP_KEY_LEN);
            return_value = -1;
            break;
        }
    }

    /* close input file */
    if(fclose(sp_file)) {
        perror("HDCP key file close");
        return -1;
    }

    return return_value;
}

/*================================[ EXPORTED FUNCTION DEFINITIONS ]==========*/

#ifdef VIOCTRL_MAIN
int main(int argc, char* argv[])
{
    const char* cp_cmd = NULL;

    //printf("Hello vioctrl\n");

    /* check arguments */
    if(argc < 2) {
        l_usage_f(argv[0]);
        return -1;
    }

    /* get command */
    cp_cmd = argv[1];

    if(0 == strcasecmp(cp_cmd, "hdmichip")) {
        l_hdmichip_cmd_f(argc, argv);
    } else if(0 == strcasecmp(cp_cmd, "pins")) {
        l_pins_cmd_f(argc, argv);
    } else if(0 == strcasecmp(cp_cmd, "vin")) {
        l_vin_cmd_f(argc, argv);
    } else if((0 == strcasecmp(cp_cmd, "wdma")) || 
              (0 == strcasecmp(cp_cmd, "rdma"))) {
        l_dma_cmd_f(argc, argv);
    } else if(0 == strcasecmp(cp_cmd, "wmix")) {
        l_wmix_cmd_f(argc, argv);
    } else if(0 == strcasecmp(cp_cmd, "sc")) {
        l_sc_cmd_f(argc, argv);
    } else if((0 == strcasecmp(cp_cmd, "start")) || 
              (0 == strcasecmp(cp_cmd, "stop"))) {
        l_auto_cmd_f(argc, argv);
    } else if((0 == strcasecmp(cp_cmd, "load")) || 
              (0 == strcasecmp(cp_cmd, "save"))) {
        l_load_save_cmd_f(argc, argv);
    } else if(0 == strcasecmp(cp_cmd, "scroll")) {
        l_scroll_cmd_f(argc, argv);
    } else {
       fprintf(stderr, "invalid command %s\n", cp_cmd);
        return -1;
    }

    return 0;
}
#endif

/* Function to get dmabuffer address from /proc/pmap (see header for more details) */
uint32_t vioctrl_get_memarea_f(const char* p_cp_name, uint32_t p_u32_min_size)
{
    FILE* sp_file;
    uint32_t u32_addr;
    uint32_t u32_size;
    char* cp_err;
    char st_line[256];

    /* open /proc/pmap */
    sp_file = fopen(L_ST_PROC_PMAP, "r");
    if(NULL == sp_file) {
        perror("open " L_ST_PROC_PMAP);
        return 0;
    }

    /* read input */
    while(NULL != fgets(st_line, sizeof(st_line), sp_file)) {
        char* cp_nl;
        char* cp_addr = NULL;
        char* cp_size = NULL;
        char* cp_name = NULL;
        
        /* terminate line */
        cp_nl = strrchr(st_line, '\n');
        if(cp_nl) {
            *cp_nl = '\0';
        }
        /* parse line */
        cp_addr = strtok(st_line, L_ST_DELIM);
        if(cp_addr) {
            cp_size = strtok(NULL, L_ST_DELIM);
        }
        if(cp_size) {
            cp_name = strtok(NULL, L_ST_DELIM);
        }
        if(NULL == cp_name) {
            continue;
        }
        u32_addr = (uint32_t)strtoul(cp_addr, &cp_err, 16);
        if(cp_err && *cp_err) {
            continue;
        }
        u32_size = (uint32_t)strtoul(cp_size, &cp_err, 16);
        if(cp_err && *cp_err) {
            continue;
        }
        /* check name */
        if(0 != strcmp(cp_name, p_cp_name)) {
            continue;
        }
        /* check size */
        if(u32_size < p_u32_min_size) {
            break;
        }
        (void)fclose(sp_file);
        return u32_addr;
    }

    (void)fclose(sp_file);
    return 0;
}

/* Function to load file into memory (see header for more details) */
int vioctrl_load_file_f(const char* p_cp_file, uint32_t p_u32_addr)
{
    int return_value = 0;
    struct stat s_stat;
    uint32_t u32_size_file;
    uint32_t u32_size_mem;
    int fd_mem;
    void* vp_ptr;
    FILE* sp_file;
    int ret;

    /* check argument */
    if(NULL == p_cp_file) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    /* get size of the input file */
    if(stat(p_cp_file, &s_stat)) {
        perror("stat");
        return -1;
    }
    u32_size_file = (uint32_t)s_stat.st_size;
    u32_size_mem = u32_size_file;

    /* map address */
    vp_ptr = l_mem_init_f(&fd_mem, p_u32_addr, &u32_size_mem);
    if(NULL == vp_ptr) {
        fprintf(stderr, "Can not map 0x%.8x (%d)\n", p_u32_addr, u32_size_mem);
        return -1;
    }

    sp_file = fopen(p_cp_file, "rb");
    if(sp_file) {
        ret = fread(vp_ptr, 1, u32_size_file, sp_file);
        if(ret != u32_size_file) {
            fprintf(stderr, "Can not read %d bytes from %s (%d)\n", u32_size_file, p_cp_file, ret);
            return_value = -1;
        }
        if(fclose(sp_file)) {
            perror("close");
            return_value = -1;
        }
    } else {
        perror("open");
        fprintf(stderr, "Can not open %s\n", p_cp_file);
        return_value = -1;
    }

    l_mem_close_f(fd_mem, vp_ptr, u32_size_mem);

    return return_value;
}

/* Function to save memory dump into a file memory (see header for more details) */
int vioctrl_save_file_f(const char* p_cp_file, uint32_t p_u32_addr, uint32_t p_u32_len)
{
    int return_value = 0;
    uint32_t u32_size_mem;
    int fd_mem;
    void* vp_ptr;
    FILE* sp_file;
    int ret;

    /* check argument */
    if(NULL == p_cp_file) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    u32_size_mem = p_u32_len;

    /* map address */
    vp_ptr = l_mem_init_f(&fd_mem, p_u32_addr, &u32_size_mem);
    if(NULL == vp_ptr) {
        fprintf(stderr, "Can not map 0x%.8x (%d)\n", p_u32_addr, u32_size_mem);
        return -1;
    }

    sp_file = fopen(p_cp_file, "wb");
    if(sp_file) {
        ret = fwrite(vp_ptr, 1, p_u32_len, sp_file);
        if(ret != p_u32_len) {
            fprintf(stderr, "Can not write %d bytes to %s (%d)\n", p_u32_len, p_cp_file, ret);
            return_value = -1;
        }
        if(fclose(sp_file)) {
            perror("close");
            return_value = -1;
        }
    } else {
        perror("open");
        fprintf(stderr, "Can not open %s\n", p_cp_file);
        return_value = -1;
    }

    l_mem_close_f(fd_mem, vp_ptr, u32_size_mem);

    return return_value;
}

/* Function to get the base address of an RDMA VIOC component (see header for more details) */
uint32_t vioctrl_get_rdma_base_f(uint32_t p_u32_id)
{
    uint32_t u32_comp_base = 0;

    switch(p_u32_id) {
    case 0:
        u32_comp_base = L_U32_VIOC_BASE_RDMA00;
        break;
    case 1:
        u32_comp_base = L_U32_VIOC_BASE_RDMA01;
        break;
    case 2:
        u32_comp_base = L_U32_VIOC_BASE_RDMA02;
        break;
    case 3:
        u32_comp_base = L_U32_VIOC_BASE_RDMA03;
        break;
    case 4:
        u32_comp_base = L_U32_VIOC_BASE_RDMA04;
        break;
    case 5:
        u32_comp_base = L_U32_VIOC_BASE_RDMA05;
        break;
    case 6:
        u32_comp_base = L_U32_VIOC_BASE_RDMA06;
        break;
    case 7:
        u32_comp_base = L_U32_VIOC_BASE_RDMA07;
        break;
    case 12:
        u32_comp_base = L_U32_VIOC_BASE_RDMA12;
        break;
    case 13:
        u32_comp_base = L_U32_VIOC_BASE_RDMA13;
        break;
    case 14:
        u32_comp_base = L_U32_VIOC_BASE_RDMA14;
        break;
    case 15:
        u32_comp_base = L_U32_VIOC_BASE_RDMA15;
        break;
    case 16:
        u32_comp_base = L_U32_VIOC_BASE_RDMA16;
        break;
    case 17:
        u32_comp_base = L_U32_VIOC_BASE_RDMA17;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    default:
        fprintf(stderr, "Invalid RDMA id %d\n", p_u32_id);
        u32_comp_base = 0;
        break;
    }
    return u32_comp_base;
}        

/* Function to get the base address of an WDMA VIOC component (see header for more details) */
uint32_t vioctrl_get_wdma_base_f(uint32_t p_u32_id)
{
    uint32_t u32_comp_base = 0;

    switch(p_u32_id) {
        case 0:
            u32_comp_base = L_U32_VIOC_BASE_WDMA00;
            break;
        case 1:
            u32_comp_base = L_U32_VIOC_BASE_WDMA01;
            break;
        case 3:
            u32_comp_base = L_U32_VIOC_BASE_WDMA03;
            break;
        case 4:
            u32_comp_base = L_U32_VIOC_BASE_WDMA04;
            break;
        case 5:
            u32_comp_base = L_U32_VIOC_BASE_WDMA05;
            break;
        case 6:
            u32_comp_base = L_U32_VIOC_BASE_WDMA06;
            break;
        case 7:
            u32_comp_base = L_U32_VIOC_BASE_WDMA07;
            break;
        case 8:
            u32_comp_base = L_U32_VIOC_BASE_WDMA08;
            break;
        case 2:
        default:
            fprintf(stderr, "Invalid WDMA id %d\n", p_u32_id);
            u32_comp_base = 0;
            break;
    }
    return u32_comp_base;
}        

/* Function to get the base address of a WMIX VIOC component (see header for more details) */
uint32_t vioctrl_get_wmix_base_f(uint32_t p_u32_id)
{
    uint32_t u32_comp_base = 0;

    switch(p_u32_id) {
    case 0:
        u32_comp_base = L_U32_VIOC_BASE_WMIX0;
        break;
    case 1:
        u32_comp_base = L_U32_VIOC_BASE_WMIX1;
        break;
    case 3:
        u32_comp_base = L_U32_VIOC_BASE_WMIX3;
        break;
    case 4:
        u32_comp_base = L_U32_VIOC_BASE_WMIX4;
        break;
    case 5:
        u32_comp_base = L_U32_VIOC_BASE_WMIX5;
        break;
    case 2:
    default:
        fprintf(stderr, "Invalid WMIX id %d\n", p_u32_id);
        u32_comp_base = 0;
        break;
    }
    return u32_comp_base;
}

/* Function to map the VIOC module registers to user accesible memory (see header for more details) */
void* vioctrl_get_vioc_mem_f(int* p_fdp_mem, uint32_t* p_u32p_size)
{
    /* check arguments */
    if((NULL == p_fdp_mem) || (NULL == p_u32p_size)) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return NULL;
    }

    *p_u32p_size = L_U32_VIOC_MEM_SIZE;
    return l_mem_init_f(p_fdp_mem, L_U32_VIOC_MEM_BASE, p_u32p_size);
}

/* Function to free/close the mapped memory area (see header for more details) */
int vioctrl_mem_close_f(int p_fd_mem, void* p_u32p_base, uint32_t p_u32_size)
{
    /* check arguments */
    if(NULL == p_u32p_base) {
        fprintf(stderr, "NULL argument for %s\n", __func__);
        return -1;
    }

    return l_mem_close_f(p_fd_mem, p_u32p_base, p_u32_size);
}

/**@}*/
