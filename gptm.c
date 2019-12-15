#include "gptm.h"
#include "tm4c123gh6pm.h"

void CAP0_init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;
    while ((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5 == 0) || (SYSCTL_PRTIMER_R&SYSCTL_PRTIMER_R0 == 0));

    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R = 0x1;
    GPIO_PORTF_DIR_R &= ~0x1;
    GPIO_PORTF_DEN_R |= 0x1;
    GPIO_PORTF_AFSEL_R |= 0x1;
    GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&(~0xF) | 0x7);
    GPIO_PORTF_PUR_R |= 0x1;

    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
    TIMER0_CFG_R = 0x4;
    TIMER0_TAMR_R = 0x7;
    TIMER0_TAILR_R = 0x23FF;
    TIMER0_TAPR_R = 0xF4;
    TIMER0_CTL_R |= TIMER_CTL_TAEN;

}

int CAP0_isCaptured(void) {
    return (TIMER0_RIS_R & TIMER_RIS_CAERIS);
}

unsigned long CAP0_captureTime() {
    return TIMER0_TAR_R;
}

void CAP0_clearCapture(void) {
    TIMER0_ICR_R = 4;
}

void PERIODIC1_init(void) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
    while (SYSCTL_PRTIMER_R&SYSCTL_PRTIMER_R1 == 0);

    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;
    TIMER1_CFG_R = 0x4;
    TIMER1_TAMR_R = 0x11;
    TIMER0_TAILR_R = 0x23FF;
    TIMER0_TAPR_R = 0xF4;
    TIMER1_CTL_R |= TIMER_CTL_TAEN;
}

unsigned long PERIODIC1_isOverflow(void) {
    return (TIMER0_RIS_R & TIMER_RIS_TATORIS);
}

void PERIODIC1_clearOverflow(void) {
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;
}
