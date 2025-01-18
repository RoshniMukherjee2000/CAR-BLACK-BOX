#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);  //TO TRANSMIT 1BYTE OF DATA
int puts(const char *s);//to transmit a string
unsigned char getch(void);//to receive 1 byte
unsigned char getche(void);//receive 1 byte,transmit 1 byte

#endif