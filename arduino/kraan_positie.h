#include <stdint-gcc.h>
#ifndef KRAAN_POSITIE_H_INCLUDED
#define KRAAN_POSITIE_H_INCLUDED

void init_motors(void);
void control_x(int8_t mode, int16_t position);
void control_y(int8_t mode, int16_t position);
void control_z(int8_t mode, int16_t position);
#endif // KRAAN_POSITIE_H_INCLUDED
