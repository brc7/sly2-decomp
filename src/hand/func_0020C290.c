struct M { int a, b, n8; };
void func_0020C2F8(void *p, int n);
int HandleBasicMessage(void *p, int msg, struct M *pm);
int func_0020C290(void *p, int msg, struct M *pm) { if (msg == 9 && pm->n8 == 0x22) func_0020C2F8(p, 1); return HandleBasicMessage(p, msg, pm); }
