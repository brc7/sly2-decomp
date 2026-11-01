struct VTE { short d; short pad; void *(*pfn)(void *, int); };
struct MSG { int cb; int nKind; };
struct Q { void *p0; void *p4; void *p8; void *pc; char pad[0x90]; char xa0[0x10]; char xb0[0x10]; };
struct S { char pad[0xC]; struct VTE *vt; char p1[0x138]; unsigned long fl; };
int func_00117BD0(void *pbis);
int func_00117D50(void *pbis);
void func_00117EB0(void *pbis, void *pv);
void *func_001AE2A0(struct MSG *pmsg);
void func_00186930(struct Q *q);
void func_0012DD28(struct S *p, void *pv, int n, struct Q *q);
static __inline__ void *PvLookup(struct S *p, void *pbis)
{
    struct VTE *e = &p->vt[51];
    return e->pfn((char *)p + e->d, func_00117D50(pbis));
}
/** @sly2-annot addr=0x186670
 * Purpose: Read N IK records (kind 0x10, 0xE0 bytes: two bones + parent + vectors), init each (0x186930),
 *   create actuators (PactNewC); set SO flag bit 36
 * Info: proposed name LoadSoIkFromBrx (T1); TU ik
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest+parse-verified
 * See: docs/engine/animation.md#animation-format-summary-anim_lab-v31,
 *   docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#6-so-payload-loadsofrombrx-0x1f31e0-cparse
 * @end-sly2-annot */
void LoadSoIkFromBrx(struct S *p, void *pbis)
{
    int c = func_00117BD0(pbis);
    int i;
    if (!c) return;
    p->fl |= 1UL << 36;
    for (i = 0; i < c; i++) {
        struct MSG msg;
        struct Q *q;
        msg.cb = 0xE0;
        msg.nKind = 0x10;
        q = (struct Q *)func_001AE2A0(&msg);
        q->p4 = PvLookup(p, pbis);
        q->p8 = PvLookup(p, pbis);
        q->pc = PvLookup(p, pbis);
        func_00117EB0(pbis, q->xb0);
        q->p0 = PvLookup(p, pbis);
        func_00117EB0(pbis, q->xa0);
        func_00186930(q);
        func_0012DD28(p, q->p4, -1, q);
        func_0012DD28(p, q->p8, -1, q);
    }
}
