struct S { char pad[0x234]; int x; char pad2[0x268 - 0x238]; char y[4]; };
void RemoveSoHierarchy(struct S *p);
void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(int a, struct S *b, int c, void *d, float f, float g);
void ReleaseAct(void *p);
/** @sly2-annot addr=0x1B1E38
 * Purpose: MISSILE::OnRemove (vt12): OnSoRemove (0x1F2D48) then retract current asega
 * Info: proposed name OnMissileRemove (T1); class MISSILE slot12; TU missile
 * Notes: slot 12 is RemoveHierarchy by position, but the body matches Sly 1 OnMissileRemove; Sly 1 name kept
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; sources: r5_1AE130
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void OnMissileRemove__FP7MISSILE(struct S *p)
{
    RemoveSoHierarchy(p);
    ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(0, p, 0, &p->x, 0.0f, 1.0f);
    ReleaseAct(p->y);
}
