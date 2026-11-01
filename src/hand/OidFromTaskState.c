struct S { char pad[0x54]; int n; };
int OidFromTaskState(struct S *p)
{
    switch (p->n) {
    case 0: return 0x612;
    case 1: return 0x613;
    case 2: return 0x614;
    case 3: return 0x615;
    }
    return -1;
}
