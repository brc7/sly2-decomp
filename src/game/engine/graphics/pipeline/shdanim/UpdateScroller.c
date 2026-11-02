struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct SAI { int a; };
struct SCROLLER { char pad[0x24]; short du; short dv; char pad2[0x34 - 0x28]; struct SAI sai; char pad3[0x50 - 0x38];
    float duSpeed; float dvSpeed; float duMax; float dvMax; float sdu; float sdv; };
int FIsSlEmpty(struct SAI *psai);
float fmodf(float x, float y);
void SetSaiDuDv(struct SCROLLER *p, float du, float dv);
/** @sly2-annot addr=0x1EE4E0
 * Purpose: SCROLLER vt8 Update
 * Info: proposed name UpdateScroller (T1); class SCROLLER slot8; TU shdanim
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void UpdateScroller(struct SCROLLER *p)
{
    float du, dv;
    if (FIsSlEmpty(&p->sai)) return;
    du = fmodf(p->du * (1.0f / 4096.0f) + p->duSpeed * p->sdu * D_002DDED0.dt, p->duMax);
    dv = fmodf(p->dv * (1.0f / 4096.0f) + p->dvSpeed * p->sdv * D_002DDED0.dt, p->dvMax);
    SetSaiDuDv(p, du, dv);
}
