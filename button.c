/*
* RP2040-Button
* Button debounce library for Raspberry Pi Pico
*
* Fork of https://github.com/jkroso/pico-button.c
* including https://github.com/jkroso/pico-gpio-interrupt.c,
* both by Jake Rosoman. MIT license.
*
*/

#ifndef PICO_BUTTON_C
#define PICO_BUTTON_C

#include "pico/stdlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
typedef void (*handler)(void *argument);
typedef struct {
  void * argument;
  handler fn;
} closure_t;
closure_t handlers[28] = {NULL};
void handle_interupt(uint gpio, uint32_t events) {
  closure_t handler = handlers[gpio];
  handler.fn(handler.argument);
}


void listen(uint pin, int condition, handler fn, void *arg) {
  gpio_init(pin);
  gpio_pull_up(pin);
  gpio_set_irq_enabled_with_callback(pin, condition, true, handle_interupt);
  closure_t *handler = malloc(sizeof(closure_t));
  handler->argument = arg;
  handler->fn = fn;
  handlers[pin] = *handler;
}

typedef struct button_t {
  uint8_t pin;
  bool state;
  void (*onchange)(struct button_t *button);
} button_t;

long long int handle_button_alarm(long int a, void *p) {
  button_t *b = (button_t *)(p);
  bool state = gpio_get(b->pin);
  if (state != b->state) {
    b->state = state;
    b->onchange(b);
  }
  return 0;
}

void handle_button_interrupt(void *p) {
  button_t *b = (button_t *)(p);
  bool state = gpio_get(b->pin);
  add_alarm_in_us(200, handle_button_alarm, b, true);
}

button_t * create_button(int pin, void (*onchange)(button_t *)) {
  button_t *b = (button_t *)(malloc(sizeof(button_t)));
  listen(pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, handle_button_interrupt, b);
  b->pin = pin;
  b->onchange = onchange;
  b->state = gpio_get(pin);
  return b;
}
#endif