void RemoveSoHierarchy(void *p);
void SetButtonButtons(void *p, int n);
/** @sly2-annot addr=0x1492B0
 * Purpose: cid 0x53 vt12: base RemoveSoHierarchy; if pressed, reset buttons to 0
 * Info: proposed name RemoveButtonHierarchy (T2); class BUTTON slot12; TU button
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_001492B0(char *p)
{
    RemoveSoHierarchy(p);
    if ((unsigned int)(*(int *)(p + 0x1E0) - 1) < 2) {
        SetButtonButtons(p, 0);
    }
}
