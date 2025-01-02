/******************************************************************************
 * file: BC_core_io_print.c
 * description: printing handler
 *              
 ******************************************************************************/
/******************************************************************************
 * headers
 ******************************************************************************/
#include "hw.h"



/******************************************************************************
 * definitions
 ******************************************************************************/



/******************************************************************************
 * declarations
 ******************************************************************************/
static const char id[] = "$Id: BC_hw_spi.c 894 2012-02-29 09:18:29Z dobany_imre $";


/******************************************************************************
 * prototypes
 ******************************************************************************/



/*******************************************************************************************************
 * spi basic FUNCTIONS
 *******************************************************************************************************/
void spi_ini(unsigned int type)
{
	if (type & SPI_INI_START){
		AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_SPI;  // clock enable for the SPI
	}


	if(type & SPI_INI_RESET){
		AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SWRST;  // reset the SPI engine

		sleeptick = 0;
		while(sleeptick < 40);	                // 400ms wait
			;
	}
	

	
	//AT91C_BASE_SPI->SPI_TDR = 0;
	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								0x000f0000 |  // no nPCSn active
								SPI_CS_DELAY;



	AT91C_BASE_SPI->SPI_CSR[CSR_CO] = SPI_CSR_CO;
	AT91C_BASE_SPI->SPI_CSR[CSR_FLASH1] = SPI_CSR_FLASH1;
	
	#if defined(SPI_MODE_TEST)
	AT91C_BASE_SPI->SPI_CSR[CSR_SD] = 0x7808 | (spi_mode & 3);  // SD		clk ~ 400kHz
				// was 0x780a, now playing...
	#else
	if(type & SPI_INI_SD_SLOW)
		AT91C_BASE_SPI->SPI_CSR[CSR_SD] = SPI_CSR_SD_SLOW;//200khz
	else
		AT91C_BASE_SPI->SPI_CSR[CSR_SD] = SPI_CSR_SD;  // SD		clk = MCK / 2 = 25mhz
	#endif
	
	#if defined(__BC2__)
	AT91C_BASE_SPI->SPI_CSR[CSR_ETH] = SPI_CSR_ETH;
	#else
	AT91C_BASE_SPI->SPI_CSR[CSR_FLASH2] = SPI_CSR_FLASH2;
	#endif

	AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;
}

//////////////////////////////////
int spi_putbyte(int n)
{
int	r;

	#if defined(DEBUG_SD)
	debug.putch('>');
	debug.putbyte(n);
	debug.putch('<');
	#endif
	AT91C_BASE_SPI->SPI_TDR = n & 0xff;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY))
			;
	#if defined(DEBUG_SD)
	if(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_OVRES)
		debug.putch('o');
	#endif
	r = AT91C_BASE_SPI->SPI_RDR & 0xff;
	#if defined(DEBUG_SD)
	debug.putbyte(r);
	#endif
	return r;
}

//////////////////////////////////
int spi_getbyte()
{
int	r;

	AT91C_BASE_SPI->SPI_TDR = 0xff;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY))
			;
	#if defined(DEBUG_SD)
	if(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_OVRES)
		debug.putch('o');
	#endif
	r = AT91C_BASE_SPI->SPI_RDR & 0xff;
	#if defined(DEBUG_SD)
	debug.putbyte(r);
	#endif
	return r;
}


void coproc_spi_cs_enable(void)
{ 
	#if 0
	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								SPI_NPCSX_FLASH1 |
								SPI_CS_DELAY;
	tick1 = 0;
	while(tick1 < 8)
			;
	#endif
	
	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								SPI_NPCSX_COPROC |
								SPI_CS_DELAY;

}


void coproc_spi_cs_disable(void)
{ 
	AT91C_BASE_SPI->SPI_MR =	AT91C_SPI_MSTR |  // master mode
								AT91C_SPI_MODFDIS |  // mode fault disable
								0x000f0000 |
								SPI_CS_DELAY;

}


int coproc_spi_send(void)
{
unsigned int spi_sr;	
unsigned int dummy;	
	//debug.memdump("b spi_armcmd: ", (uint8_t*)&spi_armcmd, sizeof(spi_armcmd));
	//debug.memdump("b spi_response: ",(uint8_t*)&spi_response, sizeof(spi_response));

	spi_sr = AT91C_BASE_SPI->SPI_SR;
	//debug.puthex(" spi_sr b:", spi_sr);
	dummy = AT91C_BASE_SPI->SPI_RDR;

	spi_to_tick = 0;

	AT91C_BASE_SPI->SPI_TPR = (unsigned int) &spi_armcmd;
	AT91C_BASE_SPI->SPI_TCR = sizeof(spi_armcmd);
	AT91C_BASE_SPI->SPI_RPR = (unsigned int) &spi_response;
	AT91C_BASE_SPI->SPI_RCR = sizeof(spi_response);

	
	//spi_sr = AT91C_BASE_SPI->SPI_SR;
	//debug.puthex(" spi_sr b:", spi_sr);
	
	
	AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_ENDTX)){
		if(spi_to_tick > SPI_TIMEOUT){
			debug.puttxt("send_spi TO1");
			return 1;
		}
	}
	while(! (AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY)){
		if(spi_to_tick > SPI_TIMEOUT){
			debug.puttxt("send_spi TO2");
			return 1;
		}
	}
	AT91C_BASE_SPI->SPI_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;
	
	spi_sr = AT91C_BASE_SPI->SPI_SR;
	//debug.puthex(" spi_sr a:", spi_sr);
	//debug.memdump("a spi_armcmd: ", (uint8_t*)&spi_armcmd, sizeof(spi_armcmd));
	//debug.memdump("a spi_response: ", (uint8_t*)&spi_response, sizeof(spi_response));
	return 0;
	
}

