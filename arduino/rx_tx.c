/*
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define message_length 10
#define start_byte 0xFD //254
#define end_byte 0x21 //255

#define message_identifier_position 0x01

#define F_CPU   16000000
#define BAUD    9600
#define BRC     ((F_CPU/16/BAUD) - 1)

#define D1 PB7//13
#define D2 PB6//12
#define D3 PB5//11
#define D4 PB4//10
#define S1 PF1//A1
#define S2 PF2//A2
#define S3 PF3//A3

volatile uint8_t new_recieve_data = 0;
volatile uint8_t recieve_message[message_length];
volatile uint8_t transmit_message[message_length];
volatile uint8_t transmiting_data = 0;

//USART0_UDRE_vect
//USART0_RX_vect
//USART0_TX_vect

ISR(USART0_UDRE_vect) //UDR empty
{
    static uint8_t writer_position = 0;
    UDR0 = transmit_message[writer_position];
    if ((transmit_message[writer_position] == end_byte) ||(writer_position > 9))
    {

        writer_position = 0;
        UCSR0B &= ~(1<<UDRIE0);
        transmiting_data = 0;
    }
    else
    {
        writer_position++;
    }
}

ISR(USART0_RX_vect) //recievemsg
{
    static uint8_t building_recieve_message[message_length];
    static uint8_t reader_position = 0;
    volatile uint8_t tempbyte = UDR0;
    if(tempbyte == start_byte)
    {
        reader_position = 0;
        PORTB &= ~(1<<D3);
        building_recieve_message[reader_position] = tempbyte;
        reader_position++;
    }
    else if(tempbyte == end_byte)
    {
        building_recieve_message[reader_position] = tempbyte;
        uint8_t i = message_length;
        while(i)
        {
            i--;
            recieve_message[i] = building_recieve_message[i];
        }
        reader_position = 0;
        new_recieve_data = 1;
        PORTB &= ~(1<<D4);
    }
    else
    {
        building_recieve_message[reader_position] = tempbyte;
        reader_position++;
        PORTB &= ~(1<<D2);
    }
}

void init_RXTX(void)
{
    cli();
    UBRR0H = (BRC >> 8);
    UBRR0L = BRC;
    UCSR0B |= (1<<RXCIE0) | (0<<TXCIE0) | (1<<RXEN0) | (1<<TXEN0);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    sei();
}

void transmit_data(uint8_t *data)
{
    if (transmiting_data == 0)
    {
        transmiting_data = 1;
        uint8_t i = message_length;
        while(i)
        {
            i--;
            transmit_message[i] = *(data+i);
        }
        UCSR0B |= (1<<UDRIE0);
    }
}

//void create_and_transmit_posdata(uint16_t X, uint16_t Y, uint16_t Z)
//{
//    uint8_t crane_position_message[];
//    uint16_t *position_pointer = crane_position_message +2;
//    crane_position_message[0] = start_byte;
//    crane_position_message[1] = message_identifier_position;
//    position_pointer[0] = X;
//    position_pointer[1] = Y;
//    position_pointer[2] = Z;
//    crane_position_message[8] = end_byte;
//
//
//
//    crane_position_message = {start_byte, 0x00, Xh, Xl,Yh, Yl, Zh, Zl, end_byte}
//    transmit_data(crane_position_message);
//}




