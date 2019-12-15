
#ifndef CAPTURE_TIMER_CAPTURE_TIMER_H
#define CAPTURE_TIMER_CAPTURE_TIMER_H

void PERIODIC1_init();
int PERIODIC1_isOverflow(void);
unsigned long PERIODIC1_curValue(void);
void PERIODIC1_clearOverflow(void);

#endif //CAPTURE_TIMER_CAPTURE_TIMER_H
