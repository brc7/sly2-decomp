struct S { char pad[0xE8]; float a, b, c; int f; };
void InitXfm(struct S *p);
/** @sly2-annot addr=0x21D3D8
 * Purpose: InitXfm; 0xE8=2000, 0xEC=-800, 0xF0=800, 0xF4=1
 * Info: proposed name InitCid21 (T2); class CID21 slot7; TU target
 * Notes: agent qualifier: target-like XFM class
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#target-0x21d278-0x21d638
 * @end-sly2-annot */
void func_0021D3D8(struct S *p)
{
    InitXfm(p);
    p->a = 2000.0f;
    p->b = -800.0f;
    p->c = 800.0f;
    p->f = 1;
}
