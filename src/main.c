#include "milis.h"
#include "stm8s.h"

#define DIN_PORT GPIOD          // data in
#define DIN_PIN GPIO_PIN_4
#define CS_PORT GPIOD           // chip select
#define CS_PIN GPIO_PIN_5
#define CLK_PORT GPIOD          // clock
#define CLK_PIN GPIO_PIN_6

#define LOW(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)
#define HIGH(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define REVERSE(BAGR) GPIO_WriteReverse(BAGR##_PORT, BAGR##_PIN)

#include "max.h"

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
}

void send_max(uint8_t address, uint8_t data) {
    uint8_t mask;

    LOW(CS);   // zahájení komunikace
    
    // posílám adresu
    mask = 1 << 7;
    mask = 0b10000000;
    while (mask) {
        if (address & mask) {   // nachystám DIN
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);    // vytvořím hodinový puls
        mask = mask >> 1;
        LOW(CLK);
    }

    // posílám data
    mask = 1 << 7;
    while (mask) {
        if (data & mask) {   // nachystám DIN
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);    // vytvořím hodinový puls
        mask = mask >> 1;
        LOW(CLK);
    }

    HIGH(CS);  // ukončení komunikace
}


int main(void)
{
    int32_t time = 0;
    uint8_t bagr = 1;

    setup();
    LOW(CS);

    send_max(DECODE_MODE, 0b01111111); // zapnout znakovou sadu na všech cifrách
    send_max(SCAN_LIMIT, 7); // velikost displeje 8 cifer (počítáno od nuly, proto je argument číslo 7)
    send_max(INTENSITY, 3); // volíme ze začátku nízký jas (vysoký jas může mít velkou spotřebu - až 0.25A !)
    send_max(DISPLAY_TEST, 0); //
    send_max(SHUTDOWN, 1);           // zapneme displej

    send_max(7, 0);
    send_max(6, 0);
    send_max(5, 0);
    send_max(4, 0);
    send_max(3, 6);
    send_max(2, 9);
    send_max(1, 7);

    while (1) {
        if (milis() - time > 333) {
            time = milis();
            send_max(8, bagr);
            bagr <<= 1;
            if (bagr == 0) {
                bagr = 1;
            }
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
