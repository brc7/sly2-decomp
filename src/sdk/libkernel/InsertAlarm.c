// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// 26/33: normal mode (target has beql = gcc reorg ON). Structure right; regs differ (target al=a2,key=a3,p=a1) and
// target has the 2.96-style `nop` after the `b` (loop alignment). 2.96: 25/34. Tried 5 loop shapes, -fstrict-aliasing.
typedef struct ALARM { struct ALARM *next; struct ALARM *prev; int pad[2]; unsigned long a; unsigned long b; unsigned long c; } ALARM;
typedef struct { int pad[6]; ALARM *head; } ALARMQ;
extern ALARMQ D_002C5A30;
void InsertAlarm(ALARM *al)
{
    ALARM *prev = 0, *p;
    unsigned long key = al->c + al->a - al->b;
    p = D_002C5A30.head;
    while (p) {
        if (key < p->c + p->a - p->b) break;
        prev = p;
        p = prev->next;
    }
    al->prev = prev;
    al->next = p;
    if (p) p->prev = al;
    if (prev) prev->next = al;
    else D_002C5A30.head = al;
}
