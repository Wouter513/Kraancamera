/*
    uses counter0 8bit


*/

#include <avr/io.h>
#include <avr/interrupt.h>

void init_time(){
    cli();
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    OCR0A = 249;
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TIMSK0 |= (1 << OCIE0A);
    sei();
    //clock 0 defineren
    //interupt setpoint vaststellen
}

int time_current_ms(int update){//0 = return, 1 = time++
    static unsigned long long int time_ms = 0;
    if (update == 1){
        time_ms++;
    }
    return(time_ms);
}

ISR(TIMER0_COMPA_vect){
    current_time_ms(1);
}
