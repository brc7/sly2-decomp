struct S { char pad[0x13d0]; int st; };
int BENTLEY_PlayerStateCur(struct S *p) {
    switch (p->st) {
    case 0: return 0;
    case 2: return 1;
    case 3: return 2;
    case 4: return 3;
    case 9: return 4;
    case 10: return 5;
    case 11: return 6;
    case 12: return 7;
    case 13: return 8;
    }
    return -1;
}
