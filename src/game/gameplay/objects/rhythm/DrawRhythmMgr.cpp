// DrawRhythmMgr (Claude agent, lane D17): MATCH. The last diff was the daddu a0/sb order in the key loop: in sched1 the f12 = scale arg move
// (reg weight 0) took the t=1 alu slot ahead of a0 = &fontBig (weight +1), so a0 got a later luid than the a[0] store. Pinning scale to $f12 removes that move.
typedef int qword __attribute__((mode(TI)));
struct FONT { char ab[0x84]; float dy; char pad[0xA0 - 0x88]; };
struct RGBA { unsigned int word; };
template <class T> static inline T *Addr(T *p) { return p; }
struct TB {
    float x, y, dx, dy;
    RGBA color;
    int jh;
    int jv;
    void SetPos(float x, float y) __asm__("func_00258438");
    void SetSize(float dx, float dy) __asm__("func_00258448");
    void SetColor(RGBA *prgba) __asm__("func_00258458");
    void SetHorizontalJust(int j) __asm__("func_00258470");
    void SetVerticalJust(int j) __asm__("func_00258478");
    TB() {}
    void Init(float x, float y, float dx, float dy, unsigned int word, RGBA *prgba) {
        SetPos(x, y);
        SetSize(dx, dy);
        RGBA *p = Addr(prgba);
        p->word = word;
        SetColor(p);
        SetHorizontalJust(0);
        SetVerticalJust(0);
    }
};
struct SZ { char a[6]; };
struct KEY { int id; unsigned char ch; float dx, dy; };
struct ENT { float t; int k; char pad[8]; qword pos; int fDone; char pad2[0x30 - 0x24]; };
struct P {
    char pad[0x50]; int state; char pad54[0x5C - 0x54]; float dt; int c; char pad64[0x70 - 0x64];
    ENT ae[6]; int stateHint; int iHint; char pad198[0x1A0 - 0x198]; qword pos; qword vel;
};
struct CLOCK { float t; float dt; };
extern "C" {
extern CLOCK D_002DDED0;
extern char D_003DD040[];
extern float D_002BDB50, D_002BDB54, D_002BDB58, D_002BDB5C, D_002BDBB0;
extern KEY D_002BDB70[4];
extern const SZ D_002A5018;
extern qword D_002BDBC0, D_002BDBD0;
void RenderLoSelf(P *p);
float TMusicCur(void *p);
FONT *CRichText_ctor(FONT *pf, const char *sz, float scale);
float CRichText_Dx(FONT *pf);
void func_002163B0(FONT *pf, TB *ptb, int grf);
void EvaluateBezierPos(float dt, float t, float dtScale, qword p0, qword v0, qword p1, qword v1, qword *ppos, qword *pvel, qword *pacc);
void DrawRhythmMgr(P *p);
}

static inline float GMax(float a, float b) { return a < b ? b : a; }
/** @sly2-annot addr=0x152C58
 * Purpose: Draws the button-prompt HUD: per prompt 4 button glyphs ('&2?&.'-style font strings) at timeline
 *   positions, highlights the expected one, animates a hint marker
 * Info: proposed name DrawRhythmMgr (T1); class CID121 slot19; TU rhythm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/minigames.md#7-dance-dancer-0x44--rhythm-manager-cid121-0x79-rhythm-tu-0x1520c8-0x1539a8-ccode,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void DrawRhythmMgr(P *p)
{
    RenderLoSelf(p);
    float t = TMusicCur(D_003DD040);
    if (p->state < 3) if (p->state > 0) if (p->c != 0) {
        FONT fontA;
        CRichText_ctor(&fontA, "&2?&.", D_002BDB58);
        FONT fontB;
        CRichText_ctor(&fontB, "&2m&.", D_002BDB5C);
        TB tb;
        union { RGBA rgba; SZ s; } u;
        tb.Init(0.0f, 0.0f, 640.0f, 448.0f * 1.1f, 0xFF808080, &u.rgba);
        {
        u.s = D_002A5018;
        FONT fontBig;
        CRichText_ctor(&fontBig, u.s.a, D_002BDB50);
        float dxBig = CRichText_Dx(&fontBig);
        float dyBig = fontBig.dy;
        FONT fontSmall;
        CRichText_ctor(&fontSmall, u.s.a, D_002BDB54);
        float dxSmall = CRichText_Dx(&fontSmall);
        float dySmall = fontSmall.dy;
        float dxA = CRichText_Dx(&fontA);
        float dyA = fontA.dy;
        float dxB = CRichText_Dx(&fontB);
        float dyB = fontB.dy;
        int i;
        for (i = 0; i < p->c; i++) {
            ENT *pe = &p->ae[i];
            if (p->state == 1 || pe->fDone || pe->t + p->dt < t) {
                unsigned int j;
                for (j = 0; j < 4; j++) {
                    KEY *pk = &D_002BDB70[j];
                    float x = ((float *)&pe->pos)[0] + pk->dx * dxSmall;
                    float y = ((float *)&pe->pos)[1] + pk->dy * dySmall;
                    unsigned char ch = 'o';
                    register float scale asm("$f12") = D_002BDB54; /* pin: no separate f12 arg move in sched1 */
                    if (pk->id == pe->k) {
                        x -= (dxBig - dxSmall) * 0.5f;
                        y -= (dyBig - dySmall) * 0.5f;
                        ch = pk->ch;
                        scale = D_002BDB50;
                    }
                    u.s.a[2] = ch;
                    u.s.a[0] = '&';
                    u.s.a[1] = '2';
                    u.s.a[3] = '&';
                    u.s.a[4] = '.';
                    u.s.a[5] = 0;
                    CRichText_ctor(&fontBig, u.s.a, scale);
                    tb.x = x;
                    tb.y = y;
                    func_002163B0(&fontBig, &tb, 0xC);
                }
            } else {
                tb.x = ((float *)&pe->pos)[0] - dxA * 0.5f;
                tb.y = ((float *)&pe->pos)[1] - dyA;
                func_002163B0(&fontA, &tb, 0xC);
            }
        }
        if (p->iHint < p->c) {
            ENT *pe = &p->ae[p->iHint];
            switch (p->stateHint) {
            case 0:
                if (pe->t - t < D_002BDBB0) {
                    p->vel = D_002BDBC0;
                    p->stateHint = 1;
                }
                break;
            case 1:
                if (pe->t < t) {
                    p->stateHint = 0;
                    p->iHint = p->iHint + 1;
                }
                break;
            }
            if (p->stateHint == 1) {
                EvaluateBezierPos(GMax(pe->t - t, D_002DDED0.dt), D_002DDED0.dt, 1.0f, p->pos, p->vel, pe->pos, D_002BDBD0, &p->pos, &p->vel, 0);
            }
        }
        tb.x = ((float *)&p->pos)[0] - dxB * 0.5f;
        tb.y = ((float *)&p->pos)[1] - dySmall * 3.0f - dyB - 7.0f;
        func_002163B0(&fontB, &tb, 0xC);
        }
    }
}
