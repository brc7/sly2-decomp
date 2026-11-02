typedef int qword __attribute__((mode(TI)));
struct Q { qword pos; char pad[0x50]; struct Q *next; };
struct E { int oid; int c; int c8; int d; struct Q *pqFirst; struct Q *pqLast; int ib; int x1c; float g20; int x24; float g28; float g2c; };
struct O { char pad[0x1C8]; int ce; struct E ae[4]; };
/** @sly2-annot addr=0x182810
 * Purpose: Nearest segment record to a world position
 * Info: proposed name PsegFindHackwall (T2); class HACKWALL; TU tu_17F090
 * Notes: Cid4C->Cid76 (CID 0x4C=76, hex-literal) | class identity from whole_game WE names/object strings:
 *   PsegFindCid76 -> PsegFindHackwall
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
struct Q *func_00182810(struct O *po, qword pos)
{
    int i;
    struct Q *pqBest = 0;
    float gBest = 3.4028235e38f;
    for (i = 0; i < po->ce; i++) {
        struct Q *pq;
        struct E *pe = &po->ae[i];
        for (pq = pe->pqFirst; pq; pq = pq->next) {
            qword b;
            register qword a __asm__("vf1");
            register qword one __asm__("vf2");
            float g;
            __asm__("qmtc2.ni %1, %0" : "=j"(b) : "r"(pos));
            a = pq->pos;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(a) : "j"(a), "j"(b));
            __asm__("vmul.xyz %0, %1, %1" : "=j"(a) : "j"(a));
            __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "+j"(a));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
            if (g < gBest) {
                gBest = g;
                pqBest = pq;
            }
        }
    }
    return pqBest;
}
