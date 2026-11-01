struct VT { char pad[0x198]; short d; short pd; void (*pf)(void *, void *); };
struct X { char pad[0xC]; struct VT *pvt; };
struct MSG { unsigned int grf; struct X *p4; float g8; char padc[0x24]; struct X *p30; char pad34[0xC]; };
struct X *func_0015E938(void *pg, int k, int oid, void *ctx);
/** @sly2-annot addr=0x210538
 * Purpose: ASEG event 17: find object and two optional others, build a message (grf bits 1/0x20/0x18) and
 *   send via vt
 * Info: proposed name SendSwAsegMessage (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210538(void *pg, int oid, int oid1, int oid2, float g, void *ctx)
{
    struct X *x = func_0015E938(pg, 4, oid, ctx);
    struct MSG msg;
    if (!x)
        return;
    msg.grf = 0;
    if (oid1 != -1) {
        struct X *y = func_0015E938(pg, 4, oid1, ctx);
        if (y) {
            msg.p4 = y;
            msg.grf |= 1;
        }
    }
    {
        unsigned int v = msg.grf;
        if ((v ^ 1) & 1) {
            msg.p4 = x;
            msg.grf = v | 1;
        }
    }
    if (oid2 != -1) {
        struct X *z = func_0015E938(pg, 4, oid2, ctx);
        if (z) {
            msg.p30 = z;
            msg.grf |= 0x20;
        }
    }
    if (g != 1.0f) {
        msg.g8 = g;
        msg.grf |= 0x18;
    }
    x->pvt->pf((char *)x + x->pvt->d, &msg);
}
