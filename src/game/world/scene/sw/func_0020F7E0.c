struct F8 { char pad[0x14]; float v; };
struct O { char pad[0xF8]; struct F8 *pf8; };
struct E { struct O *po; float v; int f; };
struct S { char pad[0x3BA0]; struct E ae[4]; char pad2[0x476C - 0x3BD0]; float t476C; int n4770; };
extern struct { float t; float dt; } D_002DDED0;
extern char D_002C3E88[];
extern float D_002C3E80;
int FWidgetVisible(struct O *po);
float func_0011D1C0(float a, float b, float c, void *p, float *pv, float dt);
void *PsndStartSound(int n);
void SetSndSourceLo(void *p, void *source);

/** @sly2-annot addr=0x20F7E0
 * Purpose: Per-frame: smooth the 4 HUD elements; while a countdown (SW+0x476C - t) runs play tick/tock sfx
 *   0x30/0x31 each second
 * Info: proposed name UpdateSwTimerHud (T2); TU sw
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/overview.md#5-per-frame-update-and-render,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F7E0(struct S *p)
{
    float z;
    float t = 0.0f;
    int i;
    z = t;
    for (i = 0; i < 4; i++) {
        struct E *e = &p->ae[i];
        struct O *o = e->po;
        if (!o || !FWidgetVisible(o)) {
            e->f = 1;
            continue;
        }
        if (e->f) {
            o->pf8->v = t;
            e->v = 0.0f;
            e->f = 0;
        } else {
            o->pf8->v = func_0011D1C0(o->pf8->v, e->v, t, D_002C3E88, &e->v, D_002DDED0.dt);
        }
        t += D_002C3E80;
        if (i == 3) {
            float d = -D_002DDED0.t + p->t476C;
            int n;
            d = (0.0f > d) ? 0.0f : d;
            n = (int)d;
            if (n != p->n4770) {
                SetSndSourceLo(PsndStartSound((n & 1) ? 0x30 : 0x31), 0);
                p->n4770 = n;
            }
        }
    }
}
