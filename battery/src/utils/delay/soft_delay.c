#include "soft_delay.h"

#define _nop_() __asm nop __endasm;

//STC8G

void Delay1s(void)	//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 122;
	j = 193;
	k = 128;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Delay1ms(void)	//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	i = 32;
	j = 40;
	do
	{
		while (--j);
	} while (--i);
}


void Delay10ms(void)	//@24.000MHz
{
	unsigned char i, j, k;

	i = 2;
	j = 56;
	k = 172;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay100ms(void)	//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 13;
	j = 45;
	k = 214;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay1us(void)	//@24.000MHz
{
	unsigned char i;

	i = 6;
	while (--i);
}

void Delay10us(void)	//@24.000MHz
{
	unsigned char i;

	i = 78;
	while (--i);
}


void Delay100us(void)	//@24.000MHz
{
	unsigned char i, j;

	i = 4;
	j = 27;
	do
	{
		while (--j);
	} while (--i);
}

