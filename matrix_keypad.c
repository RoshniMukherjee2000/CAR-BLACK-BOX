#include <xc.h>
#include "matrix_keypad.h"

void init_matrix_keypad(void)
{
	/* Config PORTB as digital */
	ADCON1 = 0x0F;

	/* Set Rows (RB7 - RB5) as Outputs and Columns (RB4 - RB1) as Inputs */
	TRISB = 0x1E;

	/* Set PORTB input as pull up for columns */
	RBPU = 0;

	MATRIX_KEYPAD_PORT = MATRIX_KEYPAD_PORT | 0xE0;
}

unsigned char scan_key(void)
{
	ROW1 = LO;//SETTING ROW1 AS 0
	ROW2 = HI;//SETTING ROW2 AS 1
	ROW3 = HI;//SETTING ROW3 AS 1

	if (COL1 == LO)//IF COLOUMN1 IS 0
	{
		return 1;//SWITCH1 IS PRESSED
	}
	else if (COL2 == LO)
	{
		return 4;//SWITCH4 IS PRESSED
	}
	else if (COL3 == LO)
	{
		return 7;//SWITCH7 IS PRESSED
	}
	else if (COL4 == LO)
	{
		return 10;//SWITCH10 IS PRESSED
	}

	ROW1 = HI;//SETTING ROW1 AS 1
	ROW2 = LO;//SETTING ROW2 AS 0
	ROW3 = HI;//SETTING ROW3 AS 1

	if (COL1 == LO)
	{
		return 2;//SWITCH2 IS PRESSED
	}
	else if (COL2 == LO)
	{
		return 5;//SWITCH5 IS PRESSED
	}
	else if (COL3 == LO)
	{
		return 8;//SWITCH8 IS PRESSED
	}
	else if (COL4 == LO)
	{
		return 11;//SWITCH11 IS PRESSED
	}

	ROW1 = HI;
	ROW2 = HI;
	ROW3 = LO;
	/* TODO: Why more than 2 times? */
	ROW3 = LO;

	if (COL1 == LO)
	{
		return 3;
	}
	else if (COL2 == LO)
	{
		return 6;
	}
	else if (COL3 == LO)
	{
		return 9;
	}
	else if (COL4 == LO)
	{
		return 12;
	}

	return 0xFF;
}

unsigned char read_switches(unsigned char detection_type)
{
	static unsigned char once = 1, key;

	if (detection_type == STATE_CHANGE)
	{
		key = scan_key();
		if(key != 0xFF && once  )
		{
			once = 0;
			return key;
		}
		else if(key == 0xFF)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL_CHANGE)
	{
		return scan_key();
	}

	return 0xFF;
}
