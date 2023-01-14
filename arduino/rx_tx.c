void init_rx_tx(){
    cli();
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 0;
    OCR0A = 249; //top
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    TIMSK0 |= (1 << OCIE0A);
    sei();

}
