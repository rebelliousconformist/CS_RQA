/*
 * Road_Quality_Analyzer_main.c
 * 
 * Created: 11-Jun-15 7:10:33 PM
 * Author: Dipak Shetty
 * Function: Main program with the state machine implemented and the functions for calculating the road grade
 *			 The LCD display and ADC module are interfaced. The values are stored in a array of 500 at creation.
 * Connections: LCD and ADC connections, Button1- PC6, Button2- PC7, LED- PC5, Buzzer- PB7, Aref- 3.33V
 */ 

/*
Changelog-	
Date			Version		ChangeID		Changed by					Changes						Remarks
11.06.15		1.0			Created			Dipak Shetty
14.06.15		1.1			CH01			Subhajit Pande				EEPROM storage				Writing and reading values from EEPROM
*/

/*Include files*/
#include <avr/io.h>
#include "LCD_DISPLAY.h"
#include "ADC_Module.h"
#include "memory.h" /* CH01- The library file created by Chris Majda is used*/


/*Calibration values*/
# define POOR_ROAD  50
# define GOOD_ROAD  5
#define horizontal_movement_threshold 40
/*-----------------*/

/*Global variables*/
int Zero_G_XAxis;
int Zero_G_YAxis;
int One_G_ZAxis;
volatile uint16_t i=0;
/*CH01 begins*/
//int digitalInput_ZAxis_Array[500];
/*CH01 ends*/
int digital_x_axis;
int digital_y_axis;
int digital_z_axis;

/*Function declarations*/
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
	DDRC |= 0x10;								// LED pin
	PORTC |= 0xC0;								//Enabling pull up for SW1 and SW2
	PORTC &= 0b11001111;						// LED  OFF
	DDRB |= 0b10000000;							// Buzzer pin
	PORTB &= 0b01111111;						// Buzzer off
	inti_ADC();									//ADC intialization
	
    while(1)
    {
        switch(state)  // State case implementation using switch case
		{
			
		case 0:
				display_Start();
				i=0;
				if(keyPressAndRelease(1))
				{
					autocalibrate();			
					state=1;
				}
				break;
				 
		case 1: 
				display_DAQ();
				record_ADC_data();
				if(keyPressAndRelease(1))
					state=3;
				else if(keyPressAndRelease(2))
					state=4;
				else if(i>=7999)
					state=2;
				break;
		case 2: 
				display_Memory_Full();
				PORTB |= 0b10000000; // Buzzer ON
				if(keyPressAndRelease(1))
				{
					PORTB &= 0b01111111;
					state = 4;
				}
				break;
		case 3: 
				display_Pause();
				if(keyPressAndRelease(1))
					state= 1;
				break;
		case 4:
				display_Stop();
				PORTC &= 0b11101111; // Buzzer OFF
				if(keyPressAndRelease(1))
					state=6;
				else if (keyPressAndRelease(2))
					state = 5;
				break;
		case 5: 
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

/* This function is used to detect a button press of button1 and 2 
return- int 1  when the button is pressed and released else 0*/
uint8_t keyPressAndRelease(uint8_t button)
{
	button = button+5;
	if(!(PINC & (1<<button)))// button pressed
	{
		_delay_ms(50);
		if(!(PINC & (1<<button)))
		{
			
			return 1;
		}
		else
			return 0;
			
	}
	return 0;
}

/* The function is called during DAQ to record the data into the memory*/
void record_ADC_data()
{
	digital_x_axis= ADC_X();
	digital_y_axis= ADC_Y();
	digital_z_axis= ADC_Z();
	int16_t deviation_mean;
	uint8_t deviation_MSB;
	uint8_t deviation_LSB;
	
	/* Condition- if the movement in X-Y plane is greater than the threshold */
	if(digital_x_axis>(Zero_G_XAxis+horizontal_movement_threshold)||digital_x_axis<(Zero_G_XAxis-horizontal_movement_threshold)|| digital_y_axis>(Zero_G_YAxis+horizontal_movement_threshold)||digital_y_axis<(Zero_G_YAxis-horizontal_movement_threshold))
	 {
		 /* Condition- if the count of values recorded is less than the max memory space */
		 if(i<7999)			//CH01- from 499-7999
		 	 {
				PORTC |= 0b00100000; // LED ON
				  
			 	 if(digital_z_axis>One_G_ZAxis)
				  {
					  deviation_mean=digital_z_axis-One_G_ZAxis;
					
				  }
				  else
				  {
					  deviation_mean=One_G_ZAxis-digital_z_axis;
				  }
				  /*CH01 begins*/
				    deviation_LSB = deviation_mean & 0x0F;
				    deviation_MSB = deviation_mean >> 8;
			 	/* digitalInput_ZAxis_Array[i]=digital_z_axis-One_G_ZAxis;
			 	 else
			 	 digitalInput_ZAxis_Array[i]=One_G_ZAxis-digital_z_axis;*/
				 EEPROM_WriteByte(1,i,deviation_LSB);
				 EEPROM_WriteByte(2,i,deviation_MSB);
				 /*CH01 ends*/
			 	 _delay_ms(20);
			 	 i++;
				  
				 PORTC &= 0b11011111; // LED OFF
				 
		 	 }
	 }
	 
}
/* This function is used to record the ADC data in global variables from the 3 channels at the beginning of the DAQ*/
void autocalibrate()
{
	Zero_G_XAxis=ADC_X();
	Zero_G_YAxis=ADC_Y();
	One_G_ZAxis=ADC_Z();
}

/* This function is used to calculate the road grade
return- The road grade from range 0-5*/
double calculate_result()
{
	/*CH01 begins*/
	//double result= getAverage(digitalInput_ZAxis_Array,i);		
	double result= getAverage(i);
	/*CH01 ends*/
 
}


/* CH01 begins*/
/*
double getAverage(int arr[], int size)
{
	int    i;
	double avg;
	double sum=0;

	for (i = 0; i < size; ++i)
	{
		sum += arr[i];
	}

	avg = sum / size;

	return avg;
}
*/

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
/* CH01 ends*/
