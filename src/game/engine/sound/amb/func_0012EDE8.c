typedef struct T { char pad[0xE4]; int c; int pad2; void **ap; } T;
void OnLoRemove(T *p);
void StopSnd(void *pv, float g);
void func_00258040(void **pp, int n);
/** @sly2-annot addr=0x12EDE8
 * Purpose: slot 22: LO::Freeze then StopSnd
 * Info: proposed name OnCid43Remove (T2); class CID43 slot22; TU amb
 * Notes: names_v2 C proposed FIsAloStatic__FP3ALO (0.438) | slot 22 OnRemove resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was CID43_Freeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; static naming pass; sources:
 *   arbitration_slots22_23
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0
 * @end-sly2-annot */
void func_0012EDE8(T *p)
{
    int i;
    OnLoRemove(p);
    for (i = 0; i < p->c; i++) {
        void **pp = (void **)((i << 2) + (int)p->ap);
        if (pp && *pp) {
            StopSnd(*pp, 0.0f);
            func_00258040(pp, 0);
        }
    }
}
