#include "clock.h"

int bio_kraan_enable(int mode){//0 = return value, 1 = flip value, 2=checkpin
    static int kraan_enable = 0;
    static unsigned long long int last_press_time = 0;
    if(mode == 1){
        kraan_enable = !kraan_enable;
    }
    else if(mode == 2){
        if(current_time_ms(0)-last_press_time >200){
            if(!(PINF&0b00000010)){ //statement die knop uitleest
                kraan_enable = !kraan_enable;
                last_press_time = current_time_ms(0);
            }
        }
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
        if(current_time_ms(0)-last_press_time >200){
            if(!(PINF&0b00000010)){//read kraan_load_unload pin
                load_unload_status = !load_unload_status;
                //flip load_unload_status als knop ingedrukt is
                last_press_time = current_time_ms(0);
            }
        }
    }
    return(load_unload_status);
}
