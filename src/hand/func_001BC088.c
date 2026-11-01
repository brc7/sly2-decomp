struct MSG { int cb; int nKind; };
struct S { char pad[0xE8]; int c; char pad2[0x1EC - 0xEC]; void *p; };
void LoadShapeFromBrx(struct S *p);
void *func_001AE2A0(struct MSG *pmsg);
static __inline__ void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
void func_001BC088(struct S *p)
{
    struct MSG msg;
    LoadShapeFromBrx(p);
    p->p = PvAllocMsg(&msg, (p->c << 4) - 0x10, 0x10);
}
