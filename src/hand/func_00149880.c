struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
void RemoveLoHierarchy(void *p);
void SetVolbtnButtons(void *p, int n);
/** @sly2-annot addr=0x149880
 * Purpose: vt12: base, untrigger if pressed
 * Info: proposed name RemoveVolbtnHierarchy (T2); class VOLBTN slot12; TU button
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00149880(char *p)
{
    RemoveLoHierarchy(p);
    if ((unsigned int)(*(int *)(p + 0x1C0) - 1) < 2) {
        SetVolbtnButtons(p, 0);
        if (*(int *)(p + 0x2C8)) {
            struct VTE *e;
            *(int *)(p + 0x2C0) = 0;
            e = &((struct O *)p)->vt[3];
            ((void (*)(void *, int, int, int, int, int))e->pfn)(p + e->d, 9, 0, 0, 0, 0);
        }
    }
}
