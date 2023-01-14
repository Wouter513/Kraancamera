//motor init gebruikt timers 3 4 en 5

void init_motor(){



}
ISR(TIMER3_OVF_vect)
{
	if (OCR3A == 0 && OCR3B == 0)
	{
		PORT_RPWM3 &= ~(1<<PIN_RPWM);
		PORT_LPWM3 &= ~(1<<PIN_LPWM);
	}
	else if (OCR3A != 0)
	{
		PORT_LPWM3 &= ~(1<<PIN_LPWM);
		PORT_RPWM3 |= (1<<PIN_RPWM);
	}
	else if (OCR3B != 0)
	{
		PORT_RPWM3 &= ~(1<<PIN_RPWM);
		PORT_LPWM3 |= (1<<PIN_LPWM);
	}
}

ISR(TIMER3_COMPA_vect)
{
	if (OCR3A != 255)
	{
		PORT_RPWM3 &= ~(1<<PIN_RPWM3);
	}
}

ISR(TIMER3_COMPB_vect)
{
	if (OCR3B != 255)
	{
		PORT_LPWM3 &= ~(1<<PIN_LPWM3);
	}
}



ISR(TIMER4_OVF_vect)
{
	if (OCR4A == 0 && OCR0B == 0)
	{
		PORT_RPWM4 &= ~(1<<PIN_RPWM);
		PORT_LPWM4 &= ~(1<<PIN_LPWM);
	}
	else if (OCR4A != 0)
	{
		PORT_LPWM4 &= ~(1<<PIN_LPWM);
		PORT_RPWM4 |= (1<<PIN_RPWM);
	}
	else if (OCR4B != 0)
	{
		PORT_RPWM4 &= ~(1<<PIN_RPWM);
		PORT_LPWM4 |= (1<<PIN_LPWM);
	}
}

ISR(TIMER4_COMPA_vect)
{
	if (OCR4A != 255)
	{
		PORT_RPWM4 &= ~(1<<PIN_RPWM4);
	}
}

ISR(TIMER4_COMPB_vect)
{
	if (OCR4B != 255)
	{
		PORT_LPWM4 &= ~(1<<PIN_LPWM4);
	}
}



ISR(TIMER5_OVF_vect)
{
	if (OCR5A == 0 && OCR0B == 0)
	{
		PORT_RPWM5 &= ~(1<<PIN_RPWM);
		PORT_LPWM5 &= ~(1<<PIN_LPWM);
	}
	else if (OCR5A != 0)
	{
		PORT_LPWM5 &= ~(1<<PIN_LPWM);
		PORT_RPWM5 |= (1<<PIN_RPWM);
	}
	else if (OCR5B != 0)
	{
		PORT_RPWM5 &= ~(1<<PIN_RPWM);
		PORT_LPWM5 |= (1<<PIN_LPWM);
	}
}

ISR(TIMER5_COMPA_vect)
{
	if (OCR5A != 255)
	{
		PORT_RPWM5 &= ~(1<<PIN_RPWM5);
	}
}

ISR(TIMER5_COMPB_vect)
{
	if (OCR5B != 255)
	{
		PORT_LPWM5 &= ~(1<<PIN_LPWM5);
	}
}


