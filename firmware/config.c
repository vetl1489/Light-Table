#include <avr/io.h>
#include "config.h"

void BaseConfig()
{
	// подключаем кнопочки
	DDRC &= ~(1<<0)|(1<<1); 
	PORTC |= (1<<0)|(1<<1);
	DDRD &= ~(1<<3)|(1<<4);
	PORTD |= (1<<3)|(1<<4);
	
	// ставим на выход пины с ШИМ
	DDRB |= (1<<PWM1)|(1<<PWM2)|(1<<PWM3);
	PORTB &= ~(1<<PWM1)|(1<<PWM2)|(1<<PWM3);
	
	// на выход пины светодидов
	DDRC |= (1<<2)|(1<<3)|(1<<4)|(1<<5);
	PORTC &= ~(1<<2)|(1<<3)|(1<<4)|(1<<5);

	
	// инициализируем таймеры
	
	// T0 - для программных таймеров
	// прерывание с частотой 448 Гц
	TCCR0 &= ~(1<<CS02);
	TCCR0 |= (1<<CS00)|(1<<CS01);
	TIMSK |= (1<<TOIE0);
	
	// T1 - для ШИМ
	// 8 битный FastPWM
	TCCR1A |= (1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	// с максимальной частотой
// 	TCCR1B |= (1<<CS10);
// 	TCCR1B &= ~(1<<CS11)|(1<<CS12);

	TCCR1B |= (1<<CS11);
 	TCCR1B &= ~(1<<CS10)|(1<<CS12);

	// на обоих выводах неинвертированный шим
	TCCR1A |= (1<<COM1B1)|(1<<COM1A1);
	TCCR1A &= ~(1<<COM1B0)|(1<<COM1A0);
	// OCR1A
	// OCR1B
	
	// T2 для ШИМ
	TCCR2 |= (1<<WGM20)|(1<<WGM21)|(1<<CS21)|(1<<COM21); // FastPWM, максимальная частота
	//TCCR2 &= ~(1<<FOC2)|(1<<CS21)|(1<<CS22)|(1<<COM20);
	TCCR2 &= ~(1<<FOC2)|(1<<CS22)|(1<<COM20);
	TCCR2 |= (1<<CS21);
	// OCR2 - сюда ШИМ
	
	EnableInterrupt;
}

void WatchDogConfig()
{
	WDTCR |= (1<<WDE)|(1<<WDP1)|(1<<WDP2);
}


void PWMOFF(uint8_t chanel)
{
	switch(chanel)
	{
		case CH1:
			TCCR1A &= ~(1<<COM1A1);
			PORTB &= ~(1<<PWM2);
			break;
		
		case CH2:
			TCCR1A &= ~(1<<COM1B1);
			PORTB &= ~(1<<PWM2);
			break;
		
		case CH3:
			TCCR2 &= ~(1<<COM21)|(1<<COM20);
			PORTB &= ~(1<<PWM3);
			break;
		
		case ALLCH:
			TCCR2 &= ~(1<<COM21)|(1<<COM20);
			TCCR1A &= ~(1<<COM1A1);
			TCCR1A &= ~(1<<COM1B1);
			PORTB &= ~(1<<PWM1)|(1<<PWM2)|(1<<PWM3);
			break;
	}
}

void PWMON(uint8_t chanel)
{
	switch(chanel)
	{
		case CH1:
			TCCR1A |= (1<<COM1A1);
			TCCR1A &= ~(1<<COM1A0);
			break;
		
		case CH2:
			TCCR1A |= (1<<COM1B1);
			TCCR1A &= ~(1<<COM1B0);
			break;
		
		case CH3:
			TCCR2 |= (1<<COM21);
			TCCR2 &= ~(1<<COM20);
			break;
		
		case ALLCH:
			TCCR2 |= (1<<COM21);
			TCCR2 &= ~(1<<COM20);
			TCCR1A |= (1<<COM1B1)|(1<<COM1A1);
			TCCR1A &= ~(1<<COM1B0)|(1<<COM1A0);
			break;
	}
}