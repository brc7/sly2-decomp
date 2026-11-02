typedef struct VECTOR { float x, y, z; } VECTOR;
struct S { int n; char pad[0xC]; VECTOR v; char pad2[0x8C]; int a; };
/** @sly2-annot addr=0x201D78
 * Purpose: recon selector init: owner, origin = screen centre (320,246.4), state -1
 * Info: proposed name InitReconSel (T1); TU tu_201D78
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/core-library.md#6-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void InitReconSel(struct S *p, int a)
{
    VECTOR *pv = &p->v;
    float y;
p->a = a;
pv->x = 0;
pv->y = 0;
pv->z = 0;
    y = pv->y;
    pv->x = pv->x * 320.0f + 320.0f;
    pv->y = 246.40000915527344f;
    pv->z = y;
    p->n = -1;
}
