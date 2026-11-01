struct M { int a, b, n8; };
void SetLeapState(void *p, int n);
int HandleBasicMessage(void *p, int msg, struct M *pm);
int HandleLeapMessage(void *p, int msg, struct M *pm) { if (msg == 9 && pm->n8 == 0x22) SetLeapState(p, 1); return HandleBasicMessage(p, msg, pm); }
