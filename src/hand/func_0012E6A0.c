extern char D_002DDF40[];
void AddLoHierarchy(void *p);
void func_0014E328(void *a, int b, void *c, int d, void *e);
typedef struct T { char pad[0x124]; int f; } T;
/** @sly2-annot addr=0x12E6A0
 * Purpose: slot 11: AddLoHierarchy (0x1A5800) + SetCmPolicy
 * Info: proposed name AddCid44Hierarchy (T2); class CID44 slot11; TU tu_12E600
 * Notes: slot 11 is AddHierarchy (CONFLICTS#5: LO base 0x1A5800/0x1A5848 AddLoHierarchy/RemoveLoHierarchy;
 *   OnAdd/OnRemove are slots 23/22 per arbitration); was OnCid44Add
 * Confidence: low (draft) [T2 0.45] - evidence: arbitration; sources: r1_116918
 * See: docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48
 * @end-sly2-annot */
void func_0012E6A0(T *t)
{
    AddLoHierarchy(t);
    if (t->f)
        func_0014E328(D_002DDF40, 10, D_002DDF40 + 0x4A8, 0, t);
}
