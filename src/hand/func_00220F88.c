struct K { int a, b; };
extern struct K D_002A96E8;
extern char D_003DD490[];
void func_001A5848(void *p);
void func_00149B58(void *g, int a, void *p, struct K k, int z);
/** @sly2-annot addr=0x220F88
 * Purpose: RemoveLoHierarchy then clears the TN sw-callback (0x149B58(g_sw,1,ptn,cb))
 * Info: proposed name RemoveTnHierarchy (T1); class TN slot12; TU tn
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00220F88(void *p)
{
    func_001A5848(p);
    func_00149B58(D_003DD490, 1, p, D_002A96E8, 0);
}
