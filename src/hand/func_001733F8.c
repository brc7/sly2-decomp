struct G;
extern struct G D_003DD490;
struct O { char pad[0x54]; int n54; };
struct E { int n0; int n4; void *p8; struct O *pC; int n10; int pad[2]; };
struct S { char pad[0x40]; unsigned int u40; char pad2[0xC]; int n50; struct E *a54; int pad3; int n5C; void *p60; };
struct AL { int cb; int n; };
void PostLoLoad(struct S *p);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int a, int b, int c);
struct O *PloNew__F3CIDP2SO3OIDi(int a, int b, int c, int d);
void func_00170C50(struct O *o, int a, int b, int c);
void *func_001AE2A0(struct AL *a);
void SetLoBusyk(struct S *p, int n);
/** @sly2-annot addr=0x1733F8
 * Purpose: Resolves selector oids; for kind 1 creates a cid 0x71 script with a goto; allocates per-slot
 *   entries; marks busy
 * Info: proposed name PostCid119Load (T2); class CID119 slot15; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, vtable)
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_001733F8(struct S *p) {
    int i;
    struct AL al;
    unsigned int f;
    PostLoLoad(p);
    for (i = 0; i < p->n50; i++) {
        struct E *e = &p->a54[i];
        switch (e->n0) {
        case 1:
            {
                struct O *o;
                e->p8 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, e->n4, 0);
                o = PloNew__F3CIDP2SO3OIDi(0x71, 0, 0x2CB, -1);
                func_00170C50(o, e->n4, 1, -1);
                e->n10 = o->n54;
                e->pC = o;
            }
            break;
        case 0:
        case 2:
            e->p8 = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, e->n4, 0);
            break;
        }
    }
    al.cb = p->n5C * 0x30;
    al.n = 0x10;
    p->p60 = func_001AE2A0(&al);
    SetLoBusyk(p, 1);
    f = p->u40;
    if ((f ^ 1) & 1) *(int *)f |= 2;
    else p->u40 = f | 2;
}
