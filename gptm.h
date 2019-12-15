
#ifndef CAPTURE_TIMER_CAPTURE_TIMER_H
#define CAPTURE_TIMER_CAPTURE_TIMER_H

void CAP0_init(void);
int CAP0_isCaptured(void);
unsigned long CAP0_captureTime(void);
void CAP0_clearCapture(void);

void PERIODIC1_init(void);
unsigned long PERIODIC1_isOverflow(void);
void PERIODIC1_clearOverflow(void);

#endif //CAPTURE_TIMER_CAPTURE_TIMER_H
