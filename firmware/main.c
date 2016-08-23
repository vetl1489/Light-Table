// https://translate.google.ru/

// ATMega8A
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
//#include "port_macros.h"
#include "config.h"
//#include "softtimers.h"
#include "buttons.h"
#include "managepwm.h"

// номер активного канала для регулировки
uint8_t current_ch_saved EEMEM = 0;
uint8_t current_ch = 0;

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
uint16_t work_time = WORKTIME;
// время мигания светодиода выбора канала
#define FLICKTIME 150
uint16_t flick_time = FLICKTIME;
// время задержки перед запуском функции опроса кнопок
#define TIMEBUT 8
uint16_t but_time = TIMEBUT;

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

int main(void)
{
	// текущий канал
	current_ch = eeprom_read_byte(&current_ch_saved);
	// уровни на каждом канале
	ch1_pwm = eeprom_read_byte(&ch1_pwm_saved);
	ch2_pwm = eeprom_read_byte(&ch2_pwm_saved);
	ch3_pwm = eeprom_read_byte(&ch3_pwm_saved);
	
	BaseConfig();
	BUT_Init();
	SetSavedPWM();

	flag.flick = SET;
	flag.onoff = ON;
	
    while (1) 
    {
		asm("WDR"); // сбрасываем сторожевой таймер
		
		if (flag.buttons == UNSET)
		{
			BUT_Poll();
			but = BUT_GetBut(); //проверка буфера
			flag.buttons = SET;
			
			if (but)
			{
				but_code = BUT_GetBut();
				switch(but){
					case BUT_OFF:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.onoff == ON) {
								flag.onoff = OFF;
								SetPWM(CH1, 0);
								SetPWM(CH2, 0);
								SetPWM(CH3, 0);
								DisableInterrupt;
								work_time = 0;
								EnableInterrupt;
							}
							else {
								flag.onoff = ON;
								SetSavedPWM();
							}
						}
						break;
					case BUT_CH:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.onoff == ON) {
								if (flag.work == UNSET) flag.work = SET;
								else {
									if (current_ch < CHNUM) current_ch++;
									else current_ch = 1;
								}
								DisableInterrupt;
								eeprom_write_byte(&current_ch_saved, current_ch);
								EnableInterrupt;
								ResetWorkTime();
								ResetFlickTime();
								/*DisableInterrupt;
								flick_time = 0;
								EnableInterrupt;*/
							}
						}
						break;
					case BUT_DOWN:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.work == SET) {
								//DownPWM(current_ch);
								ResetWorkTime();
							}
						}
						break;
					case BUT_UP:
						if (but_code == BUT_RELEASED_CODE){
							if (flag.work == SET) {
								//DownPWM(current_ch);
								//UpPWM(current_ch);
								ResetWorkTime();
							}
						}
						break;
				}
			}
		}
		
		
		/*if (flag.onoff == ON)
		{
			if (flag.work == SET) {
				switch(current_ch){
					case CH1:
					LED(LED1, ON);
					LED(LED2, OFF);
					LED(LED3, OFF);
					break;
					case CH2:
					LED(LED2, ON);
					LED(LED1, OFF);
					LED(LED3, OFF);
					break;
					case CH3:
					LED(LED3, ON);
					LED(LED2, OFF);
					LED(LED1, OFF);
					break;
			}
		}*/
		
		
		if (flag.onoff == ON) {
			if (flag.work == SET) {
				LEDState(LEDOFF, OFF);
				switch(current_ch) {
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
			Flickering(current_ch);
			}
			
			if (flag.work == UNSET) {
				LEDState(LED1, ON);
				LEDState(LED2, ON);
				LEDState(LED3, ON);
				LEDState(LEDOFF, OFF);
			}
		} else {
			LEDState(LED1, OFF);
			LEDState(LED2, OFF);
			LEDState(LED3, OFF);
			LEDState(LEDOFF, ON);
		}

	}
}


