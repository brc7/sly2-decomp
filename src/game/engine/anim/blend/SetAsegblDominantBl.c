struct X { char pad[0x80]; float f80; float f84; void *f88; void *f8c; char pad2[0x9a - 0x90]; short f9a; };
struct C { int f0; int f4; struct X *px; char pad[0x1c - 0xc]; int f1c; char pad2[0x28 - 0x20]; struct C *pnext; };
struct A { char pad[0x2c]; struct A *pnext; int f30; struct C *pcFirst; };
struct S { char pad[0x50]; struct A *paFirst; char pad2[0xd4 - 0x54]; int fd4; };
extern const float D_002A4AF0;
extern char D_002C44E0[];
int func_0012CEE0(struct C *);
/** @sly2-annot addr=0x1383F0
 * Purpose: When the index of the last bl with weight > 0.9999 changes, store it (+0xD4) and reset the damping
 *   of every live ACT on the blended aseg channels
 * Info: proposed name SetAsegblDominantBl (T1); TU blend
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_followups
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#blend-0x137a30-0x138f08
 * @end-sly2-annot */
void SetAsegblDominantBl(struct S *p, int n) {
    struct A *pa;
    struct C *pc;
    if (n == p->fd4) return;
    p->fd4 = n;
    for (pa = p->paFirst; pa; pa = pa->pnext) {
        for (pc = pa->pcFirst; pc; pc = pc->pnext) {
            if (pc->f1c && func_0012CEE0(pc)) {
                struct X *px = pc->px;
                px->f80 = px->f84 = D_002A4AF0; px->f88 = px->f8c = D_002C44E0; px->f9a = 0;
            }
        }
    }
}
