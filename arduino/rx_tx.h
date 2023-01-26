#ifndef RX_TX_H_INCLUDED
#define RX_TX_H_INCLUDED
#define message_length 10
extern volatile uint8_t new_recieve_data;
extern volatile uint8_t recieve_message[message_length];
void init_RXTX(void);
void transmit_data(uint8_t *data);
#endif // RX_TX_H_INCLUDED
