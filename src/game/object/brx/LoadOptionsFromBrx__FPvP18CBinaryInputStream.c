struct E { int f0; int f4; char pad[0x20]; };
extern struct E D_002AC880[];
int func_00117D50(void *);
void LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream(void *, struct E *, int, void *);
/** @sly2-annot addr=0x13F810
 * Purpose: Loop: S16 eopid until <0; LoadOptionFromBrx(pv, &g_aeopid[eopid] (D_002AC880 stride 0x28), grf &
 *   0x2580, pbis)
 * Info: proposed name LoadOptionsFromBrx (T1); TU brx
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels;
 *   reads every object option list (LEVEL_FORMAT 3.1)
 * Confidence: high [T1 0.97] - evidence: semantic review; sources: r2_131F70+parse-verified
 * See: docs/engine/object-model.md#9-key-functions, docs/engine/splice-scripting.md#5-objects-and-scripts,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file
 * @end-sly2-annot */
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(void *p, void *pbis) {
    int n;
    while ((n = func_00117D50(pbis)) >= 0) {
        struct E *pe = &D_002AC880[n];
        LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream(p, pe, pe->f4 & 0x2580, pbis);
    }
}
