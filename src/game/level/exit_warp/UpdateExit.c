typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD490;
extern char D_002F66B0[];
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0xEC]; void *xEC; char pad1[0x138 - 0xF0]; int x138; int k; float t140; int x144; int c; int *a; int x150; int x154; float t158; float dt; };
void UpdateLo(struct S *p);
void CMpeg_QueueMovie(void *a, int b);
void WipeToWorldWarp(struct G *p, int a, int b, int c, int d, int e);
void *PpoHero(struct G *pg);
qword func_001A62E8(void *p);
qword ConvertAloPos(int a, struct S *p, qword q);
int PbspPointInBspQuick(void *p, int n, qword q);
void SetExitExits(struct S *p, int k);
void func_002283D0(struct S *p);
/** @sly2-annot addr=0x228538
 * Purpose: UpdateLo then player inside test (BSP) and trigger handling
 * Info: proposed name UpdateExit (T1); class EXIT slot16; TU exit_warp
 * Notes: CID26 EXIT Update = Sly1 UpdateExit (player-in-volume state machine, wipe after tWipe) (was
 *   UpdateWarp)
 * Confidence: mid (draft) [T1 0.70] - evidence: Sly 1 / Lilly reference; sources:
 *   verify_B+verify_r7r8_summary+verdicts_B
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void UpdateExit(struct S *p)
{
    UpdateLo(p);
    switch (p->k) {
    case 4:
        if (0.0f < p->t158 && p->t158 <= D_002DDED0.t) {
            int i;
            for (i = 0; i < p->c; i++)
                CMpeg_QueueMovie(D_002F66B0, p->a[i]);
            WipeToWorldWarp(&D_003DD490, p->x144, p->x150, -1, p->x138 ? 0x20 : 0, p->x154);
            p->t158 = 0.0f;
        }
        break;
    case 0: case 1: case 3: {
        void *pj = PpoHero(&D_003DD490);
        union { qword q; } u;
        int f;
        if (!pj) return;
        u.q = ConvertAloPos(0, p, func_001A62E8(pj));
        f = PbspPointInBspQuick(p->xEC, 0x7FFFFFFF, u.q) != 0;
        switch (p->k) {
        case 3:
            if (f) func_002283D0(p);
            break;
        case 1:
            if (f) SetExitExits(p, 0);
            else if (D_002DDED0.t - p->t140 > p->dt) SetExitExits(p, 3);
            break;
        case 0:
            if (!f) SetExitExits(p, 1);
            break;
        }
        break;
    }
    case 2:
        break;
    }
}
