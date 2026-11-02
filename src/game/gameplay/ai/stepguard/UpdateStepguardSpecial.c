extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3D40[3];
struct S { char pad[0xFBC]; int k; float t; };
void SetStepguardSpecial(struct S *p, int n);
/** @sly2-annot addr=0x207288
 * Purpose: End the special state after D_002C3D40 = 10 s (8/0xD), 30 s (0x22/0x25), 5 s (0x15)
 * Info: proposed name UpdateStepguardSpecial (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void UpdateStepguardSpecial(struct S *p)
{
    switch (p->k) {
    case 8:
    case 13:
        if (D_002DDED0.t - p->t > D_002C3D40[0])
            SetStepguardSpecial(p, -1);
        break;
    case 34:
    case 37:
        if (D_002DDED0.t - p->t > D_002C3D40[1])
            SetStepguardSpecial(p, -1);
        break;
    case 21:
        if (D_002DDED0.t - p->t > D_002C3D40[2])
            SetStepguardSpecial(p, -1);
        break;
    }
}
