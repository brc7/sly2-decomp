// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct Alarm {
    struct Alarm *next;
    int x4;
    int (*func)(int, int, int, void *);
    void *arg;
} Alarm;
extern Alarm * volatile D_0044E700;
int func_0023A668(int id, int a1, int a2, Alarm *p)
{
    int r = p->func(((int)p << 4) | (id & 0xFE) | 1, a1, a2, p->arg);
    if (r == 0) {
        Alarm *h = D_0044E700;
        D_0044E700 = p;
        p->next = h;
        p->x4 = 0;
        return -1;
    }
    return r;
}
