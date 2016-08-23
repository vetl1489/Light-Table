/*
 * softtimers.c
 *
 * Created: 30.03.2016 22:43:42
 *  Author: 1489
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
//#include "bits_macros.h"
#include "config.h"
#include "softtimers.h"



/*
ISR(ST_vect)
{
	for(unsigned char i=0; i!=MAXTIMERS; i++)      // Прочесываем очередь таймеров
	{
		if(SoftTimer[i].Number == 255 ) continue; // Если нашли пустышку - следующая итерация
		if(SoftTimer[i].Time != 0)		 // Если таймер не выщелкал, то щелкаем еще раз.
		{
			SoftTimer[i].Time--;	 // Уменьшаем число в ячейке если не конец.
		}
		else
		{
			if (SoftTimer[i].Number==0) flag.timer1 = 1;
			if (SoftTimer[i].Number==1) flag.timer2 = 1;
			if (SoftTimer[i].Number==2) flag.timer3 = 1;
			SoftTimer[i].Number = 255;
		}
	}
	
	IND_Update();
	
	static uint16_t five_time = 1024;
	if (flag.five)
	{
		five_time--;
		if (five_time == 0)
		{
			flag.five = 0;
		}
	}
	
	
}
*/

inline void ST_ISR_SoftTimers()
{
	for(unsigned char i=0; i!=MAXTIMERS; i++)      // Прочесываем очередь таймеров
	{
		if(SoftTimer[i].Number == 255 ) continue; // Если нашли пустышку - следующая итерация
		if(SoftTimer[i].Time != 0)		 // Если таймер не выщелкал, то щелкаем еще раз.
		{
			SoftTimer[i].Time--;	 // Уменьшаем число в ячейке если не конец.
		}
		else
		{
			if (SoftTimer[i].Number==0) flag.timer1 = 1;
			if (SoftTimer[i].Number==1) flag.timer2 = 1;
			if (SoftTimer[i].Number==2) flag.timer3 = 1;
			if (SoftTimer[i].Number==3) flag.timer4 = 1;
			SoftTimer[i].Number = 255;
		}
	}
}


// запускаем таймер
void ST_SetTimer(unsigned char NewNumber, unsigned int NewTime)
{
	unsigned char i;
	DisableInterrupt;	// Запрещаем прерывания. Помним об атомарном доступе!
	
	for(i=0; i!=MAXTIMERS; i++)   	//Прочесываем очередь таймеров. Ищем нет ли уже там такого
	{
		if(SoftTimer[i].Number == NewNumber)	// Если уже есть запись с таким флагом
		{
			SoftTimer[i].Time = NewTime;	// Перезаписываем ей выдержку
			EnableInterrupt;
			return;				// Выходим.
		}
	}
	for(i=0; i!=MAXTIMERS; i++)	//Если не находим, то ищем любой пустой
	{
		if (SoftTimer[i].Number == 255)
		{
			SoftTimer[i].Number = NewNumber;	// Заполняем поле флага
			SoftTimer[i].Time = NewTime;		// И поле выдержки времени
			EnableInterrupt;
			return;					// Выход.
		}
	}
	EnableInterrupt;	// Восстанавливаем прерывания как было.
}

// запускаем таймер и выставляем флаг
/*
void ST_SetFlagTimer(uint8_t NewNumber, _Bool *new_flag, uint16_t NewTime)
{
	uint8_t i;
	DisableInterrupt;	// Запрещаем прерывания. Помним об атомарном доступе!
	
	for(i=0; i!=MAXTIMERS; i++)   	//Прочесываем очередь таймеров. Ищем нет ли уже там такого
	{
		if(SoftTimer[i].Number == NewNumber)	// Если уже есть запись с таким флагом
		{
			SoftTimer[i].Time = NewTime;	// Перезаписываем ей выдержку
			EnableInterrupt;
			return;				// Выходим.
		}
	}
	*new_flag = 1;
	EnableInterrupt;	// Восстанавливаем прерывания как было.
}

*/
