struct CHK { int a[16]; };
struct X { int flags; struct CHK chk; int a; int b; };
struct M { char pad[0x3C40]; struct CHK chk; };
extern char D_00443740[];
void func_0022B220(void *p, struct X *px, int n, int f);
void func_0020DF80(struct M *pm, struct X *px);
static __inline__ int FSameChk(struct CHK *a, struct CHK *b)
{
    if (a == b) return 1;
    if (!a || !b) return 0;
    return (a->a[2] ^ a->a[14]) == (b->a[2] ^ b->a[14]);
}
/** @sly2-annot addr=0x22B490
 * Purpose: Request a level transition: build a TRANS {flags, CHK of target level (default current), warp
 *   args}; mark "same level" (flags|=2); if wipek != -1 ActivateWipe(g_wipe,&trans,wipek,1) else
 *   SetSwTransition directly
 * Info: proposed name WipeToWorldWarp (T1); TU wipe
 * Notes: top50_round (0.75): Sly 1 wipe.h WipeToWorldWarp(world, oidWarp, wipek) does the same (build TRANS,
 *   ActivateWipe). Callers: CheatWarpEpisode/CheatResetEpisode, FailJob, GoToTask, StartTaskByOid,
 *   PlayGameFromGs, RunNextBootStep, PauseItemRet | runtime5b: transition request =
 *   WipeToWorldWarp(PrecWorldEntry(7), 0x189, ..., 0x30A) from WIDGET131 (S55/S56)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); Sly 1 / Lilly reference; semantic review;
 *   sources: r8_21BB88+top50_round+runtime
 * See: docs/engine/overview.md#3-world--level-lifecycle, docs/engine/world-sw.md#2-loading-a-level,
 *   docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue
 * @end-sly2-annot */
void func_0022B490(struct M *pm, struct CHK *pchk, int a, int b, int flags, int n)
{
    struct X x;
    struct CHK *pchkCur = &pm->chk;
    struct CHK *pchkX = &x.chk;
    *pchkX = *(pchk ? pchk : pchkCur);
    x.a = a;
    x.b = b;
    x.flags = flags;
    if (FSameChk(pchkX, pchkCur)) x.flags |= 2;
    if (n != -1)
        func_0022B220(D_00443740, &x, n, 1);
    else
        func_0020DF80(pm, &x);
}
