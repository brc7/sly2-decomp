struct K { int a, b; };
extern struct K D_002A6790;
void *PsndStartSound(int n);
void SetSndSourceLo(void *p, int n);
void SetJtHurtState(void *p, int n);
void func_001A15B0(void *p, struct K k);
/** @sly2-annot addr=0x1A2028
 * Purpose: Callback: sound 0x6A, SetJtHurtState(6), chain next callback D_002A6790
 * Info: proposed name JtHurtShockCb (T2); TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A2028(void *p)
{
    SetSndSourceLo(PsndStartSound(0x6A), 0);
    SetJtHurtState(p, 6);
    func_001A15B0(p, D_002A6790);
}
