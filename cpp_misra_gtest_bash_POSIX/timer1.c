

#if 0

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");

static struct timer_list my_timer;

void my_timer_callback( unsigned long data )
{
  printk( "my_timer_callback called (%ld).\n", jiffies );
}

int init_module( void )
{
  int ret;

  printk("Timer module installing\n");

  // my_timer.function, my_timer.data
  setup_timer( &my_timer, my_timer_callback, 0 );

  printk( "Starting timer to fire in 200ms (%ld)\n", jiffies );
  ret = mod_timer( &my_timer, jiffies + msecs_to_jiffies(200) );
  if (ret) printk("Error in mod_timer\n");

  return 0;
}

void cleanup_module( void )
{
  int ret;

  ret = del_timer( &my_timer );
  if (ret) printk("The timer is still in use...\n");

  printk("Timer module uninstalling\n");

  return;
}




#else
/* SingleDigitNumericDisplay :sdnd
 * linux/drivers/gpio/sdnd.c
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
  

#include <linux/timer.h>

  
static int init_result;
chartable
{'0', {0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_)}}
1
2
3
4
5
6
7
8
9
A
C
E
F
H
I
h
n
-
=
^

DispBuff0 
DispBuff1
DispBuffConvertedCommon
DispDp
DispFreshTime

 
 


/*******************************************************************************
 *
 ******************************************************************************/

static ssize_t sdnd_read( struct file* F, char *buf, size_t count, loff_t *f_pos )
{
    char buffer[50];

    memset( buffer, 0, sizeof(buffer));
     
    sprintf( buffer, "sdnd(v1.0)/DispNum:%u/DispBuff:%u/DispFresh:%u\n" ,DispNum, DispBuff, DispFresh);
      
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
          
static ssize_t sdnd_write( struct file* F, const char *buf, size_t count, loff_t *f_pos )
{
unsigned int i;           
//chk szring db
//elso masodik sw
//buff0 buff1  konvert to buff_kommon
//



    printk(KERN_INFO "sdnd device driver executing WRITE.\n");
            
    if( buf[0]  < '0' || buf[0]  > '9'){
        printk("sdnd device driver: wrong option.\n");
        return 0;
    }

    for(i = 0; i < 2*8; i++){
        gpio_set_value(TCC_GPC(SDND_CLK), 0);
        gpio_set_value(TCC_GPC(SDND_DATA), 0);
        if(DispBuffConvertedCommon[i] & ){
            gpio_set_value(TCC_GPC(SDND_DATA), 1);
        }

        gpio_set_value(TCC_GPC(SDND_CLK), 1);
        gpio_set_value(TCC_GPC(SDND_CLK), 0);
    }

               
    return count;
}
                
static int sdnd_open( struct inode *inode, struct file *file )
{
    return 0;
}
                 
static int sdnd_close( struct inode *inode, struct file *file )
{
    return 0;
}
                  
static struct file_operations FileOps =
{
    .open         = sdnd_open,
    .read         = sdnd_read,
    .write        = sdnd_write,
    .release      = sdnd_close,
};
                   
//SingleDigitNumericDisplay :sdnd
static int init_sdnd(void)
{
    init_result = register_chrdev( 0, "sdnd", &FileOps );
                   
    if( 0 > init_result )
    {
        printk(KERN_ALERT "sdnd device registration failed\n");
        return -1;
    }
    else
    {
        printk(KERN_ALERT "sdnd major number is: %d\n",init_result);
        return 0;
    }
}
                     
void cleanup_sdnd(void)
{
    unregister_chrdev( init_result, "sdnd" );
    printk(KERN_ALERT "sdnd device unregistered\n");
}
                       
module_init(init_sdnd);
module_exit(cleanup_sdnd);

MODULE_AUTHOR("idobany@dension.com");
MODULE_LICENSE("GPL");








#endif