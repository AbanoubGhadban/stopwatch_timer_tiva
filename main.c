#include "lcd.h"
#include "gptm.h"
#include "tm4c123gh6pm.h"

#define SW (1U << 4)

void initSwitch(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    while (SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5 == 0);

    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R |= SW;
    GPIO_PORTF_DIR_R &= ~SW;
    GPIO_PORTF_DEN_R |= SW;
    GPIO_PORTF_PUR_R |= SW;
}

int main() {
    int seconds;
    int isRunning;

    initSwitch();
    LCD_init();
    LCD_start();

    CAP0_init();
    PERIODIC1_init();

    seconds = 0;
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

            if (CAP0_isCaptured()) {
                unsigned long capCount = CAP0_captureTime();
                double time = (capCount * 62.5)/1000000000L;
                LCD_clear();
                LCD_int(seconds);
                LCD_data('.');
                LCD_int((int)(time*1000000000));
                isRunning = 0;
                CAP0_clearCapture();
            }
        } else if (!(GPIO_PORTF_DATA_R&SW)) {
            isRunning = 1;
            seconds = 0;
            LCD_clear();
            LCD_int(seconds);
            CAP0_init();
            PERIODIC1_init();
        }
    }
}