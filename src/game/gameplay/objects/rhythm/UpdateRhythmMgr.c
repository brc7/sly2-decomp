struct E { int n0; int n4; int n8; float gC; int pad10; int n14; int n18; int n1C; int n20; };
struct S {
    char pad0[0x50]; int n50; char pad54[0x5C - 0x54]; float g5C;
    char pad60[0x1C0 - 0x60]; int c1C0; struct E a1C4[3];
    int n230; char pad234[0x284 - 0x234]; float g284; float g288; char x28C[0x18]; float g2A4;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD040[];
extern char D_002DEB30[];
void UpdateLo(struct S *);
float TMusicCur(void *);
void func_00152390(int, int, int, int);
void BroadcastGameEvent(void *, int, struct S *);
void AddRhythmScore(struct S *, float);
float GSmooth__FfffP3SMPPf(void *, float, float, float, int);
void func_00152B58(struct S *);
/** @sly2-annot addr=0x152970
 * Purpose: cid 0x79 vt16 per-frame: music time t = 0x1FE3F8(&g_sndmgr); state 1 plays cue moves when due,
 *   state 2 posts msg 0x34 per prompt and scores misses (AddRhythmScore g2A4); smooth displayed score;
 *   UpdateRhythmMgrGrades
 * Info: proposed name UpdateRhythmMgr (T1); class CID121 slot16; TU rhythm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/minigames.md#7-dance-dancer-0x44--rhythm-manager-cid121-0x79-rhythm-tu-0x1520c8-0x1539a8-ccode,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void UpdateRhythmMgr(struct S *p)
{
    float t;
    int i, j;
    struct E *pe;
    UpdateLo(p);
    t = TMusicCur(D_003DD040);
    switch (p->n50) {
    case 1:
        for (j = 0; j < p->c1C0; j++) {
            pe = &p->a1C4[j];
            if (pe->n14)
                continue;
            if (pe->gC < t) {
                func_00152390(pe->n0, pe->n4, pe->n8, 0);
                pe->n14 = 1;
                break;
            }
        }
        break;
    case 2:
        for (i = 0; i < p->c1C0; i++) {
            pe = &p->a1C4[i];
            if (pe->n20)
                continue;
            if (!pe->n18 && pe->gC < t) {
                p->n230 = pe->n4;
                BroadcastGameEvent(D_002DEB30, 0x34, p);
                pe->n18 = 1;
            }
            if (pe->gC + p->g5C < t) {
                pe->n20 = 1;
                if (!pe->n1C) {
                    func_00152390(pe->n0, pe->n4, pe->n8, 2);
                    AddRhythmScore(p, p->g2A4);
                }
                break;
            }
        }
        break;
    }
    { float dt = D_002DDED0.dt; p->g288 = GSmooth__FfffP3SMPPf(p->x28C, p->g288, p->g284, dt, 0); }
    func_00152B58(p);
}
