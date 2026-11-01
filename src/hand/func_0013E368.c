struct E { int id; void *pv; };
struct S { char pad[0x4640]; int c; struct E *a; };
extern char D_002A4BE0[];
/** @sly2-annot addr=0x13E368
 * Purpose: Linear search of SW.alangstr by id; returns string or "" (D_002A4BE0) if id==-1 or not found
 * Info: proposed name PchzFindSwLangstr (T1); TU brx
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#9-source-files, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/sound.md#tu_2275b8-0x2275b8-0x227bb8
 * @end-sly2-annot */
void *func_0013E368(struct S *p, int id) {
    int i;
    if (id == -1) return D_002A4BE0;
    for (i = 0; i < p->c; i++) {
        if (p->a[i].id == id) return p->a[i].pv;
    }
    return D_002A4BE0;
}
