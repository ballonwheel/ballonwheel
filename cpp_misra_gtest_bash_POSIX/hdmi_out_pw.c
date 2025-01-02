/* linux/drivers/gpio/hdmi_out_pw.c
 *
 * Copyright (C) 2013 Dension Audio Systems
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/div64.h>
#include <asm/mach/map.h>
#ifdef CONFIG_PM
#include <linux/pm.h>
#endif
#include <mach/structures_smu_pmu.h>
#include <mach/tcc_fb.h>
#include <mach/bsp.h>
#include <mach/tca_ckc.h>
#include <mach/tca_fb_hdmi.h>
#include <mach/tccfb_ioctrl.h>
#include <mach/vioc_outcfg.h>
#include <mach/globals.h>
#include <mach/reg_physical.h>

#include <mach/tcc_board_hdmi.h>
//#include <mach/hdmi.h>
  
  
#define HDMI_OUT_PW_SUCCESS	0
#define HDMI_OUT_PW_FAILED	-1


//pdf page181, 4.3 Register Descriptions
//Main State for PMU Power Management
enum {
    MAIN_STATE_IDLE = 0,
    MAIN_STATE_ALL_DOWN,
    MAIN_STATE_ALL_UP,
    MAIN_STATE_MBUS_DOWN,
    MAIN_STATE_MBUS_UP,
    MAIN_STATE_VBUS_DOWN,
    MAIN_STATE_VBUS_UP,
    MAIN_STATE_GBUS_DOWN,
    MAIN_STATE_GBUS_UP,
    MAIN_STATE_DBUS_DOWN,
    MAIN_STATE_DBUS_UP,
    MAIN_STATE_HSBUS_DOWN,
	MAIN_STATE_HSBUS_UP,
};

  
static int init_result;


/*******************************************************************************
 *
 ******************************************************************************/
unsigned int IO_PMU_EnterPowerDown_DBUS (void)
{
#if 1
    volatile PPMU pPMU = (volatile PPMU)(tcc_p2v(HwPMU_BASE));
    
    pPMU->PMU_ISOL.bREG.HDMI = 1;
    
    return 0;

#else
    // TO DO
    #define MAX_LCDC_WAIT_TIEM 		0x70000000

    unsigned loop;
    unsigned int nWakeupEnBackup0;
    unsigned int nWakeupEnBackup1;
    volatile PPMU pPMU = (volatile PPMU)(tcc_p2v(HwPMU_BASE));
    volatile PMEMBUSCFG pMEMBUSCFG = (volatile PMEMBUSCFG)(tcc_p2v(HwMBUSCFG_BASE));
    volatile PCKC pCKC= (volatile PCKC)tcc_p2v(HwCKC_BASE);	

    if ( pPMU->PMU_PWRSTS.bREG.PD_DB == 1 ) {
        printk("IO_PMU_EnterPowerDown_DBUS()->Note: already in power-down status.\n");
        return (HDMI_OUT_PW_SUCCESS);
    }

    /* The backup and restoring code for PMU_WKUPENx registers are due to the design error */
    nWakeupEnBackup0 = pPMU->PMU_WKUP0.bREG.GPIO_HDMI00;
    nWakeupEnBackup1 = pPMU->PMU_WKUP0.bREG.GPIO_HDMI01;
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI00 = 0;
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI01 = 0;
    if ( pPMU->PMU_PWRSTS.bREG.MAIN_STATE != MAIN_STATE_IDLE ) {
        printk("IO_PMU_EnterPowerDown_DBUS()->Error: It should be in IDLE state.");
        return (HDMI_OUT_PW_FAILED);
    }

    /* HCLKMASK and MBUSSTS registers are described in the MEMORY BUS datasheet.*/
    pMEMBUSCFG->HCLKMASK.bREG.DBUS = 0;	/*BIT9*/
    
    // video bus clock masking
    loop = 0;
    //while ( (pMEMBUSCFG->MBUSSTS.bREG.BITNUM2 & pMEMBUSCFG->MBUSSTS.bREG.BITNUM6) == 1);
    while ( (pMEMBUSCFG->MBUSSTS.bREG.DBUS0 & pMEMBUSCFG->MBUSSTS.bREG.DBUS1) == 1){
        if(loop++ > MAX_LCDC_WAIT_TIEM){
            printk("IO_PMU_EnterPowerDown_DBUS()->Error: video bus clock masking TIMEOUT!");
            return (HDMI_OUT_PW_FAILED);
    }
    };
    
    
    pPMU->PMU_SYSRST.bREG.DB = 0;
    //??
    //HDMI_IOC_SET_PHYRESET
    
    // reset the display bus from PMU.
    /* CLKCTRL2 register is described in the CKC */
    /* Peripheral clock for Display Bus could be disabled at here */
    pCKC->CLKCTRL2.bREG.EN = 0;
    
    // Bus clock for Video Bus
    pPMU->PWRDN_DBUS.nREG = 1;
    
    // enter power-down
    loop = 0;
    while ( pPMU->PMU_PWRSTS.bREG.PD_DB == 1 ){
        if(loop++ > MAX_LCDC_WAIT_TIEM){
            printk("IO_PMU_EnterPowerDown_DBUS()->Error: enter power-down TIMEOUT!");
            return (HDMI_OUT_PW_FAILED);
    	}
    };
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI00 = nWakeupEnBackup0;
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI01 = nWakeupEnBackup1;
    
    return (HDMI_OUT_PW_SUCCESS);
#endif
}


unsigned int IO_PMU_ExitPowerDown_DBUS (void)
{
#if 1
    volatile PPMU pPMU = (volatile PPMU)(tcc_p2v(HwPMU_BASE));
    
    pPMU->PMU_ISOL.bREG.HDMI = 0;

    return 0;
#else

    // TO DO
    #define MAX_LCDC_WAIT_TIEM 		0x70000000

    unsigned loop;
    unsigned int nWakeupEnBackup0;
    unsigned int nWakeupEnBackup1;
    volatile PPMU pPMU = (volatile PPMU)(tcc_p2v(HwPMU_BASE));
    volatile PMEMBUSCFG pMEMBUSCFG = (volatile PMEMBUSCFG)(tcc_p2v(HwMBUSCFG_BASE));
    volatile PCKC pCKC= (volatile PCKC)tcc_p2v(HwCKC_BASE);	

    if ( pPMU->PMU_PWRSTS.bREG.PU_DB == 1 ) {
        printk("IO_PMU_ExitPowerDown_DBUS()->Note: already in power-up status.");
        return (HDMI_OUT_PW_SUCCESS);
    }

    /* The backup and restoring code for PMU_WKUPENx registers are due to the design error */
    nWakeupEnBackup0 = pPMU->PMU_WKUP0.bREG.GPIO_HDMI00;
    nWakeupEnBackup1 = pPMU->PMU_WKUP0.bREG.GPIO_HDMI01;
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI00 = 0;
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI01 = 0;
    if ( pPMU->PMU_PWRSTS.bREG.MAIN_STATE != MAIN_STATE_IDLE ) {
        printk("IO_PMU_ExitPowerDown_DBUS()->Error: PMU status is not idle state. It should be in IDLE state.");
        return (HDMI_OUT_PW_FAILED);
    }

    /* CLKCTRL2 register is described in the CKC */
    /* Clock configuration Sources could be located at this. */
    pCKC->CLKCTRL2.bREG.EN = 1;

    // Bus clock for Display Bus
    pPMU->PWRUP_DBUS.nREG = 1;

    // exit power-down
    loop = 0;
    while ( pPMU->PMU_PWRSTS.bREG.PU_DB == 1 ){
        if(loop++ > MAX_LCDC_WAIT_TIEM){
            printk("IO_PMU_ExitPowerDown_DBUS()->Error: exit power-down TIMEOUT!");
            return (HDMI_OUT_PW_FAILED);
        }
    };
    pPMU->PMU_SYSRST.bREG.DB = 1;

    // release reset the display bus from PMU.
    /* HCLKMASK and MBUSSTS registers are described in the MEMORY BUS datasheet.*/
    pMEMBUSCFG->HCLKMASK.bREG.DBUS = 1; //BIT9

    // video bus clock masking
    loop = 0;
    //while ( (pMEMBUSCFG->MBUSSTS.bREG.BITNUM2 | pMEMBUSCFG->MBUSSTS.bREG.BITNUM6) == 0);
    while ( (pMEMBUSCFG->MBUSSTS.bREG.DBUS0 | pMEMBUSCFG->MBUSSTS.bREG.DBUS1) == 0){
        if(loop++ > MAX_LCDC_WAIT_TIEM){
            printk("IO_PMU_ExitPowerDown_DBUS()->Error: video bus clock masking TIMEOUT!");
            return (HDMI_OUT_PW_FAILED);
    	}
    };
    
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI00 = nWakeupEnBackup0;
    pPMU->PMU_WKUP0.bREG.GPIO_HDMI01 = nWakeupEnBackup1;

    return (HDMI_OUT_PW_SUCCESS);
#endif
}


/*******************************************************************************
 *
 ******************************************************************************/

static ssize_t gpio_read( struct file* F, char *buf, size_t count, loff_t *f_pos )
{
    char buffer[50];
    volatile PGPIO pGPIO = (volatile PGPIO)tcc_p2v(HwGPIO_BASE);
    
    int temp = gpio_get_value(TCC_GPC(22));
    
    memset( buffer, 0, sizeof(buffer));
     
    sprintf( buffer, "hdmi_out_pw(v1.8): %i \n" , temp );
      
    count = sizeof( buffer );
       
    if( copy_to_user( buf, buffer, count ) )
    {
        return -EFAULT;
    }
        
    if( *f_pos == 0 )
    {
        *f_pos += 1;
        return count;
    }
    else
    {
        return 0;
    }
}
          
static ssize_t gpio_write( struct file* F, const char *buf, size_t count, loff_t *f_pos )
{
    int i;
    volatile PGPIO pGPIO = (volatile PGPIO)tcc_p2v(HwGPIO_BASE);
           
    printk(KERN_INFO "hdmi_out_pw device driver executing WRITE.\n");
            
    switch( buf[0] )
    {
        case '0':
            IO_PMU_ExitPowerDown_DBUS ();
            mdelay(25);
            gpio_set_value(TCC_GPC(22), 0);
            break;
             
        case '1':
            gpio_set_value(TCC_GPC(22), 1);
            mdelay(25);
            IO_PMU_EnterPowerDown_DBUS ();
            break;

        default:
            printk("hdmi_out_pw device driver: wrong option.\n");
            break;
    }
               
    return count;
}
                
static int gpio_open( struct inode *inode, struct file *file )
{
    return 0;
}
                 
static int gpio_close( struct inode *inode, struct file *file )
{
    return 0;
}
                  
static struct file_operations FileOps =
{
    .open         = gpio_open,
    .read         = gpio_read,
    .write         = gpio_write,
    .release     = gpio_close,
};
                   
static int init_gpio(void)
{
    init_result = register_chrdev( 0, "hdmi_out_pw", &FileOps );
                   
    if( 0 > init_result )
    {
        printk(KERN_ALERT "hdmi_out_pw device registration failed\n");
        return -1;
    }
    else
    {
        printk(KERN_ALERT "hdmi_out_pw major number is: %d\n",init_result);
        return 0;
    }
}
                     
void cleanup_gpio(void)
{
    unregister_chrdev( init_result, "hdmi_out_pw" );
    printk(KERN_ALERT "hdmi_out_pw device unregistered\n");
}
                       
module_init(init_gpio);
module_exit(cleanup_gpio);

MODULE_AUTHOR("idobany@dension.com");
MODULE_LICENSE("GPL");



