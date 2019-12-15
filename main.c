#include "lcd.h"
#include "gptm.h"
#include "tm4c123gh6pm.h"

#define SW1 (1U << 0)
#define SW2 (1U << 4)
#define RED_LED (1U << 1)

void initSwitchAndLed(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    while (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5 == 0);

    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= (SW1 | SW2 | RED_LED);
    GPIO_PORTF_DIR_R &= ~(SW1 | SW2);
    GPIO_PORTF_DIR_R |= RED_LED;
    GPIO_PORTF_DEN_R |= (SW1 | SW2 | RED_LED);
    GPIO_PORTF_PUR_R |= (SW1 | SW2);
}

int main() {
    int seconds;
    int isRunning;
    int blinking;

    initSwitchAndLed();
    LCD_init();
    LCD_start();

    seconds = 0;
    blinking = -1;
    isRunning = 0;
    LCD_clear();
    LCD_int(seconds);
    while (1) {
        if (isRunning) {
            if (PERIODIC1_isOverflow()) {
                ++seconds;
                LCD_clear();
                LCD_int(seconds);
                PERIODIC1_clearOverflow();
            }

            if (!(GPIO_PORTF_DATA_R&SW2)) {
                unsigned long capCount = PERIODIC1_curValue();
                double time = (capCount * 62.5)/1000000000L;
                LCD_clear();
                LCD_int(seconds);
                LCD_data('.');
                LCD_int((int)(time*1000000000));
                isRunning = 0;
                blinking = 0;
            }
        } else if (!(GPIO_PORTF_DATA_R&SW1)) {
            isRunning = 1;
            seconds = 0;
            blinking = -1;
            LCD_clear();
            LCD_int(seconds);

            PERIODIC1_init();
            PERIODIC1_clearOverflow();
            GPIO_PORTF_DATA_R &= ~RED_LED;\
        } else if (blinking >= 0 && blinking <= 5 && PERIODIC1_isOverflow()) {
            PERIODIC1_clearOverflow();
            GPIO_PORTF_DATA_R ^= RED_LED;
            ++blinking;
        }
    }
}