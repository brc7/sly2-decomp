typedef int qword __attribute__((mode(TI)));
typedef struct VTE { short d; short pad; void (*pfn)(); } VTE;
typedef struct I { int a; int b; } I;
typedef struct E { int id; int n; int c; void *a; } E;
typedef struct P {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x150 - 0x10];
    int c150;
    I *x154;
    int x158;
    int i;
    int c;
    E *ae;
    int x168;
    void *x16C;
    int x170;
} P;
extern char D_003DD490[];
extern char D_002DEB30[];
int func_001BBE48(P *p);
int func_001BBE18(P *p);
void *PpoHero(void *g);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
void func_001BB648(P *p, E *pe);
qword func_001A62E8(P *p);
void EmitSwNoise(void *g, int z, qword q, void *x);
void BroadcastGameEvent(void *g, int k, P *p);

/** @sly2-annot addr=0x1BB400
 * Purpose: Callback: classify instigator (player 0xE, zap kinds -> 8), run current stage actions (0x1BB648),
 *   stimulus 0x156B98, BroadcastGameEvent, advance stage
 * Info: proposed name ReactCid98 (T2); class CID98; TU tu_1BAB50
 * Notes: names_v2 C proposed FIsJsgJsgeComplete__FP3JSGP4JSGE (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BB400(P *p)
{
    E *pe;
    int v;
    if (func_001BBE48(p)) return;
    if (p->x16C == PpoHero(D_003DD490)) {
        p->x168 = 0xE;
    } else {
        v = p->x170;
        if (v == 0x100 || v == 0x10000 || v == 0x20000 || (p->x16C && FIsBasicDerivedFrom__FP5BASIC3CID(p->x16C, 0x3B)))
            p->x168 = 8;
        else
            p->x168 = 0;
    }
    ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 0x10, 0, 0, 0, 0);
    if (p->x168 & 1) return;
    pe = &p->ae[p->i];
    func_001BB648(p, p->ae - 1);
    func_001BB648(p, pe);
    if (!p->x16C || !FIsBasicDerivedFrom__FP5BASIC3CID(p->x16C, 0x38) || FIsBasicDerivedFrom__FP5BASIC3CID(p->x16C, 0x3C)) {
        EmitSwNoise(D_003DD490, 0, func_001A62E8(p), p->x16C);
    }
    if (pe->n > 0) {
        if (++p->x158 >= pe->n) {
            p->x158 = 0;
            if (++p->i >= p->c) {
                ((void (*)(void *, int, P *))p->vt[4].pfn)((char *)p + p->vt[4].d, 0x10, p);
                ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
                BroadcastGameEvent(D_002DEB30, 0x11, p);
            }
        }
    }
    if (func_001BBE18(p)) {
        pe = &p->ae[p->i];
        if (pe->n <= 0) {
            int i;
            for (i = 0; i < p->c150; i++) p->x154[i].b = 0;
        }
    }
}
