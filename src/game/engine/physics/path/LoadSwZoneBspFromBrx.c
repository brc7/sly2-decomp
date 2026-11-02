struct MSG { int cb; int nKind; };
struct G { char pad[0x45CC]; void *p45cc; };
int U32Read__18CBinaryInputStream(void *pbis);
void *func_001AE2A0(struct MSG *pmsg);
void LoadZoneBspFromBrx(void *p, int n, void *pbis);
/** @sly2-annot addr=0x1C0EC0
 * Purpose: Read world zone BSP (if count) into SW+0x45CC
 * Info: proposed name LoadSwZoneBspFromBrx (T1); TU path
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5b_rest+parse-verified
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx
 * @end-sly2-annot */
void LoadSwZoneBspFromBrx(struct G *p, void *pbis)
{
    int n = U32Read__18CBinaryInputStream(pbis);
    if (n) {
        struct MSG msg;
        msg.cb = 0x30;
        msg.nKind = 0x10;
        p->p45cc = func_001AE2A0(&msg);
        LoadZoneBspFromBrx(p->p45cc, n, pbis);
    }
}
