struct K { int a, b; };
struct S { char pad[0x1B4]; struct K k; };
void func_001E5DB8(struct S *p);
/** @sly2-annot addr=0x1C34B8
 * Purpose: PIPE vt41 Clone
 * Info: proposed name ClonePipe (T1); class PIPE slot41; TU pipe
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#pipe-0x1c3390-0x1c3d70
 * @end-sly2-annot */
void func_001C34B8(struct S *p)
{
    struct K k = p->k;
    func_001E5DB8(p);
    p->k = k;
}
