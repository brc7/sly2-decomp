typedef struct REC { int id; float a, b, c, d; } REC;
typedef struct H { void *p; } H;
typedef struct T { char pad[0xE4]; int c; REC *arec; H *ap; } T;
void OnLoAdd(T *t);
void *func_001FBB90(int id);
void func_001FB2F8(void *p, T *t);
void func_001FB4F0(void *p, float a, float b);
void func_001FB1F8(void *p, float c);
void func_001FB2C0(void *p, float d);
void func_00258040(H *pp, void *p);
/** @sly2-annot addr=0x12ED20
 * Purpose: slot 23: LO::Unfreeze then starts its ambient sound (0x1FBB90/0x1FB2F8, SetSndRange)
 * Info: proposed name OnCid43Add (T2); class CID43 slot23; TU amb
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was CID43_Unfreeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0
 * @end-sly2-annot */
void func_0012ED20(T *t)
{
    int i;
    OnLoAdd(t);
    for (i = 0; i < t->c; i++) {
        REC *prec = &t->arec[i];
        void *p = func_001FBB90(prec->id);
        func_001FB2F8(p, t);
        func_001FB4F0(p, prec->a, prec->b);
        func_001FB1F8(p, prec->c);
        func_001FB2C0(p, prec->d);
        func_00258040((H *)(i * 4 + (int)t->ap), p);
    }
}
