struct G { char pad[0x3B0]; int a; };
struct S { char pad[0x16C]; void *p; };
extern struct G D_002DDF40;
void func_00221268(void *p, int f);
void func_0014E328(struct G *pg, int n, void *pv, void *p, int m);
void func_0014E4C8(struct G *pg, int a, int b, void *c, void *d, int e);
/** @sly2-annot addr=0x1C3FF0
 * Purpose: PO virtual slot 78 (Lilly OnActive(int)): if PO has its own camera TN (+0x16C) (de)activate it,
 *   else SetCmPolicy / RevokeCmPolicy on g_cm
 * Info: proposed name OnPoActive (T1); class PO slot78; TU po
 * Notes: names_v2 B proposed DroppedRobRoh__FP3ROBP3ROH (0.923)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void OnPoActive(struct S *p, int f)
{
    if (f) {
        if (p->p) func_00221268(p->p, 1);
        else func_0014E328(&D_002DDF40, 1, &D_002DDF40.a, p, 0);
    } else {
        if (p->p) func_00221268(p->p, 0);
        else func_0014E4C8(&D_002DDF40, 0xF, 1, &D_002DDF40.a, p, 0);
    }
}
