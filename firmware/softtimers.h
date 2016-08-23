/*
 * softtimers.h
 *
 * Created: 30.03.2016 22:41:49
 *  Author: 1489
 */ 


#ifndef SOFTTIMERS_H_
#define SOFTTIMERS_H_

// определяем прерывание в котором крутятся программные таймеры
#define ST_vect TIMER0_OVF_vect

// функция обработки программных таймеров
void ST_ISR_SoftTimers();

// устанавливаем таймер
void ST_SetTimer(unsigned char NewNumber, unsigned int NewTime);


#endif /* SOFTTIMERS_H_ */