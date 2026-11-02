// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct ALARM { struct ALARM *next; struct ALARM *prev; } ALARM;
extern ALARM *_alarm_head;
ALARM *UnlinkAlarm(ALARM *a)
{
    ALARM *n = a->next;
    if (a->prev) a->prev->next = n;
    else _alarm_head = n;
    if (n) n->prev = a->prev;
    a->prev = 0;
    return n;
}
