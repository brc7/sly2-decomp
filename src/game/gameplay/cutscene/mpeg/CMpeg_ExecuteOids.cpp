struct E { int a[16]; };
struct S { int n; E ae[4]; int x104; int x108; };
extern "C" unsigned int CbFreeMem(int n);
extern "C" int func_001B2820(E *pe);
extern "C" void Execute__5CMpegPv(S *p, E *pe);
/** @sly2-annot addr=0x1B3B08
 * Purpose: Play every queued movie (Execute until done), unless CbFreeMem<=0x41FC3F; called during level
 *   transition
 * Info: proposed name CMpeg_ExecuteOids (T1); class CMpeg; TU mpeg
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#2-fmv-playback-mpegcpp-0x1b2820-0x1b4438,
 *   docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
extern "C" int CMpeg_ExecuteOids(S *p)
{
    int i;
    if (p->n <= 0) return 0;
    if (CbFreeMem(1) <= 0x41FC3F) { p->n = 0; return 0; }
    for (i = 0; i < p->n; i++) {
        E *pe = &p->ae[i];
        p->x104 = func_001B2820(pe);
        do { Execute__5CMpegPv(p, pe); } while (p->x108);
        p->x104 = 0;
    }
    p->n = 0;
    return 1;
}
