// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char cmd; unsigned char id; } WQE;
typedef struct { int x0; int idx; WQE q[0x200]; } WQ;
extern int D_00448940;
extern WQ D_00448948;
extern int D_002C49E8;
int func_00234960(int);
int func_00234A50(int);
int iWakeupThread(int id)
{
    int cur;
    WQ *q;
    int i;
    __asm__ volatile("addiu $3, $0, -0x2F\n\tsyscall\n\tmove %0, $2" : "=r"(cur) : : "$2", "$3");
    if (cur != id)
        return func_00234960(id);
    if ((unsigned)cur >= 0x100 || D_002C49E8 == 0)
        return -1;
    q = &D_00448948;
    i = q->idx & 0x1FF;
    q->idx = i + 1;
    q->q[i].cmd = 0;
    q->q[i].id = cur;
    func_00234A50(D_00448940);
    return cur;
}
