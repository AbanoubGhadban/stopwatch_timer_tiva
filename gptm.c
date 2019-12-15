#include "gptm.h"
#include "tm4c123gh6pm.h"

void PERIODIC1_init() {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
    while (SYSCTL_PRTIMER_R&SYSCTL_PRTIMER_R1 == 0);

    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;
    TIMER1_CFG_R = TIMER_CFG_32_BIT_TIMER;
    TIMER1_TAMR_R = (TIMER_TAMR_TAMR_PERIOD | TIMER_TAMR_TACDIR);
    TIMER1_TAILR_R = 16000000;
    TIMER1_CTL_R |= TIMER_CTL_TAEN;
}

int PERIODIC1_isOverflow(void) {
    return (int) (TIMER1_RIS_R & TIMER_RIS_TATORIS);
}

unsigned long PERIODIC1_curValue(void) {
    return TIMER1_TAV_R;
}

void PERIODIC1_clearOverflow(void) {
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;
}
