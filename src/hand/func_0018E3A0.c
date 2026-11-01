struct K { int a; int b; };
extern struct K D_002A5F98;
extern char D_003DD490[];
void OnStepplayerActive(void *p);
void EnsureSwCallback(void *, void *, struct K, int);
/** @sly2-annot addr=0x18E3A0
 * Purpose: JT vt78: base 0x144480 then 0x149AB8(g_sw, pjt, D_002A5F98, 0)
 * Info: proposed name OnJtActive (T2); class JT slot78; TU jt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018E3A0(void *p)
{
    OnStepplayerActive(p);
    EnsureSwCallback(D_003DD490, p, D_002A5F98, 0);
}
