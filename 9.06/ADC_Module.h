/*
 * LCD_DISPLAY.h 
 *
 * Created: 04-Jun-15 8:20:48 PM
 * Author: Purva Joshi
 * Function: This header file is created to display different messages on LCD display at every state of the program
 * Connection: DATA: PD0- PD7, RS- PC4, R/W- PC3, E- PC2
 */
/*
Changelog-

Date			Version		ChangeID		Changed by			Changes					Remarks 
04.06.2015		1.0			Created			Purva Joshi							
*/

#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_
#include "LCD.h"  /* The library file created by Chris Majda is used*/

void display_Start();
void display_DAQ();
void display_Pause();
void display_Memory_Full();
void display_Stop();
void display_Result(double);

/* Start Screen display*/
void display_Start()
{
	Initialize_LCD();
	GotoLCD_Location(1,1);
	Send_String("Start DAQ      ");
	
	GotoLCD_Location(1,2);
	Send_String("1: Acquire Data_");
}

/* Display during DAQ*/
void display_DAQ()
{
	GotoLCD_Location(1,1);
	Send_String("1: Pause DAQ    ");
	
	GotoLCD_Location(1,2);
	Send_String("2: Stop DAQ_    ");
}
/* Pause screen display*/
void display_Pause()
{
	GotoLCD_Location(1,1);
	Send_String("DAQ Paused      ");
	
	GotoLCD_Location(1,2);
	Send_String("1: Resume_     ");
}

/*Memory full display*/
void display_Memory_Full()
{
	GotoLCD_Location(1,1);
	Send_String("Memory Full!!! ");
	
	GotoLCD_Location(1,2);
	Send_String("1: Stop DAQ_    ");
}

/*Stop Display*/
void display_Stop()
{
	GotoLCD_Location(1,1);
	Send_String("1. Reset        ");
	
	GotoLCD_Location(1,2);
	Send_String("2: Analyze Data_");
}

/*Result display
Parameter- grade: Road grade calculated in main function*/
void display_Result(double grade)
{
	Send_Command(0x01);
	_delay_ms(2);
	GotoLCD_Location(1,1);
	Send_String("Road Grade: ");
	
	GotoLCD_Location(12,1);
	Send_Double(grade, 4, 2);
	
	GotoLCD_Location(1,2);
	Send_String("1: Restart");
	
}



#endif /* LCD_DISPLAY_H_ */
