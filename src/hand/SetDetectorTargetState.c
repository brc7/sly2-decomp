typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
extern char D_002DEB30[];
struct VTE { short d; short pad; void (*pfn)(); };
struct E { int id; int n; float t; float tC; };
struct O { int a[3]; struct VTE *vt; void *p10; };
struct ARGS { struct E *pe; int *pn; int *pnNew; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void BroadcastGameEvent(void *p, int n, void *q);
/** @sly2-annot addr=0x154080
 * Purpose: Changes a target entry state: records times, notifies the detector through its HandleSpliceEvent
 *   (slot 3) virtual (msg 0x1C, args entry/old/new) and, when a player-class object becomes detected (state
 *   1), broadcasts game event 0x13
 * Info: proposed name SetDetectorTargetState (T1); TU detector
 * Notes: verifier: purpose partly wrong: Detector TU; entry {id,n,t,tC}: stamps times, sends msg 0x1C via vt3
 *   with (entry,&old,&new). On state 1 the STEP-class (cid 0x33) test is on the detector's own | names_v2 C
 *   proposed LoadStepguardAnimations__FP9STEPGUARD (0.200) | arbitration follow-up: the virtual it calls is
 *   slot 3 HandleSpliceEvent, not SendMessage
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
void SetDetectorTargetState(struct O *o, struct E *pe, int nNew)
{
    struct ARGS args;
    struct VTE *e;
    if (!pe || nNew == pe->n)
        return;
    if (pe->n == 0)
        pe->tC = D_002DDED0.t;
    args.pe = pe;
    args.pn = &pe->n;
    args.pnNew = &nNew;
    e = &o->vt[3];
    e->pfn((char *)o + e->d, 0x1C, 0, 3, &args, 0);
    pe->n = nNew;
    pe->t = D_002DDED0.t;
    if (nNew == 1) {
        void *q = o->p10;
        if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x33))
            BroadcastGameEvent(D_002DEB30, 0x13, q);
    }
}
