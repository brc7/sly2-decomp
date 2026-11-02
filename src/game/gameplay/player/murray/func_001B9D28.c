typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; int pad1[6]; unsigned long f28; };
struct P { char pad[0x1AD0]; struct O *po; int n1AD4; };
int FPowerupAcquired(int n);
void func_0012F9B8(int n, struct O *po, int a, int b, float g0, float g1);
/** @sly2-annot addr=0x1B9D28
 * Purpose: If power-up 0x11 owned: position effect object (vt29) at given point and play its anim
 * Info: proposed name SpawnMurrayHitEffect (T2); TU murray
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B9D28(struct P *p, qword q)
{
    if (FPowerupAcquired(0x11)) {
        struct O *po = p->po;
        if (!((po->f28 >> 60) & 1)) {
            struct VTE *e = &po->vt[29];
            ((void (*)(void *, qword))e->pfn)((char *)po + e->d, q);
            func_0012F9B8(p->n1AD4, p->po, 0, 0, 0.0f, 1.0f);
        }
    }
}
