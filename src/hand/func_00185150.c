struct MSG { int cb; int nKind; };
struct X { int a; int b; int c; };
struct V { int a[4]; };
struct S { char pad[0xE0]; int cv; struct V *av; int cx; struct X *ax1; struct X *ax2; };
void LoadXfmXfFromBrx(struct S *p, void *pbis);
int func_00117D50(void *pbis);
void func_00117EB0(void *pbis, void *pv);
void *func_001AE1D0(struct MSG *pmsg);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(struct S *p, void *pbis);
static __inline__ void *PvAllocMsg(struct MSG *pmsg, int cb, int nKind) { pmsg->nKind = nKind; pmsg->cb = cb; return func_001AE1D0(pmsg); }
/** @sly2-annot addr=0x185150
 * Purpose: CID 8/9 vt14: LoadXfmXfFromBrx, read vertex count + vectors (triangle mesh) into PvAllocSw,
 *   LoadOptionsFromBrx
 * Info: proposed name LoadCid8FromBrx (T2); class CID8 slot14; TU tu_185130
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   verifier PLAUSIBLE: Placeholder class. CID 8/9 vtable slot 14: LoadXfmXfFromBrx, count+qword vectors,
 *   count + two arrays of int triples, LoadOptionsFromBrx. Purpose omits the two triple arrays
 *   (index/adjacency?), so 'tri | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources:
 *   r4b_rest+parse-verified+verdicts_followups
 * See: docs/engine/player.md#11-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185150(struct S *p, void *pbis)
{
    struct MSG msg;
    int i, j;
    int n;
    LoadXfmXfFromBrx(p, pbis);
    p->cv = func_00117D50(pbis);
    p->av = (struct V *)PvAllocMsg(&msg, p->cv << 4, 0x10);
    for (i = 0; i < p->cv; i++)
        func_00117EB0(pbis, &p->av[i]);
    p->cx = func_00117D50(pbis);
    p->ax1 = (struct X *)PvAllocMsg(&msg, p->cx * 12, 0x10);
    for (j = 0; j < p->cx; j++) {
        struct X *px = &p->ax1[j];
        px->a = func_00117D50(pbis);
        px->b = func_00117D50(pbis);
        px->c = func_00117D50(pbis);
    }
    p->ax2 = (struct X *)PvAllocMsg(&msg, p->cx * 12, 0x10);
    for (j = 0; j < p->cx; j++) {
        p->ax2[j].a = func_00117D50(pbis);
        p->ax2[j].b = func_00117D50(pbis);
        p->ax2[j].c = func_00117D50(pbis);
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(p, pbis);
}
