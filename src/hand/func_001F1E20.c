struct E { int n; int pad[2]; int nC; };
struct P { char pad[0x404]; struct E ae[8]; int c484; int i488; int pad2[2]; void *p494; };
struct G;
extern struct G D_002DEB30;
void func_001619A0(struct G *, int, struct P *);
void *func_001C2EF0(void *, int);
void func_001F17F8(struct P *, int);
void func_001F1CF8(struct P *);
/** @sly2-annot addr=0x1F1E20
 * Purpose: Sly pickpockets the guard: BroadcastGameEvent(0x19), launch the staged loot (count) from the pouch
 *   toward the player (0x1C2EF0), state -1, advance to next entry, stage it; returns the launched object
 * Info: proposed name PickpocketStepguard (T1); TU pickpocket
 * Notes: runtime: pickpocket pouch is CID99 (SO child), not a PO
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime-corrected
 * See: docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void *func_001F1E20(struct P *p)
{
    void *r;
    if (p->i488 >= p->c484) return 0;
    {
        func_001619A0(&D_002DEB30, 0x19, p);
        r = func_001C2EF0(p->p494, (p->ae + p->i488)->nC);
        func_001F17F8(p, -1);
        p->i488++;
        func_001F1CF8(p);
    }
    return r;
}
