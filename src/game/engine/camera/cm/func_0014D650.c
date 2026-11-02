struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct E { struct O *po; int pad[3]; };
struct P { char pad[0x30C]; int c; int pad2; struct E ae[1]; };
/** @sly2-annot addr=0x14D650
 * Purpose: Calls vt7 (unknown 'reset/close' slot) on every policy in acpr[], then rUnk620=0, rUnk624=10.0
 * Info: proposed name ResetCmPolicies (T2); TU cm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70+verdicts_A1
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014D650(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct O *o = p->ae[i].po;
        struct VTE *e = &o->vt[7];
        ((void (*)(void *))e->pfn)((char *)o + e->d);
    }
    *(int *)((char *)p + 0x620) = 0;
    *(float *)((char *)p + 0x624) = 10.0f;
}
