typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
struct E { char *pchz; int n4; int n8; int nC; char ab[0x40]; };
struct S { char pad[0x50]; int c; struct E *ae; };
extern char D_002DD590[];
void func_001A5C28(struct S *p, void *pbis);
int func_00117BD0(void *pbis);
int func_00117D00(void *pbis);
int func_00117DB8(void *pbis);
void CBinaryInputStream_ReadStringSw(void *pbis, char **ppchz, void *palloc, int nKind);
void CBinaryInputStream_ReadCb64(void *pbis, void *pv);
void func_00153B08(struct S *p, void *pbis)
{
    MSG msg;
    int i;
    func_001A5C28(p, pbis);
    p->c = func_00117BD0(pbis);
    p->ae = PvAllocMsg(&msg, p->c * sizeof(struct E), 0x10);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        CBinaryInputStream_ReadStringSw(pbis, &pe->pchz, D_002DD590, 7);
        pe->n4 = func_00117D00(pbis);
        pe->n8 = func_00117DB8(pbis);
        pe->nC = func_00117DB8(pbis);
        CBinaryInputStream_ReadCb64(pbis, pe->ab);
    }
}
