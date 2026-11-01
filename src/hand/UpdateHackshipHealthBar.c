struct S { char pad[0x180]; int c; int n; char p1[0x118]; void *p2a0; };
void func_00212E40(void *p, int n, float g);
/** @sly2-annot addr=0x180100
 * Purpose: Set widget anim 0x34F phase = 1 - health/max (clamped)
 * Info: proposed name UpdateHackshipHealthBar (T1); class HACKSHIP; TU tu_17F090
 * Notes: Cid48->Cid72 (CID 0x48=72, evidence) | class identity from whole_game WE names/object strings:
 *   UpdateCid72HealthBar -> UpdateHackshipHealthBar
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void UpdateHackshipHealthBar(struct S *p)
{
    if (p->p2a0) {
        int n = p->n;
        float g;
        if (n <= 0) n = 0;
        g = 1.0f - (float)n / (float)p->c;
        if (g < 0.0f) g = 0.0f;
        else if (g > 1.0f) g = 1.0f;
        func_00212E40(p->p2a0, 0x34F, g);
    }
}
