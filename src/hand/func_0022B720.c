struct S { char pad[0x40]; int f; char pad2[0xC]; float g; char pad3[8]; int h; char pad4[0x680]; int i; char pad5[0x20C]; char buf[0xA0]; };
void InitLo(struct S *p);
void func_0011A050(void *pv, int cb);
void SetLoBusyk(struct S *p, int a);
static inline int FIsPtr(int f) { return !(f & 1); }
/** @sly2-annot addr=0x22B720
 * Purpose: InitLo; -1 fields, g=-1.0, busyk, grf
 * Info: proposed name InitWr (T1); class WR slot7; TU wr
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/animation.md#6-source-files, docs/engine/world-sw.md#9-source-files,
 *   docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void func_0022B720(struct S *p)
{
    int f;
    InitLo(p);
    p->h = -1;
    p->i = -1;
    p->g = -1.0f;
    func_0011A050(p->buf, 0xA0);
    SetLoBusyk(p, 1);
    f = p->f;
    if (FIsPtr(f)) *(int *)f |= 2;
    else p->f = f | 2;
}
