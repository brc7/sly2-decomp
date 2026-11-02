struct M { int cb; int a; };
struct S { char pad[0x144]; int x; int c; int *a; };
void LoadVolFromBrx(struct S *p, void *q);
void LoadWarpBlobFromBrx(void *q, int *p);
int func_00117BD0(void *q);
int *func_001AE1D0(struct M *pm);
/** @sly2-annot addr=0x2282B0
 * Purpose: LoadVolFromBrx then blob + U8 count of entries
 * Info: proposed name LoadExitFromBrx (T1); class EXIT slot14; TU exit_warp
 * Notes: verifier WRONG: LoadWarpFromBrx -> LoadExitFromBrx | verifier: purpose partly wrong: CID 0x1A slot
 *   14, a VOL child: LoadVolFromBrx, then a 0x40-byte file record (0x228250), a U8 count, and that many more
 *   records. The class is Sly 1's EXIT, not W | names_v2 C proposed SetStepguardGoal__FP9STEPGUARDP6VECTOR
 *   (0.638) | CID26 is EXIT (VOL child), not WARP | level_parser CONFIRMED-parse: payload layout followed by
 *   the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   verify_r7r8+CONFLICTS#11+verdicts_tu_r7r8+parse-verified
 * See: docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14
 * @end-sly2-annot */
void LoadExitFromBrx(struct S *p, void *q)
{
    int i = 0;
    int n;
    struct M m;
    LoadVolFromBrx(p, q);
    LoadWarpBlobFromBrx(q, &p->x);
    n = func_00117BD0(q);
    m.a = 0x10;
    m.cb = n * 4;
    p->c = n;
    p->a = func_001AE1D0(&m);
    for (; i < p->c; i++) LoadWarpBlobFromBrx(q, &p->a[i]);
}
