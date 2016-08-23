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
	for(unsigned char i=0; i!=MAXTIMERS; i++)      // ����������� ������� ��������
	{
		if(SoftTimer[i].Number == 255 ) continue; // ���� ����� �������� - ��������� ��������
		if(SoftTimer[i].Time != 0)		 // ���� ������ �� ��������, �� ������� ��� ���.
		{
			SoftTimer[i].Time--;	 // ��������� ����� � ������ ���� �� �����.
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
	for(unsigned char i=0; i!=MAXTIMERS; i++)      // ����������� ������� ��������
	{
		if(SoftTimer[i].Number == 255 ) continue; // ���� ����� �������� - ��������� ��������
		if(SoftTimer[i].Time != 0)		 // ���� ������ �� ��������, �� ������� ��� ���.
		{
			SoftTimer[i].Time--;	 // ��������� ����� � ������ ���� �� �����.
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


// ��������� ������
void ST_SetTimer(unsigned char NewNumber, unsigned int NewTime)
{
	unsigned char i;
	DisableInterrupt;	// ��������� ����������. ������ �� ��������� �������!
	
	for(i=0; i!=MAXTIMERS; i++)   	//����������� ������� ��������. ���� ��� �� ��� ��� ������
	{
		if(SoftTimer[i].Number == NewNumber)	// ���� ��� ���� ������ � ����� ������
		{
			SoftTimer[i].Time = NewTime;	// �������������� �� ��������
			EnableInterrupt;
			return;				// �������.
		}
	}
	for(i=0; i!=MAXTIMERS; i++)	//���� �� �������, �� ���� ����� ������
	{
		if (SoftTimer[i].Number == 255)
		{
			SoftTimer[i].Number = NewNumber;	// ��������� ���� �����
			SoftTimer[i].Time = NewTime;		// � ���� �������� �������
			EnableInterrupt;
			return;					// �����.
		}
	}
	EnableInterrupt;	// ��������������� ���������� ��� ����.
}

// ��������� ������ � ���������� ����
/*
void ST_SetFlagTimer(uint8_t NewNumber, _Bool *new_flag, uint16_t NewTime)
{
	uint8_t i;
	DisableInterrupt;	// ��������� ����������. ������ �� ��������� �������!
	
	for(i=0; i!=MAXTIMERS; i++)   	//����������� ������� ��������. ���� ��� �� ��� ��� ������
	{
		if(SoftTimer[i].Number == NewNumber)	// ���� ��� ���� ������ � ����� ������
		{
			SoftTimer[i].Time = NewTime;	// �������������� �� ��������
			EnableInterrupt;
			return;				// �������.
		}
	}
	*new_flag = 1;
	EnableInterrupt;	// ��������������� ���������� ��� ����.
}

*/
