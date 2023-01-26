#include "kraan_positie.h"
#include <avr/io.h>
#include <avr/interrupt.h>
//motor init gebruikt timers 3 4 en 5

int16_t sabs16(int16_t i)
{
    int16_t res;

    if (INT16_MIN == i)
    {
        res = INT16_MAX;
    }
    else
    {
        res = i < 0 ? -i : i;
    }

    return res;
}

void init_motors(void)
{
    cli();
    TCCR3A |= (1<<WGM32) | (1<<COM3A1) | (1<<COM3B1);
    TCCR3B |= (1<<WGM31);
    OCR3A = 0;
    OCR3B = 0;
    DDRE |= (1<<PE3) | (1<<PE4);

    TCCR4A |= (1<<WGM41) | (1<<COM4A1) | (1<<COM4B1);
    TCCR4B |= (1<<WGM42) | (1<<CS40);
    OCR4A = 0;
    OCR4B = 0;
    DDRH = (1<<PH3) | (1<<PH4);

    TCCR5A |= (1<<WGM51) | (1<<COM5A1) | (1<<COM5B1);
    TCCR5B |= (1<<WGM52) | (1<<CS50);
    OCR5A = 0;
    OCR5B = 0;
    DDRL = (1<<PL3) | (1<<PL4);

    TCNT3 = 0;
    TCNT4 = 0;
    TCNT5 = 0;

    sei();
}

void motor_speed_x(int percentage)
{
    if((percentage < 100) && (percentage > -100))
    {
        if(percentage > 0)
        {
            OCR3A = 512*percentage/100;
            OCR3B = 0;
        }
        if(percentage < 0)
        {
            OCR3A = 0;
            OCR3B = 512*percentage/-100;
        }
        if(percentage == 0)
        {
            OCR3A = 0;
            OCR3B = 0;
        }
    }
}

void motor_speed_y(int percentage)
{
    if((percentage < 100) && (percentage > -100))
    {
        if(percentage > 0)
        {
            OCR4A = 512*percentage/100;
            OCR4B = 0;
        }
        if(percentage < 0)
        {
            OCR4A = 0;
            OCR4B = 512*percentage/-100;
        }
        if(percentage == 0)
        {
            OCR4A = 0;
            OCR4B = 0;
        }
    }
}

void motor_speed_z(int percentage)
{
    if((percentage < 100) && (percentage > -100))
    {
        if(percentage > 0)
        {
            OCR5A = 512*percentage/100;
            OCR5B = 0;
        }
        if(percentage < 0)
        {
            OCR5A = 0;
            OCR5B = 512*percentage/-100;
        }
        if(percentage == 0)
        {
            OCR5A = 0;
            OCR5B = 0;
        }
    }
}

void control_x(int8_t mode, int16_t position)
{
    static int16_t current_pos = 0;
    static int16_t target_pos = 0;
    static int16_t stationary_pos = 0;

    if(mode == 0) //update positie
    {
        current_pos = position;
    }
    else if(mode == 1) //update target
    {
        target_pos = position;
    }
    else
    {
        current_pos = position;
        target_pos = position;
    }
    int16_t error = target_pos - current_pos;

    int8_t direction;
    if(error >= 0)
    {
        direction = 1;
    }
    else
    {
        direction = -1;
    }

    int16_t abserror = sabs16(error);

    int16_t error_stationary = sabs16(current_pos-stationary_pos);
    if(abserror < 3)
    {
        stationary_pos = current_pos;
        motor_speed_x(0);
    }
    else
    {
        if(abserror < 10)//laatste 7mm
        {
            motor_speed_x(direction*10);
        }
        else if(error_stationary<50)//opstarten
        {
            motor_speed_x(direction*20);
        }
        else if(abserror < 30)
        {
            motor_speed_x(direction*20);
        }
        else
        {
            motor_speed_x(direction*50);
        }
    }
}

void control_y(int8_t mode, int16_t position)
{
    static int16_t current_pos = 0;
    static int16_t target_pos = 0;
    static int16_t stationary_pos = 0;

    if(mode == 0) //update positie
    {
        current_pos = position;
    }
    else if(mode == 1) //update target
    {
        target_pos = position;
    }
    else//update both (init)
    {
        current_pos = position;
        target_pos = position;
    }
    int16_t error = target_pos - current_pos;

    int8_t direction;
    if(error >= 0)
    {
        direction = 1;
    }
    else
    {
        direction = -1;
    }

    int16_t abserror = sabs16(error);

    int16_t error_stationary = sabs16(current_pos-stationary_pos);
    if(abserror < 3)
    {
        stationary_pos = current_pos;
        motor_speed_y(0);
    }
    else
    {
        if(abserror < 10)//laatste 7mm
        {
            motor_speed_y(direction*10);
        }
        else if(error_stationary<50)//opstarten
        {
            motor_speed_y(direction*20);
        }
        else if(abserror < 30)
        {
            motor_speed_y(direction*20);
        }
        else if(abserror < 100)
        {
            motor_speed_y(direction*50);
        }
        else
        {
            motor_speed_y(direction*80);
        }
    }
}

void control_z(int8_t mode, int16_t position)
{
    static int16_t current_pos = 0;
    static int16_t target_pos = 0;
    static int16_t stationary_pos = 0;

    if(mode == 0) //update positie
    {
        current_pos = position;
    }
    else if(mode == 1) //update target
    {
        target_pos = position;
    }
    else
    {
        current_pos = position;
        target_pos = position;
    }
    int16_t error = target_pos - current_pos;

    int8_t direction;
    if(error >= 0)
    {
        direction = 1;
    }
    else
    {
        direction = -1;
    }

    int16_t abserror = sabs16(error);

    int16_t error_stationary = sabs16(current_pos-stationary_pos);
    if(abserror < 3)
    {
        stationary_pos = current_pos;
        motor_speed_y(0);
    }
    else
    {
        if(abserror < 10)//laatste 7mm
        {
            motor_speed_y(direction*30);
        }
        else if(error_stationary<50)//opstarten
        {
            motor_speed_y(direction*100);
        }
        else if(abserror < 30)
        {
            motor_speed_y(direction*50);
        }
        else
        {
            motor_speed_y(direction*80);
        }
    }
}
