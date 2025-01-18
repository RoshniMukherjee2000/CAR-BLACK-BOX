#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H


	
#define MAX_ROW				3
#define MAX_COL				4

#define STATE_CHANGE				1
#define LEVEL_CHANGE				0
#define MATRIX_KEYPAD_PORT			PORTB
#define ROW3					PORTBbits.RB7
#define ROW2					PORTBbits.RB6
#define ROW1					PORTBbits.RB5
#define COL4					PORTBbits.RB4
#define COL3					PORTBbits.RB3
#define COL2					PORTBbits.RB2
#define COL1					PORTBbits.RB1


#define MK_SW1					1
#define MK_SW2					2
#define MK_SW3					3
#define MK_SW4					4
#define MK_SW5					5
#define MK_SW6					6
#define MK_SW7					7
#define MK_SW8					8
#define MK_SW9					9
#define MK_SW10				10
#define MK_SW11				11
#define MK_SW12				12

#define ALL_RELEASED	0xFF

#define HI				1
#define LO				0

void init_matrix_keypad(void);
unsigned char scan_key(void);
unsigned char read_switches(unsigned char detection_type);

#endif
