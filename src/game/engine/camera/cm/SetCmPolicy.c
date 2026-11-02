/* Insert an entry into a priority-sorted array of 8 (Claude agent). memmove goes through an inline helper: its
   parameters get their own pseudos, giving the target's (p + i*16) base copied into a1 then +0x320 / +0x310. */
struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; int a; int b; };
struct P { char pad[0x30C]; int c; struct E ae[8]; char pad2[0x390 - 0x390]; float t; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void *memmove(void *dst, void *src, int cb);
static inline void Move(void *pvDst, void *pvSrc, int cb) { memmove(pvDst, pvSrc, cb); }
/** @sly2-annot addr=0x14E328
 * Purpose: Insert {cpp,pcplcy,psoFocus,pv} into priority-sorted acpr[8] (+0x30C count); call pcplcy vt4
 *   Set(pv); if new top: prev vt3 Deactivate, tModeStart=clock.t, vt2 Activate
 * Info: proposed name SetCmPolicy (T1); TU cm
 * Notes: runtime: CM policy stack acpr[8] at +0x310 sorted by priority confirmed
 * Confidence: high [T1 0.95] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmPolicy(struct P *p, int prio, struct O *po, int a, int b)
{
    int i;
    if (p->c < sizeof(p->ae) / sizeof(p->ae[0])) {
        for (i = 0; i < p->c; i++) {
            if (p->ae[i].prio < prio)
                break;
        }
        Move(&p->ae[i + 1], &p->ae[i], (p->c - i) * sizeof(struct E));
        p->ae[i].prio = prio;
        p->ae[i].po = po;
        p->ae[i].a = a;
        p->ae[i].b = b;
        p->c++;
        {
            struct VTE *e = &po->vt[4];
            ((void (*)(void *, int))e->pfn)((char *)po + e->d, b);
        }
        if (i == 0) {
            if (p->c >= 2) {
                struct O *o = p->ae[1].po;
                struct VTE *e = &o->vt[3];
                ((void (*)(void *, int, int))e->pfn)((char *)o + e->d, p->ae[1].a, p->ae[1].b);
            }
            p->t = D_002DDED0.t;
            {
                struct VTE *e = &po->vt[2];
                ((void (*)(void *, int, int))e->pfn)((char *)po + e->d, a, b);
            }
        }
    }
}
