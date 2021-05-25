#ifndef __UTIL_H
#define __UTIL_H

#include <avr/sleep.h>


#define INTERNAL2V56_NO_CAP (6)
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)

#endif
