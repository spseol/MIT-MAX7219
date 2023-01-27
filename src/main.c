#include "milis.h"
#include "stm8s.h"

#define DIN_PORT GPIOD // data in
#define DIN_PIN GPIO_PIN_4
#define CS_PORT GPIOD // chip select
#define CS_PIN GPIO_PIN_5
#define CLK_PORT GPIOD // clock
#define CLK_PIN GPIO_PIN_6

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

void setup(void) {
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

  GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

  init_milis();
}


int main(void) {
  setup();

  while (1) {
    HIGH(CS);
  }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
