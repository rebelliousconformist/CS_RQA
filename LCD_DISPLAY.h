/*
 * LCD_DISPLAY.h
 *
 * Created: 09-Jun-15 8:20:48 PM
 *  Author: Optimus
 */ 


#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_
#include "LCD.h"

void display_Start();
void display_DAQ();
void display_Pause();
void display_Memory_Full();
void display_Stop();
void display_Result(double);


void display_Start()
{
	Initialize_LCD();
	GotoLCD_Location(1,1);
	Send_String("Start DAQ      ");
	
	GotoLCD_Location(1,2);
	Send_String("1: Acquire Data_");
}

void display_DAQ()
{
	GotoLCD_Location(1,1);
	Send_String("1: Pause DAQ    ");
	
	GotoLCD_Location(1,2);
	Send_String("2: Stop DAQ_    ");
}

void display_Pause()
{
	GotoLCD_Location(1,1);
	Send_String("DAQ Paused      ");
	
	GotoLCD_Location(1,2);
	Send_String("1: Resume_     ");
}

void display_Memory_Full()
{
	GotoLCD_Location(1,1);
	Send_String("Memory Full!!! ");
	
	GotoLCD_Location(1,2);
	Send_String("1: Stop DAQ_    ");
}

void display_Stop()
{
	GotoLCD_Location(1,1);
	Send_String("1. Reset        ");
	
	GotoLCD_Location(1,2);
	Send_String("2: Analyze Data_");
}

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