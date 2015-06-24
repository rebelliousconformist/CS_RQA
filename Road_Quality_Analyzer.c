/*
 * Road_Quality_Analyzer.c
 *
 * Created: 03-Jun-15 7:10:33 PM
 *  Author: Optimus
 */ 


#include <avr/io.h>
#include "LCD_DISPLAY.h"
#include "ADC_Module.h"
#include "memory.h"
#include "Seven_Segment.h"

/*Calibration values*/
int Zero_G_XAxis;
int Zero_G_YAxis;
int One_G_ZAxis;

# define POOR_ROAD  50
# define GOOD_ROAD  0

#define horizontal_movement_threshold 40
/*-----------------*/

volatile uint16_t i=0;
//int digitalInput_ZAxis_Array[500];
int digital_x_axis;
int digital_y_axis;
int digital_z_axis;

double calculate_result();
double getAverage( int );
uint8_t keyPressAndRelease(uint8_t );
void record_ADC_data();
void autocalibrate();
double map(double, double , double , double , double );
int main(void)
{
	int state= 0;
	double result;
	DDRC |= 0x30; // LED and buzzer
	PORTC |= 0xC0; //Enabling pull up for SW1 and SW2
	
	PORTC &= 0b11001111; // LED and Buzzer OFF
	DDRB |= 0b10000000;
	PORTB &= 0b01111111;
	inti_ADC();
	
    while(1)
    {
        switch(state)
		{
			
		case 0:
				display_Start();
				i=0;
				if(keyPressAndRelease(1))
				{
					intialize_7Segment();
					autocalibrate();
					state=1;
				}
				break;
				 
		case 1: 
				display_DAQ();
				record_ADC_data();
				display_4_digit(i);
				if(keyPressAndRelease(1))
					state=3;
				else if(keyPressAndRelease(2))
					state=4;
				else if(i>=7999)
					state=2;
				break;
		case 2: 
				display_4_digit(i);
				display_Memory_Full();
				PORTB |= 0b10000000; // Buzzer ON
				if(keyPressAndRelease(1))
				{
					PORTB &= 0b01111111;
					state = 4;
				}
				break;
		case 3: 
				display_4_digit(i);
				display_Pause();
				if(keyPressAndRelease(1))
					state= 1;
				break;
		case 4:
				display_4_digit(i);
				display_Stop();
				PORTC &= 0b11101111; // Buzzer OFF
				if(keyPressAndRelease(1))
					state=6;
				else if (keyPressAndRelease(2))
					state = 5;
				break;
		case 5: 
				display_4_digit(i);
				result=calculate_result();
				display_Result(result);
				while (!(keyPressAndRelease(1)));
				state=6;
				break;
		case 6:
				i=0;
				state=0;
				break;
		}
    }
}

uint8_t keyPressAndRelease(uint8_t button)
{
	button = button+5;
	if(!(PINC & (1<<button)))// button pressed
	{
		_delay_ms(50);
		if(!(PINC & (1<<button)))
		{
			while(!(PINC & (1<<button))); // wait for button release
			return 1;
		}
		else
			return 0;
			
	}
	return 0;
}

void record_ADC_data()
{
	digital_x_axis= ADC_X();
	digital_y_axis= ADC_Y();
	digital_z_axis= ADC_Z();
	int16_t deviation_mean;
	uint8_t deviation_MSB;
	uint8_t deviation_LSB;
	
	if(digital_x_axis>(Zero_G_XAxis+horizontal_movement_threshold)||digital_x_axis<(Zero_G_XAxis-horizontal_movement_threshold)|| digital_y_axis>(Zero_G_YAxis+horizontal_movement_threshold)||digital_y_axis<(Zero_G_YAxis-horizontal_movement_threshold))
	  
	 {
		 if(i<7999)
		 	 {
				PORTC |= 0b00100000; // LED ON
				  //PORTC |= 1<<5;
				  
			 	 if(digital_z_axis>One_G_ZAxis)
				  {
					  deviation_mean=digital_z_axis-One_G_ZAxis;
					
				  }
				  else
				  {
					  deviation_mean=One_G_ZAxis-digital_z_axis;
				  }
				    deviation_LSB = deviation_mean & 0x0F;
				    deviation_MSB = deviation_mean >> 8;
			 	/* digitalInput_ZAxis_Array[i]=digital_z_axis-One_G_ZAxis;
			 	 else
			 	 digitalInput_ZAxis_Array[i]=One_G_ZAxis-digital_z_axis;*/
				 EEPROM_WriteByte(1,i,deviation_LSB);
				 EEPROM_WriteByte(2,i,deviation_MSB);
			 	 _delay_ms(20);
			 	 i++;
				  
				 PORTC &= 0b11011111; // LED OFF
				 
		 	 }
	 }
	 

	
}

void autocalibrate()
{
	Zero_G_XAxis=ADC_X();
	Zero_G_YAxis=ADC_Y();
	One_G_ZAxis=ADC_Z();
}

double calculate_result()
{
	//double result= getAverage(digitalInput_ZAxis_Array,i);
	double result= getAverage(i);
	
	result = map(result, POOR_ROAD, GOOD_ROAD, 0,5 );
	if(result < 0)
		result =0;
	return result;
 
}
double map(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double getAverage(int size)
{
	int  j;
	double avg;
	double sum=0;
	int LSB_Data,MSB_Data,Data;

	for (j = 0; j < size; ++j)
	{
		LSB_Data=EEPROM_ReadByte(1,j);
		MSB_Data=EEPROM_ReadByte(2,j);
		Data= (MSB_Data <<8) | LSB_Data;
		sum += Data;
	}

	avg = (double)sum / size;

	return avg;
}