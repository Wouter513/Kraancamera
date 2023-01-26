/*
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "basic_io.h"
#include "clock.h"
#include "kraan_positie.h"
#include "rx_tx.h"


int main(void)
{
    //init alles
    init_time();
    init_io();
    init_motors();
    init_RXTX();

    while(new_recieve_data == 0)
    {
        _delay_ms(1);
    }
    control_x(2, ((recieve_message[2])|(recieve_message[3]<<8)));
    control_y(2, ((recieve_message[4])|(recieve_message[5]<<8)));
    control_z(2, ((recieve_message[6])|(recieve_message[7]<<8)));
    new_recieve_data = 0;
    while(1)
    {
        bio_kraan_enable(2);
        bio_kraan_load_unload(2);
        if(new_recieve_data)
        {
            new_recieve_data = 0;
            if(recieve_message[1] == 1)
            {//update position
                control_x(0, ((recieve_message[2])|(recieve_message[3]<<8)));
                control_y(0, ((recieve_message[4])|(recieve_message[5]<<8)));
                control_z(0, ((recieve_message[6])|(recieve_message[7]<<8)));
            }
            else if(recieve_message [1] == 2)
            {
                control_x(1, ((recieve_message[2])|(recieve_message[3]<<8)));
                control_y(1, ((recieve_message[4])|(recieve_message[5]<<8)));
                control_z(1, ((recieve_message[6])|(recieve_message[7]<<8)));
            }
        }
    }
    return 0;
}
