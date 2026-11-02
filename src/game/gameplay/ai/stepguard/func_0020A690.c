typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; };
struct S { char pad[0xC]; struct O *po; int k; };
struct G { char pad[0x60]; float x; float y; };
extern struct G D_002DDF40;
float atan2f(float y, float x);
#define VCALL31(o) ((float *(*)(void *))(o)->vt[31].pfn)((char *)(o) + (o)->vt[31].d)
/** @sly2-annot addr=0x20A690
 * Purpose: Heading reference: camera (kind 0) or target facing (kind 1)
 * Info: proposed name RadTargetRecordFacing (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
float func_0020A690(struct S *p)
{
    float r = 0.0f;
    switch (p->k) {
    case 0:
        r = atan2f(D_002DDF40.y, D_002DDF40.x);
        break;
    case 1: {
        float *a = VCALL31(p->po);
        float *b = VCALL31(p->po);
        r = atan2f(a[1], b[0]);
        break;
    }
    case -1:
        break;
    }
    return r;
}
