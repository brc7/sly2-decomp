struct P { char pad[0x4C0]; char sub[0x13D0 - 0x4C0]; int n13D0; };
int func_00194640(void *p, int k);
int STEPPLAYER_FInvulnerable(struct P *p, int k);
int func_001B83B8(struct P *p, int k)
{
    switch (p->n13D0) {
    case 20:
        if (k == 0x4000)
            return 1;
        break;
    case 15:
        return 1;
    case 13:
        if (func_00194640(p->sub, k))
            return 1;
        break;
    }
    return STEPPLAYER_FInvulnerable(p, k);
}
