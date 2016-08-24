/*
 * managepwm.h
 *
 * Created: 23.08.2016 2:41:23
 *  Author: 1489
 */ 


#ifndef MANAGEPWM_H_
#define MANAGEPWM_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "buttons.h"
#include "managepwm.h"

#define PWMSIZE 8

void SetSavedPWM();
void SetPWM(uint8_t chanel, uint8_t level);

void DownPWM(uint8_t chanel);
void DownPWM2(uint8_t chanel);

void UpPWM(uint8_t chanel);
void UpPWM2(uint8_t chanel);



#endif /* MANAGEPWM_H_ */