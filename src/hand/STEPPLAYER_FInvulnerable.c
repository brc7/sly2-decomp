extern void *D_003D4A54;
int FIsPoCur(void *p);
int func_001A2230(void *p, int a);
int STEPPLAYER_FInvulnerable(char *p, int a)
{
    if (*(int *)(p + 0xF20) == 1) return 1;
    if (*(int *)(p + 0x2DC) == 0x15) return 1;
    if (D_003D4A54 && FIsPoCur(D_003D4A54)) return 1;
    return func_001A2230(p + 0xDF0, a);
}
