/*
 * Seven_Segment.h
 *
 * Created: 12-Jun-15 10:23:24 AM
 *  Author: Optimus
 */ 


#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

#include <util/delay.h>
uint8_t gethexcode(uint8_t );
void display_4_digit(uint16_t);
void intialize_7Segment();

void intialize_7Segment()
{
	DDRB = 0xFF;
	DDRA = 0xF0;
}



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