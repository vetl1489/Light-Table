// config.h
// общие определения и конфигурация переферии

#ifndef CONFIG_H_
#define CONFIG_H_
#include <avr/io.h>

#define EnableInterrupt		asm("sei")
#define DisableInterrupt	asm("cli")
#define NOP					asm("nop")

#define PWM1 1
#define PWM2 2
#define PWM3 3
///////////////////////////////////////////////////
#define ON 1
#define OFF 0
#define SET 1
#define UNSET 0

// флаги флагового автомата для программных таймеров
#define t1	0
#define t2	1
#define t3	2
#define t4	3

#define LED_PORT	PORTC
#define LED1		2
#define LED2		3
#define LED3		5
#define LEDOFF		4

#define CHNUM	3
#define CH1		1
#define CH2		2
#define CH3		3
#define ALLCH	4

#define BUT_OFF		1
#define BUT_CH		2
#define BUT_DOWN	3
#define BUT_UP		4


// флаговый автомат
volatile struct rrr
{
	_Bool onoff: 1; // включено/выключено
	_Bool work: 1;	
	_Bool flick: 1;	// мигаем светиком
	_Bool buttons: 1;	// кнопки опрашиваем
	_Bool start_steps: 1; //флаг запускающий перебор значений масива
	_Bool steps: 1; // флаг запуска таймера задержки перебора значений при зажатой кнопке

} flag;

void BaseConfig();		// базовая конфигурация контроллера

void WatchDogConfig();	// сторожевая собака

void PWMOFF(uint8_t chanel);
void PWMON(uint8_t chanel);

#endif