/*
 * ADC_Module.h
 *
 * Created: 09-Jun-15 11:19:00 PM
 *  Author: Optimus
 */ 


#ifndef ADC_MODULE_H_
#define ADC_MODULE_H_

void inti_ADC();
int ADC_X();
int ADC_Y();
int ADC_Z();
void inti_ADC()
{
	ADCSRA |= (1<<ADEN);  //ADC enable
	ADMUX |= (1<<REFS0);  // AVCC as reference voltage
	ADCSRA = ~(1<<ADPS0) & ~(1<<ADPS1) & ~(1<<ADPS2);  // prescaler 2
}

int ADC_X()
{
	ADMUX = 0x00; // Channel ADC0
	ADCSRA = ADCSRA | (1<<ADSC);
	while((ADCSRA & (1<<ADSC))==1);
	_delay_ms(2);
	return ADC;
}
int ADC_Y()
{
	ADMUX = 0x01; // Channel ADC1
	ADCSRA = ADCSRA | (1<<ADSC);
	while((ADCSRA & (1<<ADSC))==1);
	_delay_ms(2);
	return ADC;
}
int ADC_Z()
{
	ADMUX = 0x02; // Channel ADC2
	ADCSRA = ADCSRA | (1<<ADSC);
	while((ADCSRA & (1<<ADSC))==1);
	_delay_ms(2);
	return ADC;
}


#endif /* ADC_MODULE_H_ */