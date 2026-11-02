typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x30 - 0x10]; int n30; char pad3[0x40 - 0x34]; qword q40; };
void ClearBhvstk__FP6BHVSTK(void *p);
struct O *func_00205438(void *p, int n);
/** @sly2-annot addr=0x142540
 * Purpose: Splice optid 446 on STEPPLAYER: clear the BHV stack and, if a leader is given, push BHVFOLLOW (cid
 *   194) with leader +0x30 and params +0x40/+0x44, then Start (AI partner follow)
 * Info: proposed name StartStepplayerFollow (T1); TU stepplayer
 * Notes: v3: superseded StartStepplayerBhvC2 | runtime6 N2/P2: TASK t1_recon_brawl calls optid 446 on Murray
 *   (JT, (85,180,0)); BHV194 +0x30 = JT, +0x40/+0x44 = 85/180; v2 StartStepplayerBhvC2
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime6
 * See: docs/engine/player.md#8-look-mode-recon-mode-and-the-ai-partner,
 *   docs/engine/player.md#11-key-functions,
 *   docs/guides/naming-workflow.md#1-where-things-stand-master_names_v2
 * @end-sly2-annot */
void StartStepplayerFollow(char *p, int a, qword q)
{
    struct O *o;
    struct VTE *e;
    ClearBhvstk__FP6BHVSTK(p + 0x26C);
    if (a) {
        o = func_00205438(p, 0xC2);
        o->n30 = a;
        o->q40 = q;
        e = &o->vt[8];
        ((void (*)(void *))e->pfn)((char *)o + e->d);
    }
}
