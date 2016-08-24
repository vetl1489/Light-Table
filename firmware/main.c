// https://translate.google.ru/

// ATMega8A
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "bits_macros.h"
#include "config.h"
//#include "softtimers.h"
#include "buttons.h"
#include "managepwm.h"

uint8_t EEMEM work_it = ON;

// номер активного канала для регулировки
uint8_t current_ch_saved EEMEM = 1;
uint8_t current_ch = 1;

// индексы таблицы значений шим для каждого из каналов
uint8_t ch1_pwm_saved EEMEM = 0;
uint8_t ch2_pwm_saved EEMEM = 0;
uint8_t ch3_pwm_saved EEMEM = 0;
uint8_t ch1_pwm = 0;
uint8_t ch2_pwm = 0;
uint8_t ch3_pwm = 0;
extern const uint8_t pwmtable[PWMSIZE] PROGMEM;

uint8_t but = 0; // код нажатой кнопки
uint8_t but_code = 0; // код события нажатой кнопки

// время, пока девайс активен ~5 сек
#define WORKTIME 7800
volatile uint16_t work_time = WORKTIME;
// время мигания светодиода выбора канала
#define FLICKTIME 150
volatile uint16_t flick_time = FLICKTIME;
// время задержки перед запуском функции опроса кнопок
#define TIMEBUT 8
volatile uint16_t but_time = TIMEBUT;
// время задержки переборе значений при зажатой кнопке
#define TIMESTEP 100
volatile uint16_t step_time = TIMESTEP;

ISR(TIMER0_OVF_vect)
{
	// флаг времени для регулировок
	if (flag.work == SET) {
		if (work_time > 0) work_time--;
		else flag.work = UNSET;
	}
	
	// флаг мигания светодиода активного канала
	if (flag.flick == SET) {
		if (flick_time > 0) flick_time--;
		else {
			flag.flick = UNSET;
			flick_time = FLICKTIME;
		}
	} 
	
	// флаг запускающий опрос кнопок
	if (flag.buttons == SET) {
		if (but_time > 0) but_time--;
		else flag.buttons = UNSET;
	}
	/*
	// флаг запуска таймера задержки перебора значений при зажатой кнопке
	if (flag.steps == SET) {
		if (step_time > 0) step_time--;
		else flag.steps = UNSET;
	}
	*/
}

void ResetWorkTime()
{
	DisableInterrupt;
	work_time = WORKTIME;
	EnableInterrupt;
}
void ResetFlickTime()
{
	DisableInterrupt;
	flick_time = 0;
	EnableInterrupt;
}

void LEDState(uint8_t led, uint8_t state)
{
	if (state) LED_PORT |= (1<<led);
	else LED_PORT &= ~(1<<led);
}

void Flickering(uint8_t chanel)
{
	static uint8_t state = 0;
	if (flag.flick == UNSET)
	{
		state = ~state;
		flag.flick = SET;
	}

	if (state == OFF) {
		switch(chanel) {
			case CH1: LEDState(LED1, OFF); break;
			case CH2: LEDState(LED2, OFF); break;
			case CH3: LEDState(LED3, OFF); break;
		}
	} else {
		switch(chanel) {
			case CH1: LEDState(LED1, ON); break;
			case CH2: LEDState(LED2, ON); break;
			case CH3: LEDState(LED3, ON); break;
		}
	}
}

void LEDS(uint8_t chanel)
{
	if (flag.onoff == ON) {
		if (flag.work == SET) {
			LEDState(LEDOFF, OFF);
			switch(chanel) {
				case CH1:
				LEDState(LED3, ON);
				LEDState(LED2, ON);
				break;
				case CH2:
				LEDState(LED1, ON);
				LEDState(LED3, ON);
				break;
				case CH3:
				LEDState(LED1, ON);
				LEDState(LED2, ON);
				break;
			}
			Flickering(chanel);
		}
	
		if (flag.work == UNSET) {
			LEDState(LED1, ON);
			LEDState(LED2, ON);
			LEDState(LED3, ON);
			LEDState(LEDOFF, OFF);
		}
	} 
	else {
		LEDState(LED1, OFF);
		LEDState(LED2, OFF);
		LEDState(LED3, OFF);
		LEDState(LEDOFF, ON);
	}
}

void PWMOFF()
{
	//T2
	TCCR2 &= ~(1<<COM21)|(1<<COM20);
	//T1
	TCCR1A &= ~(1<<COM1A1);
	TCCR1A &= ~(1<<COM1B1);
	PORTB &= ~(1<<PWM1)|(1<<PWM2)|(1<<PWM3);
}

void PWMON()
{
	//T2
	TCCR2 |= (1<<COM21);
	TCCR2 &= ~(1<<COM20);
	//T1
	TCCR1A |= (1<<COM1B1)|(1<<COM1A1);
	TCCR1A &= ~(1<<COM1B0)|(1<<COM1A0);
}

int main(void)
{

		
	// текущий канал
	if (eeprom_read_byte(&current_ch_saved) > CHNUM)
	{
		current_ch = 1;
		eeprom_write_byte(&current_ch_saved, current_ch);
	}
	else
	{
		current_ch = eeprom_read_byte(&current_ch_saved);
	}

	// уровни на каждом канале
	if (eeprom_read_byte(&ch1_pwm_saved) >= PWMSIZE)
	{
		ch1_pwm = 0;
		eeprom_write_byte(&ch1_pwm_saved, ch1_pwm);
	}
	else
	{
		ch1_pwm = eeprom_read_byte(&ch1_pwm_saved);
	}
	
	if (eeprom_read_byte(&ch2_pwm_saved) >= PWMSIZE)
	{
		ch2_pwm = 0;
		eeprom_write_byte(&ch2_pwm_saved, ch2_pwm);
	}
	else
	{
		ch2_pwm = eeprom_read_byte(&ch2_pwm_saved);
	}
	
	if (eeprom_read_byte(&ch3_pwm_saved) >= PWMSIZE)
	{
		ch3_pwm = 0;
		eeprom_write_byte(&ch3_pwm_saved, ch3_pwm);
	}
	else
	{
		ch3_pwm = eeprom_read_byte(&ch3_pwm_saved);
	}
	
	SetSavedPWM();
	BaseConfig();
	BUT_Init();
	WatchDogConfig();	
	
	if (eeprom_read_byte(&work_it) > 1)
	{
		flag.onoff = ON;
		eeprom_write_byte(&work_it, ON);
	}
	else if (eeprom_read_byte(&work_it) == 1)
	{
		flag.onoff = ON;
	}
	else
	{
		flag.onoff = OFF;
		PWMOFF();
	}
	
	flag.flick = SET;
	
    while (1) 
    {
		asm("WDR"); // сбрасываем сторожевой таймер
		
		if (flag.buttons == UNSET)
		{
			BUT_Poll();
			but = BUT_GetBut(); //проверка буфера
			flag.buttons = SET;
			
			if (but > 0)
			{
				but_code = BUT_GetBut();
				switch(but){
					case BUT_OFF:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.onoff == ON) {
								flag.onoff = OFF;
								DisableInterrupt;
								eeprom_write_byte(&work_it, OFF);
								EnableInterrupt;
								PWMOFF();
								DisableInterrupt;
								work_time = 0;
								EnableInterrupt;
							}
							else {
								flag.onoff = ON;
								DisableInterrupt;
								eeprom_write_byte(&work_it, ON);
								EnableInterrupt;
								PWMON();
							}
						}
						break;
					case BUT_CH:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.onoff == ON) {
								if (flag.work == UNSET) flag.work = SET;
								else {
									current_ch = eeprom_read_byte(&current_ch_saved);
									if (current_ch < CHNUM) current_ch++;
									else current_ch = 1;
									DisableInterrupt;
									eeprom_write_byte(&current_ch_saved, current_ch);
									EnableInterrupt;
								}
								ResetWorkTime();
								ResetFlickTime();
							}
						}
						break;
					
					case BUT_DOWN:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.work == SET) {
								DownPWM(current_ch);
								ResetWorkTime();
							}
						}
						/*else if (but_code == BUT_HELD_CODE) {
							if (flag.work == SET) {
								flag.start_steps = SET;
								flag.steps = SET;
							}
						}*/
						break;
					
					case BUT_UP:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.work == SET) {
								UpPWM(current_ch);
								ResetWorkTime();
							}
						}
						/*else if (but_code == BUT_HELD_CODE) {
							if (flag.work == SET) {
								flag.start_steps = SET;
								flag.steps = SET;
							}
						}*/
						break;
				}
			}
		}
		/*
		if (flag.onoff == ON) {
			if (flag.work == SET) {
				
				if (flag.start_steps == SET) {
					
					if (BitIsClear(PORTB, 3)) {
						if (flag.steps == SET) {
							UpPWM(current_ch);
							flag.steps = UNSET;
						}
					}
					else if (BitIsClear(PORTB, 4)) {
						if (flag.steps == SET) {
							DownPWM(current_ch);
							flag.steps = UNSET;
						}
					}
					else flag.start_steps = UNSET;
				}
			
			}
		}
		*/
		LEDS(current_ch);

	}
}


