struct S { char pad[0x13D0]; int n; };
int GetMurrayPlayerState(struct S *p)
{
    switch (p->n) {
    case 0: case 5: return 0;
    case 2: return 1;
    case 13: return 2;
    case 14: return 3;
    case 16: return 4;
    case 17: return 5;
    case 18: return 6;
    case 19: return 7;
    case 20: return 8;
    }
    return -1;
}
