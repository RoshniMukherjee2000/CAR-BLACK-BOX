#include <xc.h>
#include "adc.h"


void init_adc(void)
{
	/* Selecting right justified ADRES Registers order */
	ADFM = 1;

	/* 
	 * Acqusition time selection bits 
	 * Set for 8 Tad
	 */
	ACQT2 = 1;
	ACQT1 = 0;
	ACQT0 = 0;

	/*
	 * Selecting the conversion clock of Fosc / 32 -> 1.6usecs -> 1Tad
	 * Our device frequency is 20 MHz
	 */
	ADCS0 = 0;
	ADCS1 = 1;
	ADCS2 = 0;

	/* Stop the conversion to start with */
	GODONE = 0;

	

	/* Voltage reference bit as VSS */
	VCFG1 = 0;
	/* Voltage reference bit as VDD */
	VCFG0 = 0;

	/* Just clearing the ADRESH & ADRESL registers, for time pass */
	ADRESH = 0;
	ADRESL = 0;

	/* Turn ON the ADC module */
	ADON = 1;
}

unsigned short read_adc(unsigned char channel)
{
	unsigned short reg_val;

	/*select the channel*/
	ADCON0 =(unsigned char) ((ADCON0 & 0xC3) | (channel << 2));

	/* Start the conversion */
	GO = 1;
	while (GO);
	reg_val =(unsigned short) ((ADRESH << 8) | ADRESL); 

	return reg_val;
}