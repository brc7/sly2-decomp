/* StartBhvko (Claude agent lane G2, 2026-10-24). Count() is an inline whose else arm starts with the
   for-init `n = 0` (keeps the target's bltz/then-first layout). The v3E0 vector is read into q before the call. */
typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD490;
extern char D_002DEB30[];
extern char D_002DE578[];
struct E { int n0; int pad[2]; int nC; };
struct A {
    char pad[0xEC]; void *pEC; char padF0[0x330-0xF0]; int n330; char pad2[0x3D8 - 0x334]; int n3D8; char pad3[4]; qword v3E0; int n3F0;
    char pad4[0x404 - 0x3F4]; struct E ae[(0x484 - 0x404) / 16]; int n484; int n488;
    char pad5[0xF64 - 0x48C]; void *pF64;
};
struct S { char pad[0x10]; struct A *p10; char pad2[0x2C]; int *p40; char pad3[0xC]; void *p50; char pad4[8]; int mask5C; };
int func_001F78A0(int mask);
void StartBhvhit(struct S *p);
void *PpoHero(struct G *g);
void BroadcastGameEvent(void *a, int n, struct A *p);
qword ConvertAloPos(struct A *source, void *target, qword v);
void func_00154E10(void *a, qword v, int n, int m, struct A *p);
void RecalcSwOxfFilterForObject(struct G *g, struct A *p);
void func_00153FF8(void *p);
static inline int Count(struct A *a) {
    int n, i;
    if (a->n3D8 >= 0) {
        n = a->n3D8;
    } else {
        for (i = a->n488, n = 0; i < a->n484; i++) {
            struct E *e = &a->ae[i];
            if (e->n0 == 0x2B3) n += e->nC;
        }
        n = (n + 1) / 2;
    }
    return n;
}
/** @sly2-annot addr=0x179898
 * Purpose: BHVKO Start: entry from table +0xC10, BHVHIT Start; first time: mark KO (+0x330), broadcast 0x18
 *   (by hero) or 0x17, drop coins from the loot list, reset detector
 * Info: proposed name StartBhvko (T1); class BHVKO slot8; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded: RenderAloLine
 *   (names_v2:B(unverified)) | runtime2/4 confirmed the BHVKO role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvko(struct S *p) {
    int n;
    struct A *a;
    int *pe = (int *)((char *)p->p10 + (func_001F78A0(p->mask5C) * 16 + 0xC10));
    p->p40 = pe;
    if (*pe == -1) p->p40 = 0;
    StartBhvhit(p);
    a = p->p10;
    if (a->n330 == 0) {
        a->n330 = 1;
        if (p->p50 == PpoHero(&D_003DD490))
            BroadcastGameEvent(D_002DEB30, 0x18, p->p10);
        else
            BroadcastGameEvent(D_002DEB30, 0x17, p->p10);
        n = Count(p->p10);
        {
            qword q = p->p10->v3E0;
            qword v;
            /* Empty barrier: the target re-reads p->p10 for arg0 after loading q instead of reusing the register. */
            __asm__ __volatile__("" ::: "memory");
            v = ConvertAloPos(p->p10, 0, q);
            func_00154E10(D_002DE578, v, n, p->p10->n3F0, p->p10);
        }
        RecalcSwOxfFilterForObject(&D_003DD490, p->p10);
        func_00153FF8(p->p10->pF64);
    }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_E[(sizeof(struct E)==16)?1:-1];
typedef char layout_A_prefix[(sizeof(struct A)==0xF70)?1:-1];
typedef char layout_S_prefix[(sizeof(struct S)==0x60)?1:-1];
typedef char layout_A_align[(__alignof__(struct A)==16)?1:-1];
typedef char layout_A_pEC[((unsigned)&((struct A*)0)->pEC==0xec)?1:-1];
typedef char layout_A_n330[((unsigned)&((struct A*)0)->n330==0x330)?1:-1];
typedef char layout_A_n3D8[((unsigned)&((struct A*)0)->n3D8==0x3d8)?1:-1];
typedef char layout_A_v3E0[((unsigned)&((struct A*)0)->v3E0==0x3e0)?1:-1];
typedef char layout_A_n3F0[((unsigned)&((struct A*)0)->n3F0==0x3f0)?1:-1];
typedef char layout_A_ae[((unsigned)&((struct A*)0)->ae==0x404)?1:-1];
typedef char layout_A_n484[((unsigned)&((struct A*)0)->n484==0x484)?1:-1];
typedef char layout_A_n488[((unsigned)&((struct A*)0)->n488==0x488)?1:-1];
typedef char layout_A_pF64[((unsigned)&((struct A*)0)->pF64==0xf64)?1:-1];
typedef char layout_S_p10[((unsigned)&((struct S*)0)->p10==0x10)?1:-1];
typedef char layout_S_p40[((unsigned)&((struct S*)0)->p40==0x40)?1:-1];
typedef char layout_S_p50[((unsigned)&((struct S*)0)->p50==0x50)?1:-1];
typedef char layout_S_mask5C[((unsigned)&((struct S*)0)->mask5C==0x5c)?1:-1];
typedef char layout_E_n0[((unsigned)&((struct E*)0)->n0==0x0)?1:-1];
typedef char layout_E_nC[((unsigned)&((struct E*)0)->nC==0xc)?1:-1];
typedef char layout_A_entry_capacity[(sizeof(((struct A*)0)->ae)/sizeof(struct E)==8)?1:-1];
