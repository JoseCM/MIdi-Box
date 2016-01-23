#ifndef __GPIO_H__
#define __GPIO_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

typedef enum {IN, OUT} DIR;
typedef enum {LOW, HIGH} VAL;

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init_pin(uint8_t gpio, DIR direction);
int gpio_set_pin(uint8_t gpio, VAL value);
int gpio_read_pin(uint8_t gpio);
void gpio_deinit_pin(uint8_t gpio);

#ifdef __cplusplus
}
#endif

#endif
