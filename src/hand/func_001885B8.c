typedef struct { float x, y, z; } VEC3;
struct S { char pad[0x240]; VEC3 v; };
extern char D_002BE990[];
void func_002039C0(struct S *p);
void func_001A64E8(struct S *p, int n, void *pv);
/** @sly2-annot addr=0x1885B8
 * Purpose: JT vt14: STEP LoadFromBrx 0x2039C0, set x240 vector=(0,0,-75), BindLoOidTable(pjt,0xAD,table)
 * Info: proposed name LoadJtFromBrx (T1); class JT slot14; TU jt
 * Notes: runtime: +0x240 = (0,0,-75) confirmed | level_parser CONFIRMED-parse: payload layout followed by the
 *   parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources: r4_17F090+runtime+parse-verified
 * See: docs/engine/player.md#11-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_001885B8(struct S *p)
{
    VEC3 *pv;
    func_002039C0(p);
    pv = &p->v;
    pv->x = 0.0f;
    pv->y = 0.0f;
    pv->z = -75.0f;
    func_001A64E8(p, 0xAD, D_002BE990);
}
