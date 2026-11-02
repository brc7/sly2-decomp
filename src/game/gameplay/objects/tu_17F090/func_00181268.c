struct X { int a; int b; int c; };
struct E { int a[4]; };
struct S { char pad[0x180]; int x180; int x184; char p1[0x38]; int n; float t; char p2[0x48]; struct X x; int p3; struct E ae[1]; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
static __inline__ void ClearX(struct X *px) { px->a = px->b = px->c = 0; }
void func_0017F4C0(struct S *p, struct E *pe);
/** @sly2-annot addr=0x181268
 * Purpose: State switch; from -1 reset health; start state clip (StartCid47Anim)
 * Info: proposed name SetHackenemyState (T2); class HACKENEMY; TU tu_17F090
 * Notes: names_v2 v1 proposed SetStepguardPatrolAnimation__FP9STEPGUARDP4ASEG (0.019) | Cid49->Cid73 (CID
 *   0x49=73, evidence) | class identity from whole_game WE names/object strings: SetCid73State ->
 *   SetHackenemyState
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181268(struct S *p, int n)
{
    if (p->n == n) return;
    if (p->n == -1) p->x184 = p->x180;
    p->n = n;
    p->t = D_002DDED0.t;
    switch (n) {
    case 0: case 1: case 2: case 4:
        ClearX(&p->x);
        break;
    case 3:
        break;
    }
    func_0017F4C0(p, n == -1 ? 0 : &p->ae[n]);
}
