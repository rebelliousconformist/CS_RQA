/* EEPROM library file created by Chris Majda*/

#ifndef memory 
#define memory

#define F_CPU 1000000UL		// change this with respect to microcontroller clock speed.
#include <util/delay.h>

#define	IIC_DDR		DDRC
#define	IIC_PORT	PORTC
#define SCL			0
#define	SDA			1

#define CHIP_1_ADDRESS 	2	// select chip number between 0 and 7 for setting bits A2 A1 and A0 for physical chip address
#define CHIP_2_ADDRESS 	3	// physical addresses must be unique

uint8_t EEPROM_W_Address;
uint8_t EEPROM_R_Address;

void Initialize_TWI(void);

void Initialize_TWI()
{
	IIC_DDR &= ~((1<<SDA)|(1<<SCL));	// set these pins as input
	IIC_PORT |= (1<<SDA) | (1<<SCL);	// pull ups on SDA and SCL
}

uint8_t EEPROM_WriteByte(uint8_t eeprom_chip, uint16_t address, uint8_t data)
{

	if (eeprom_chip == 1 ) EEPROM_W_Address = 0xA0 | CHIP_1_ADDRESS<<1; 		// 1010 A2 A1 A0 R/W	W=0
	else if (eeprom_chip == 2 ) EEPROM_W_Address = 0xA0 | CHIP_2_ADDRESS<<1; 	// 1010 A2 A1 A0 R/W	W=0
	else return 0;

	do
	{
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Put Start Condition on TWI Bus
		
		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
		
		if((TWSR & 0xF8) != 0x08) return 0;		//Check status
		
		TWDR = EEPROM_W_Address;				//Now write SLA+W //EEPROM @ 00h
		TWCR = (1<<TWINT)|(1<<TWEN);			//Initiate Transfer

		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
	}
	while((TWSR & 0xF8) != 0x18);
	
	TWDR=(address>>8);						//Now write ADDRH
	TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
	
	while(!(TWCR & (1<<TWINT)));			//Poll Till Done
	
	if((TWSR & 0xF8) != 0x28) return 0;		//Check status
		
	TWDR=(address);							//Now write ADDRL
	
	TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
	
	while(!(TWCR & (1<<TWINT)));			//Poll Till Done
	
	if((TWSR & 0xF8) != 0x28) return 0;		//Check status

	TWDR=(data);							//Now write DATA
	TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
	
	while(!(TWCR & (1<<TWINT)));			//Poll Till Done
	
	if((TWSR & 0xF8) != 0x28) return 0;		//Check status
	
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	//Put Stop Condition on bus
	
	while(TWCR & (1<<TWSTO));				//Wait for STOP to finish

	_delay_ms(10);							//Wait untill Writing is complete
	
	return 1;								//Return TRUE

}

uint8_t EEPROM_ReadByte(uint8_t eeprom_chip, uint16_t address)
{

	if (eeprom_chip == 1 ) 
	{
		EEPROM_W_Address = 0xA0 | CHIP_1_ADDRESS<<1; // 1010 A2 A1 A0 R/W	W=0
		EEPROM_R_Address = 0xA1 | CHIP_1_ADDRESS<<1; // 1010 A2 A1 A0 R/W	R=1
	}
	else if (eeprom_chip == 2 ) 
	{
		EEPROM_W_Address = 0xA0 | CHIP_2_ADDRESS<<1; // 1010 A2 A1 A0 R/W	W=0
		EEPROM_R_Address = 0xA1 | CHIP_2_ADDRESS<<1; // 1010 A2 A1 A0 R/W	R=1
	}

	else return 0;

	uint8_t data;

	//*************** Initiate a Dummy Write Sequence to start Random Read ***************
	do
	{
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Put Start Condition on TWI Bus
		
		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
		
		if((TWSR & 0xF8) != 0x08) return 0;		//Check status
		
		TWDR=EEPROM_W_Address;					//Now write SLA+W EEPROM @ 00h
		TWCR=(1<<TWINT)|(1<<TWEN);				//Initiate Transfer
		
		while(!(TWCR & (1<<TWINT)));			//Poll Till Done
	}
	while((TWSR & 0xF8) != 0x18);
			
	TWDR=(address>>8);							//Now write ADDRH
	TWCR=(1<<TWINT)|(1<<TWEN);					//Initiate Transfer
	
	while(!(TWCR & (1<<TWINT)));				//Poll Till Done
	
	if((TWSR & 0xF8) != 0x28) return 0;			//Check status
	
	TWDR=(address);								//Now write ADDRL
	TWCR=(1<<TWINT)|(1<<TWEN);					//Initiate Transfer
	
	while(!(TWCR & (1<<TWINT)));				//Poll Till Done
	
	if((TWSR & 0xF8) != 0x28) return 0;			//Check status

	//***************************** DUMMY WRITE SEQUENCE END **************************
	
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);		//Put Start Condition on TWI Bus
	
	while(!(TWCR & (1<<TWINT)));				//Poll Till Done
	
	if((TWSR & 0xF8) != 0x10) return 0;			//Check status

	TWDR=EEPROM_R_Address;						//Now write SLA+R EEPROM @ 00h
	TWCR=(1<<TWINT)|(1<<TWEN);					//Initiate Transfer
	
	while(!(TWCR & (1<<TWINT)));				//Poll Till Done
	
	if((TWSR & 0xF8) != 0x40) return 0;			//Check status
	
	TWCR=(1<<TWINT)|(1<<TWEN);					//Now enable Reception of data by clearing TWINT
	
	while(!(TWCR & (1<<TWINT)));				//Wait till done
	
	if((TWSR & 0xF8) != 0x58) return 0;			//Check status
	
	data = TWDR;								//Read the data
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);		//Put Stop Condition on bus
		
	while(TWCR & (1<<TWSTO));					//Wait for STOP to finish

	return data;								//Return TRUE
}

#endif
