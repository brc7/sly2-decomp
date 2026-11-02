extern struct CLOCK { float t; float dt; } D_002DDED0;
typedef struct VT1 { struct { short d; short pad; void (*pfn)(void *, int); } a[64]; } VT1;
typedef struct { char pad[0xC]; VT1 *vt; } W;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *, int, int, int, int, int); } a[8]; } VT;
typedef struct OBJ {
    char pad[0xC];
    VT *vt;
    char pad10[0x180 - 0x10];
    int state;
    float t;
    char pad188[0x568 - 0x188];
    W *pw;
    int pad56c;
    void *x570;
    char pad574[0x598 - 0x574];
    void *x598;
    int x59C;
} OBJ;
extern int D_002C2208[];
void PlayWidgetAnim(W *, int);
void UpdateRobottankHealthMeter(OBJ *);
void *PsndStartSound(int);
void SetSndSourceLo(void *, OBJ *);
void RcPtr_Assign(void **, void *);
void SetSndRange(void *, float, float);
void PauseSnd(void *);
void StopSnd(void *, float);
void func_001D4B28(OBJ *, int);
void SetSmaGoalIfReachable(void *, int);
/** @sly2-annot addr=0x1D4940
 * Purpose: Car state (x180): 0 idle (show meter, start engine sound), 1, 2 active, 3 destroyed (vt3 msg 5);
 *   drives body SMA goal from D_002C2208[state]
 * Info: proposed name SetRobottankState (T1); TU tu_1D1370
 * Notes: names_v2 v1 proposed ProjectMatchTransform__FP5MATCHf (0.042) | class identity from whole_game WE
 *   names/object strings: SetRcCarState -> SetRobottankState
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void SetRobottankState(OBJ *p, int n)
{
    W *pw;
    if (n == p->state)
        return;
    switch (p->state) {
    case 0:
        pw = p->pw;
        if (pw) {
            pw->vt->a[49].pfn((char *)pw + pw->vt->a[49].d, -1);
            PlayWidgetAnim(p->pw, 0x34F);
            UpdateRobottankHealthMeter(p);
        }
        if (p->x59C != -1) {
            void *ps = PsndStartSound(p->x59C);
            SetSndSourceLo(ps, p);
            RcPtr_Assign(&p->x598, ps);
            SetSndRange(p->x598, 500.0f, 1500.0f);
            PauseSnd(p->x598);
        }
        break;
    case 1:
        func_001D4B28(p, -1);
        break;
    }
    p->state = n;
    p->t = D_002DDED0.t;
    switch (n) {
    case 0: {
        W *pw2 = p->pw;
        if (pw2)
            ((void (*)(void *))pw2->vt->a[50].pfn)((char *)pw2 + pw2->vt->a[50].d);
        {
            void **pps = &p->x598;
            if (*pps) {
                StopSnd(*pps, 0.0f);
                RcPtr_Assign(pps, 0);
            }
        }
        break;
    }
    case 1:
        func_001D4B28(p, 0);
        break;
    case 3:
        p->vt->a[3].pfn((char *)p + p->vt->a[3].d, 5, 0, 0, 0, 0);
        break;
    }
    if (p->x570 && p->state != -1)
        SetSmaGoalIfReachable(p->x570, D_002C2208[p->state]);
}
