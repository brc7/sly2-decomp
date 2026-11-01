struct S { char pad[0x18C8]; int n; };
int GetJtPlayerState(struct S *p)
{
    switch (p->n) {
    case 0: return 0;
    case 2: return 1;
    case 7: return 2;
    case 4: return 3;
    case 18: return 4;
    case 19: return 5;
    case 20: return 6;
    case 23: return 7;
    case 8: return 8;
    }
    return -1;
}
