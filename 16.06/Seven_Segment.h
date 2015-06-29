/*
 * Seven_Segment.h
 *
 * Created: 16-Jun-15 10:23:24 AM
 * Author: Nachiket Bapat
 */ 
/*
Function: To display a 4-digit value on the 7 segment display
Connections: A-G: PB0-PB6, DG1- DG4: PA4-PA7
*/
/*
Changelog-
Date			Version		ChangeID		Changed by				Changes		Remarks
16.06.15		1.0			Created			Nachiket Bapat
*/

#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

#include <util/delay.h>
uint8_t gethexcode(uint8_t );
void display_4_digit(uint16_t);
void intialize_7Segment();

/*Initialization of pins*/
void intialize_7Segment()
{
	DDRB = 0xFF;
	DDRA = 0xF0;
}

/*displaying the value
Parameter- value: 4 digit value*/
void display_4_digit(uint16_t value)
{
	uint8_t i, hex_code,digit_value;
	for(i=0;i<4;i++)
	{
		
		digit_value = value % 10;
		hex_code=gethexcode(digit_value);
		PORTA = (0b10000000>>i);
		PORTB = hex_code;
		_delay_ms(1);
		PORTA = 0x00;
		PORTB= 0x00;
		value /= 10;
	}
	
	
}
/*getting the hex code for the digit to send to 7-segment
Parameter- value: the digit whose hex code is to be found out
return- the hex code
*/
uint8_t gethexcode(uint8_t digit_value)
{
	switch(digit_value)
	{
		case 0: return 0x3F;
		case 1: return 0x06;
		case 2: return 0x5B;
		case 3: return 0x4F;
		case 4: return 0x66;
		case 5: return 0x6D;
		case 6: return 0x7D;
		case 7: return 0x07;
		case 8: return 0x7F;
		case 9: return 0x6F;
		
	}
	return 0;
}

#endif /* SEVEN_SEGMENT_H_ */
