/******************************************************************************
 * file: BC_hw_flash.c
 * description: flash handler
 *              
 ******************************************************************************/
/******************************************************************************
 * headers
 ******************************************************************************/
#include "hw.h"

/******************************************************************************
 * definitions
 ******************************************************************************/

//DEBUG_FOR_DEV definition is needed
#undef DEBUG_HASH 

/******************************************************************************
 * declarations
 ******************************************************************************/
static const char id[] = "$Id: BC_hw_flash.c 892 2012-02-25 23:21:52Z dobany_imre $";

static volatile uint8_t spibuff[16 + 512 + 16];	        // 528 bytes for the "log" flash + 16 for any cmd





/*******************************************************************************
 * prototypes
 ******************************************************************************/
int sreg(void)
{
int	r;
//int	last_tick = 0;

	r = AT91C_BASE_SPI->SPI_RDR & 0xff;  // should be an 0xff
	#if defined(FLASH_PDC)
	spibuff[0] = 0xd7;
	spibuff[1] = 0xff;
	spibuff[2] = 0xff;
	AT91C_BASE_SPI->SPI_TPR = (unsigned int) spibuff;
	AT91C_BASE_SPI->SPI_TCR = 3;
	AT91C_BASE_SPI->SPI_RPR = (unsigned int) spibuff;
	AT91C_BASE_SPI->SPI_RCR = 3;
	AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
	r = AT91C_BASE_SPI->SPI_SR;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX))
			;
	//while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDRX))
			;
	#if 0
	debug.putch('s');
	//debug.putbyte(spibuff[0]);
	debug.putbyte(spibuff[1]);
	//debug.putbyte(spibuff[2]);
	debug.putch(' ');
	#endif

	tick1 = 0;
	while(tick1 < 8)
			;
	return spibuff[1];
	
	#else  // not FLASH_PDC
	spi_putbyte(0xd7);
	r = spi_getbyte();
	spi_getbyte();
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
	return r;
	#endif
}

/*******************************************************************************
 *
 ******************************************************************************/
flash_operation_err_t FLASH_wr(unsigned int flashnum, unsigned int page, uint8_t *p, const int n)
{
int	r;
#if 0
int	j;
#endif
int	i;


	//100 usec wait for sure that prev _cs was finished
	//measured: min 500nsec needed
	while(!npcs_to_tick1)//1 * 100 usec
		;	


	

	if(flashnum == 2){
		#if defined(__BC2__)
		AT91C_BASE_PIOA->PIO_SODR = AT91C_PIO_PA0; //no reset FLASH
		#endif
		AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
									AT91C_SPI_MODFDIS |  // mode fault disable
									SPI_NPCSX_FLASH2 |
									SPI_CS_DELAY;	
	}
	else if(flashnum == 1){
		#if defined(__BC2__)
		AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA0; //reset FLASH
		??????flash kezelest megirni
		#endif
		AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
									AT91C_SPI_MODFDIS |  // mode fault disable
									SPI_NPCSX_FLASH1 |
									SPI_CS_DELAY;
	}
	else{
		debug.puttxt("FLASH BANK SELECT ERROR!");
		return E_FLASH_BADMAGIC;
	}
	
	
	flash_to_tick = 0;
	while(((r = sreg()) & 0xbf) != 0xac)
		if(flash_to_tick > FLASH_TIMEOUT){
			debug_puttxt("write_sector() E_FLASH_STATUSFAIL 1");
			return E_FLASH_STATUSFAIL;
		}						
	
		
		
	r = AT91C_BASE_SPI->SPI_RDR & 0xff;  // should be an 0xff


	//AT91F_AIC_DisableIt(AT91C_BASE_AIC, AT91C_ID_TC0);
	//AT91F_AIC_DisableIt(AT91C_BASE_AIC, AT91C_ID_TC1);
	//AT91F_AIC_DisableIt(AT91C_BASE_AIC, AT91C_ID_SYS);

	
	//AT45DB161D.pdf: 22.2 Buffer to Main Memory Page Program (Data from Buffer Programmed into Flash Page)
	//A20-A9 + 9 DON'T CARE BITS
	//CMD [XXPA11-6] [PA5-0,XX] [XXXXXXXX]
	//0xXX XX YY YY
	spi_putbyte(0x82);
	spi_putbyte(page >> 6);
	spi_putbyte(page << 2);
	spi_putbyte(0);
	for(i = 0; i != n; i++){
		AT91C_BASE_SPI->SPI_TDR = p[i];
		while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY))
				;
		r = AT91C_BASE_SPI->SPI_RDR & 0xff;
	}
		
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
	npcs_to_tick1 = 0;	

	//AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_TC0);
	//AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_TC1);
	//AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_SYS);

	return E_FLASH_OK;
}

/*******************************************************************************
 *
 ******************************************************************************/
flash_operation_err_t FLASH_rd(unsigned int flashnum, unsigned int page, void *p, const int n)
{
int	r;
#if 0
int	j;
#endif
int	i;

	//100 usec wait for sure that prev _cs was finished
	//measured: min 500nsec needed
	while(!npcs_to_tick1)//1 * 100 usec
		;	


	if(flashnum == 2){
		#if defined(__BC2__)
		AT91C_BASE_PIOA->PIO_SODR = AT91C_PIO_PA0; //no reset FLASH
		#endif
		AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
									AT91C_SPI_MODFDIS |  // mode fault disable
									SPI_NPCSX_FLASH2 |
									SPI_CS_DELAY;	
	}
	else if(flashnum == 1){
		#if defined(__BC2__)
		AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA0; //reset FLASH
		??????flash kezelest megirni
		#endif
		AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
									AT91C_SPI_MODFDIS |  // mode fault disable
									SPI_NPCSX_FLASH1 |
									SPI_CS_DELAY;
	}
	else{
		debug.puttxt("FLASH BANK SELECT ERROR!");
		return E_FLASH_BADMAGIC;
	}
		


	flash_to_tick = 0;
	while(((r = sreg()) & 0xbf) != 0xac)
		if(flash_to_tick > FLASH_TIMEOUT){
			debug_puttxt("read_sector_n() E_FLASH_STATUSFAIL 2");
			return E_FLASH_STATUSFAIL;
		}						
	
	spi_putbyte(0xd2);
	spi_putbyte(page >> 6);
	spi_putbyte(page << 2);
	spi_putbyte(0);
	spi_putbyte(0);
	spi_putbyte(0);
	spi_putbyte(0);
	spi_putbyte(0);
	for(i = 0; i != n; i++)
		((uint8_t *) p)[i] = spi_getbyte();
	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
	npcs_to_tick1 = 0;

	
	return E_FLASH_OK;
}


/***********************************************************************
 *  function: FLASH_clear(void)
 *  desc.: 	both FLASHes cleared approx. 20sec/16 Mbit FLASH
 ***********************************************************************/
flash_operation_err_t FLASH_clear(void)
{
flash_operation_err_t r;
uint32_t i;

	
	debug.puttxt("flash_clear();");
	
	for(i = 0; i < FLASH1_PAGES / FLASH1_PAGESPBLOCK; i++){
		/***********************************************************************
		* 
		***********************************************************************/
		//debug.puthex("flash2: ", i);
		
		
		//100 usec wait for sure that prev _cs was finished
		//measured: min 500nsec needed
		while(!npcs_to_tick1)//1 * 100 usec
			;	

		#if defined(__BC2__)
		AT91C_BASE_PIOA->PIO_SODR = AT91C_PIO_PA0; //no reset FLASH
		#endif
		AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
									AT91C_SPI_MODFDIS |  // mode fault disable
									SPI_NPCSX_FLASH2 |
									SPI_CS_DELAY;	

		flash_to_tick = 0;
		while(((r = sreg()) & 0xbf) != 0xac){
			WATCHDOG_RESET;
			if(flash_to_tick > FLASH_TIMEOUT){
				debug_puttxt("FLASH_clear() return E_FLASH_STATUSFAIL 2");
				return E_FLASH_STATUSFAIL;
			}						
		}
		#if (FLASH1_BYTEPPAGE != 528)
		#warning flash cmd error!
		#endif
		//528 byte/page cmd
		//AT45DB161D.pdf: Table 15-7. Detailed Bit-level Addressing Sequence for Standard DataFlash Page Size (528 Bytes)
		spi_putbyte(0x50);
		spi_putbyte(i >> 3);
		spi_putbyte(i << 5);
		spi_putbyte(0);


		AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
		npcs_to_tick1 = 0;

		/***********************************************************************
		*
		***********************************************************************/
		//debug.puthex("flash1: ", i);
		//100 usec wait for sure that prev _cs was finished
		//measured: min 500nsec needed
		while(!npcs_to_tick1)//1 * 100 usec
			;	


		#if defined(__BC2__)
		AT91C_BASE_PIOA->PIO_CODR = AT91C_PIO_PA0; //reset FLASH
		??????flash kezelest megirni
		#endif
		AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
									AT91C_SPI_MODFDIS |  // mode fault disable
									SPI_NPCSX_FLASH1 |
									SPI_CS_DELAY;


		flash_to_tick = 0;
		while(((r = sreg()) & 0xbf) != 0xac){
			WATCHDOG_RESET;
			if(flash_to_tick > FLASH_TIMEOUT){
				debug_puttxt("FLASH_clear() return E_FLASH_STATUSFAIL 2");
				return E_FLASH_STATUSFAIL;
			}						
		}
		#if (FLASH1_BYTEPPAGE != 528)
		#warning flash cmd error!
		#endif
		//528 byte/page cmd
		//AT45DB161D.pdf: Table 15-7. Detailed Bit-level Addressing Sequence for Standard DataFlash Page Size (528 Bytes)
		spi_putbyte(0x50);
		spi_putbyte(i >> 3);
		spi_putbyte(i << 5);
		spi_putbyte(0);


		AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_LASTXFER;
		npcs_to_tick1 = 0;


	}

	debug.puthex("flash_clear() E_FLASH_OK:", E_FLASH_OK);
	return E_FLASH_OK;
}
