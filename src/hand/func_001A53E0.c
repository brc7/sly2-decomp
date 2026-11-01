extern char D_003DD490[];
struct S { char pad[0x14]; void *pv; };
int func_0011A380(void *, void *);
void func_0011A2E8(void *, void *);
void func_00147E20(void *, void *);
void func_0011CD98(void *, void *);
/** @sly2-annot addr=0x1A53E0
 * Purpose: Unlink plo->ploroot from g_sw+0x2C list and g_sw+0x20 busy list (0x147E20), free it to pool
 *   g_sw+0x398C, clear x14
 * Info: proposed name FreeLoroot (T1); TU lo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A53E0(struct S *p)
{
    char *pdl = D_003DD490 + 0x2C;
    if (func_0011A380(pdl, p->pv))
        func_0011A2E8(pdl, p->pv);
    func_00147E20(D_003DD490, p->pv);
    func_0011CD98(D_003DD490 + 0x398C, p->pv);
    p->pv = 0;
}
