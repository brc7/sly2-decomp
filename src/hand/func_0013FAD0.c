int func_00210A88(void);
/** @sly2-annot addr=0x13FAD0
 * Purpose: True if global D_003E1110 == -1 or appears in the object's S16 filter list
 * Info: proposed name FMatchLoadFilter (T1); TU brx
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#loadswobjectsfrombrx-0x13fb40-matched,
 *   docs/formats/meshes-scene.md#4-scene-assembly-and-placement,
 *   docs/formats/meshes-scene.md#devtools-findings-v3-2026-11-01
 * @end-sly2-annot */
int func_0013FAD0(int c, short *a) {
    int x = func_00210A88();
    int i;
    short *ps;
    if (x == -1) return 1; for (i = 0; ; i++) { if (i >= c) return 0; if (a[i] == x) break; } return 1;
}
