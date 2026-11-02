extern char D_002DDF40[];
void RemoveLoHierarchy(void *p);
void RevokeCmPolicy(void *a, int b, int c, void *d, int e, void *f);
typedef struct T { char pad[0x124]; int f; } T;
/** @sly2-annot addr=0x12E6E8
 * Purpose: slot 12: RemoveLoHierarchy (0x1A5848) + RevokeCmPolicy (0x14E4C8)
 * Info: proposed name RemoveCid44Hierarchy (T2); class CID44 slot12; TU tu_12E600
 * Notes: slot 12 is RemoveHierarchy (CONFLICTS#5: LO base 0x1A5800/0x1A5848 AddLoHierarchy/RemoveLoHierarchy;
 *   OnAdd/OnRemove are slots 23/22 per arbitration); was OnCid44Remove
 * Confidence: low (draft) [T2 0.45] - evidence: arbitration; sources: r1_116918
 * See: docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48
 * @end-sly2-annot */
void func_0012E6E8(T *t)
{
    RemoveLoHierarchy(t);
    if (t->f)
        RevokeCmPolicy(D_002DDF40, 15, 10, D_002DDF40 + 0x4A8, 0, t);
}
