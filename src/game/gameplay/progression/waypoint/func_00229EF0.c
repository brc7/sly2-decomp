typedef unsigned long ulong;
struct S { char pad[0x40]; int f; char pad2[0x104]; ulong g; int pad3; int a; float b, c; char pad4[0x18]; int d; char pad5[8]; int e; };
void InitSo(struct S *p);
static inline int FIsPtr(int f) { return !(f & 1); }
/** @sly2-annot addr=0x229EF0
 * Purpose: InitSo; 50, 300.0, ...
 * Info: proposed name InitWaypt (T1); class WAYPT slot7; TU waypoint
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/game-progression.md#7-source-files, docs/engine/physics-collision.md#6-source-files,
 *   docs/reference/functions/game-progression.md#waypoint-0x229ef0-0x22b040
 * @end-sly2-annot */
void func_00229EF0(struct S *p)
{
    int f;
    InitSo(p);
    f = p->f;
    p->a = 50;
    p->b = 300.0f;
    p->c = 500.0f;
    p->d = -1;
    p->e = -1;
    if (FIsPtr(f)) *(int *)f |= 0xA;
    else p->f = f | 0xA;
    p->g &= ~(1L << 35);
}
