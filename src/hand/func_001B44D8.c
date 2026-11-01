struct E { char pad[0x10]; long x10; char pad2[0xB0 - 0x18]; };
struct O { char pad[0x20]; struct O *next; char pad1[0xE0 - 0x24]; int c; struct E *ae; };
struct REQ { int cb; int align; };
extern struct O *D_003E0714;
struct E **func_001AE318(struct REQ *r);
/** @sly2-annot addr=0x1B44D8
 * Purpose: Gather pointers to all visible entries across a global linked list D_003E0714 (count +0xE0, array
 *   +0xE4) into a stack-heap array
 * Info: proposed name CollectMsGlobs (T2); TU ms
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/rendering.md#ms-0x1b4438-0x1b4728
 * @end-sly2-annot */
void func_001B44D8(int *pc, struct E ***pap)
{
    struct O *o;
    struct E **a, **pp;
    struct REQ req;
    int n = 0, i;
    for (o = D_003E0714; o; o = o->next) n += o->c;
    req.cb = n * 4;
    req.align = 0x10;
    a = func_001AE318(&req);
    pp = a;
    for (o = D_003E0714; o; o = o->next) {
        for (i = 0; i < o->c; i++) {
            struct E *e = &o->ae[i];
            if (!(e->x10 & (1L << 48))) *pp++ = e;
        }
    }
    *pc = pp - a;
    *pap = a;
}
