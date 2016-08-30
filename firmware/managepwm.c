/*
 * managepwm.c
 *
 * Created: 23.08.2016 2:41:49
 *  Author: 1489
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "buttons.h"
#include "managepwm.h"

extern uint8_t EEMEM current_ch_saved;
extern uint8_t current_ch;

// индексы таблицы значений шим для каждого из каналов
extern uint8_t EEMEM ch1_pwm_saved;
extern uint8_t EEMEM ch2_pwm_saved;
extern uint8_t EEMEM ch3_pwm_saved;
extern uint8_t ch1_pwm;
extern uint8_t ch2_pwm;
extern uint8_t ch3_pwm;

const uint8_t pwmtable[PWMSIZE] PROGMEM = {0, 3, 8, 16, 28, 49, 85, 148, 255};

void SetSavedPWM()
{
// 	DisableInterrupt;
// 	OCR1A = pgm_read_byte(&pwmtable[ch1_pwm]);
// 	OCR1B = pgm_read_byte(&pwmtable[ch2_pwm]);
// 	OCR2 = pgm_read_byte(&pwmtable[ch3_pwm]);
// 	EnableInterrupt;
	
	if (ch1_pwm == 0) {
		PWMOFF(CH1);
	}
	else {
		PWMON(CH1);
		OCR1A = pgm_read_byte(&pwmtable[ch1_pwm]);
	}
	
	if (ch2_pwm == 0) {
		PWMOFF(CH2);
	} 
	else {
		PWMON(CH2);
		OCR1B = pgm_read_byte(&pwmtable[ch2_pwm]);
	}
	
	if (ch3_pwm == 0) {
		PWMOFF(CH3);
	}
	else {
		PWMON(CH3);
		OCR2 = pgm_read_byte(&pwmtable[ch3_pwm]);
	}
	
	
}

void SetPWM(uint8_t chanel, uint8_t level)
{
	DisableInterrupt;
	switch(chanel)
	{
		case CH1:
		OCR1A = level;
		break;
		case CH2:
		OCR1B = level;
		break;
		case CH3:
		OCR2 = level;
		break;
	}
	EnableInterrupt;
}

void DownPWM(uint8_t chanel)
{
	if (chanel == CH1)
	{
		ch1_pwm = eeprom_read_byte(&ch1_pwm_saved);
		if (ch1_pwm > 0)
		{
			ch1_pwm--;
			SetPWM(CH1, pgm_read_byte(&pwmtable[ch1_pwm]));
			eeprom_write_byte(&ch1_pwm_saved, ch1_pwm);
		}
		else {};
	}
	
	else if (chanel == CH2)
	{
		ch2_pwm = eeprom_read_byte(&ch2_pwm_saved);
		if (ch2_pwm > 0) 
		{
			ch2_pwm--;
			SetPWM(CH2, pgm_read_byte(&pwmtable[ch2_pwm]));
			eeprom_write_byte(&ch2_pwm_saved, ch2_pwm);
		}
		else {};
	}
	
	else if (chanel == CH3)
	{
		ch3_pwm = eeprom_read_byte(&ch3_pwm_saved);
		if (ch3_pwm > 0)
		{
			ch3_pwm--;
			SetPWM(CH3, pgm_read_byte(&pwmtable[ch3_pwm]));
			eeprom_write_byte(&ch3_pwm_saved, ch3_pwm);
		}
		else {};
	}
}

void DownPWM2(uint8_t chanel)
{
	switch(chanel)
	{
		case CH1:
			DisableInterrupt;
			ch1_pwm = eeprom_read_byte(&ch1_pwm_saved);
			EnableInterrupt;
			if (ch1_pwm > 0)
			{
				ch1_pwm--;
				if (ch1_pwm == 0) PWMOFF(CH1);
				else SetPWM(CH1, pgm_read_byte(&pwmtable[ch1_pwm]));
				DisableInterrupt;
				eeprom_write_byte(&ch1_pwm_saved, ch1_pwm);
				EnableInterrupt;
			}
			else {};
			break;
		
		case CH2:
			DisableInterrupt;
			ch2_pwm = eeprom_read_byte(&ch2_pwm_saved);
			EnableInterrupt;
			if (ch2_pwm > 0)
			{
				ch2_pwm--;
				if (ch2_pwm == 0) PWMOFF(CH2);
				else SetPWM(CH2, pgm_read_byte(&pwmtable[ch2_pwm]));
				DisableInterrupt;
				eeprom_write_byte(&ch2_pwm_saved, ch2_pwm);
				EnableInterrupt;
			}
			else {};
			break;
		
		case CH3:
			DisableInterrupt;
			ch3_pwm = eeprom_read_byte(&ch3_pwm_saved);
			EnableInterrupt;
			if (ch3_pwm > 0)
			{
				ch3_pwm--;
				if (ch3_pwm == 0) PWMOFF(CH3);
				else SetPWM(CH3, pgm_read_byte(&pwmtable[ch3_pwm]));
				DisableInterrupt;
				eeprom_write_byte(&ch3_pwm_saved, ch3_pwm);
				EnableInterrupt;
			}
			else {};
			break;
	}
}

void UpPWM(uint8_t chanel)
{
	if (chanel == CH1)
	{
		ch1_pwm = eeprom_read_byte(&ch1_pwm_saved);
		if (ch1_pwm < PWMSIZE-1)
		{
			ch1_pwm++;
			SetPWM(CH1, pgm_read_byte(&pwmtable[ch1_pwm]));
			eeprom_write_byte(&ch1_pwm_saved, ch1_pwm);
		}
		else {};
	} 
	
	else if (chanel == CH2)
	{
		ch2_pwm = eeprom_read_byte(&ch2_pwm_saved);
		if (ch2_pwm < PWMSIZE-1)
		{
			ch2_pwm++;
			SetPWM(CH2, pgm_read_byte(&pwmtable[ch2_pwm]));
			eeprom_write_byte(&ch2_pwm_saved, ch2_pwm);
		}
		else {};
	}
	
	else if (chanel == CH3)
	{
		ch3_pwm = eeprom_read_byte(&ch3_pwm_saved);
		if (ch3_pwm < PWMSIZE-1)
		{
			ch3_pwm++;
			SetPWM(CH3, pgm_read_byte(&pwmtable[ch3_pwm]));
			eeprom_write_byte(&ch3_pwm_saved, ch3_pwm);
		}
		else {};
	}
	
}

void UpPWM2(uint8_t chanel)
{
	if (chanel == CH1)
	{
		DisableInterrupt;
		ch1_pwm = eeprom_read_byte(&ch1_pwm_saved);
		EnableInterrupt;
		if (ch1_pwm < PWMSIZE-1)
		{
			if (ch1_pwm == 0) PWMON(CH1);
			ch1_pwm++;
			SetPWM(CH1, pgm_read_byte(&pwmtable[ch1_pwm]));
			DisableInterrupt;
			eeprom_write_byte(&ch1_pwm_saved, ch1_pwm);
			EnableInterrupt;
		}
		else {};
	
	}
	
	else if (chanel == CH2)
	{
		DisableInterrupt;
		ch2_pwm = eeprom_read_byte(&ch2_pwm_saved);
		EnableInterrupt;
		if (ch2_pwm < PWMSIZE-1)
		{
			if (ch2_pwm == 0) PWMON(CH2);
			ch2_pwm++;
			SetPWM(CH2, pgm_read_byte(&pwmtable[ch2_pwm]));
			DisableInterrupt;
			eeprom_write_byte(&ch2_pwm_saved, ch2_pwm);
			EnableInterrupt;
		}
		else {};
	}
	
	else if (chanel == CH3)
	{
		DisableInterrupt;
		ch3_pwm = eeprom_read_byte(&ch3_pwm_saved);
		EnableInterrupt;
		if (ch3_pwm < PWMSIZE-1)
		{
			if (ch3_pwm == 0) PWMON(CH3);
			ch3_pwm++;
			SetPWM(CH3, pgm_read_byte(&pwmtable[ch3_pwm]));
			DisableInterrupt;
			eeprom_write_byte(&ch3_pwm_saved, ch3_pwm);
			EnableInterrupt;
		}
		else {};
	}
}



