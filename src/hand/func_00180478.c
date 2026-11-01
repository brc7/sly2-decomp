struct T { char pad[0x60]; void *p60; };
struct S { char pad[0xEC]; struct T *pt; char p1[0xD0]; float t1c0; float g1c4; int n; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
void func_00134320(void *g, struct S *p);
/** @sly2-annot addr=0x180478
 * Purpose: Set mode field and recalc object filters (RecalcSwOxfFilterForObject)
 * Info: proposed name SetHackbulletMode (T2); class HACKBULLET; TU tu_17F090
 * Notes: Cid4A->Cid74 (CID 0x4A=74, hex-literal) | class identity from whole_game WE names/object strings:
 *   SetCid74Mode -> SetHackbulletMode
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00180478(struct S *p, int n)
{
    if (n != p->n) {
        p->n = n;
        if (p->pt->p60 == p) func_00134320(D_003DD490, p);
    }
    p->t1c0 = (p->g1c4 == 0.0f) ? 3.40282346638528859812e+38f : D_002DDED0.t + p->g1c4;
}
