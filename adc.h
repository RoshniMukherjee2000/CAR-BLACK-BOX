#ifndef ADC_H
#define ADC_H

#define CHANNEL0		0x00
#define CHANNEL1		0x01
#define CHANNEL2		0x02
#define CHANNEL3		0x03
#define CHANNEL4		0x04
#define CHANNEL5		0x05
#define CHANNEL6		0x06
#define CHANNEL7		0x07
#define CHANNEL8		0x08
#define CHANNEL9		0x09
#define CHANNEL10		0x0A

void init_adc(void);
unsigned short read_adc(unsigned char channel);

#endif