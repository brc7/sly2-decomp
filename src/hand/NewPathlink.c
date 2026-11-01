typedef struct BASIC { int m0; int m4; int m8; void *pvtbl; } BASIC;
extern char D_002D5120[];
void *func_001AE4F8(int cb);
void func_002562A8(void *pv);

static inline BASIC *CtorBasic(BASIC *p)
{
    p->pvtbl = D_002D5120;
    p->m0 = 0;
    p->m4 = 0;
    return p;
}

void *NewPathlink(void)
{
    BASIC *p = CtorBasic((BASIC *)func_001AE4F8(0x160));
    char *pb = (char *)p + 0x60;
    int i = 5;
    do {
        func_002562A8(pb);
        i--;
        pb += 0x20;
    } while (i != -1);
    return p;
}
