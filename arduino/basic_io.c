#include <avr/io.h>
#include "clock.h"

void init_io(void)
{
    DDRF &= ~((1<<PF0)|(1<<PF1));
    PORTF |= (1<<PF0) | (1<<PF1);
    DDRB |= (1<<PB0) | (1<<PB1);
}

int bio_kraan_enable(int mode){//0 = return value, 1 = flip value, 2=checkpin
    static int kraan_enable = 0;
    static unsigned long long int last_press_time = 0;
    if(mode == 1){
        kraan_enable = !kraan_enable;
    }
    else if(mode == 2){
        if(time_current_ms(0)-last_press_time >200){
            if((PINF&(1<<PF0))== 0){ //statement die knop uitleest
                kraan_enable = !kraan_enable;
                last_press_time = time_current_ms(0);
            }
        }
    }
    if(kraan_enable)
    {
        PORTB &= ~(1<<PB0);
    }
    else
    {
        PORTB |= (1<<PB0);
    }
    return(kraan_enable);
}

int bio_kraan_load_unload(int mode){//0 = return value, 1 = flip value, 2=checkpin
    static int load_unload_status = 0;
    static unsigned long long int last_press_time = 0;
    if(mode == 1){
        load_unload_status = !load_unload_status;
    }
    else if(mode == 2){
        if(time_current_ms(0)-last_press_time >200){
            if((PINF&(1<<PF1))== 0){//read kraan_load_unload pin
                load_unload_status = !load_unload_status;
                //flip load_unload_status als knop ingedrukt is
                last_press_time = time_current_ms(0);
            }
        }
    }
    if(load_unload_status)
    {
        PORTB &= ~(1<<PB1);
    }
    else
    {
        PORTB |= (1<<PB1);
    }
    return(load_unload_status);
}
