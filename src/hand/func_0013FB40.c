/* func_0013FB40 (Claude agent, laneC11 2026-10-23): matched from wip v3. One commented register pin (pbis in s5). The plo1->x50 = 1 store is written in both arms of `if (cid == 0x84)`: gcc cross-jumps them and leaves the dead `li v1,0x84` the target has. */
struct VT { short d; short pad; void *f; };
struct IF { struct VT *vt; int x4; int x8; };
struct LO { char pad[0xC]; struct VT *vt; char pad2[0x50 - 0x10]; int x50; };
struct G { char pad[0x3F0C]; int x3F0C; int x3F10; };
extern struct VT D_002CBE38[];
extern struct G D_003DD490;
int func_00117D50(void *pbis);
int func_00117D00(void *pbis);
int func_00117DB8(void *pbis);
void func_00117968(void *pbis, int cb, void *pv);
void func_001179B8(void *pbis, int cb);
int func_001C9940(int, int);
int func_001C99D8(int, int, int *);
int func_0015E938(struct G *, int, int, int, int);
int func_0015E958(struct G *, int, int, int, int);
int func_0013FAD0(int c, short *a);
int func_0015E988(int, int);
struct LO *func_0013E3C8(int cid, void *psw, int oid, int n);
void func_001C9B08(struct G *, void *psw, int cid, int oid, int n, void *pbis, struct IF *pif);

#define VCALL1(p, n, a) ((void (*)(void *, void *))(void*)(p)->vt[n].f)((char *)(p) + (p)->vt[n].d, a)
#define VCALL2(p, n, a, b) ((void (*)(void *, int, void *))(void*)(p)->vt[n].f)((char *)(p) + (p)->vt[n].d, a, b)

static inline int FList(int k) { return k > 0 || k < -2; }

/** @sly2-annot addr=0x13FB40
 * Purpose: Reads an object list: S16 cobj x {S8 k; [filter]; S16 cid; S32 oid; S16 isplice; S32 cb; payload
 *   via vt14 LoadFromBrx}, proxies (FIsCidDerivedFrom(cid,6)) via LoadProxyFromBrx, filtered objects skipped
 *   by cb
 * Info: proposed name LoadSwObjectsFromBrx (T1); TU brx
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.97] - evidence: semantic review; sources: r2_131F70+parse-verified
 * See: docs/engine/object-model.md#9-key-functions, docs/engine/overview.md#3-world--level-lifecycle,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file
 * @end-sly2-annot */
void func_0013FB40(void *psw, void *pbisArg, struct IF *pifParent)
{
    register void *pbis asm("$21") = pbisArg; /* register pin (Claude agent): target keeps pbis in s5 */
    struct IF obj;
    short as[0x80];
    int cid, oid, cb, n;
    int t;
    struct LO *plo1;
    struct LO *plo2;
    int c;
    int i;
    obj.vt = D_002CBE38;
    obj.x4 = -1;
    obj.x8 = -1;
    c = func_00117D50(pbis);
    for (i = 0; i < c; i++) {
        int k = func_00117D00(pbis);
        struct IF *pif;
        if (k > 0)
            goto read;
        if (k < -2) {
        read:
            func_00117968(pbis, k * 2, as);
        }
        pif = pifParent;
        cid = func_00117D50(pbis);
        oid = func_00117DB8(pbis);
        n = func_00117D50(pbis);
        cb = func_00117DB8(pbis);
        if (k < -2)
            goto filter;
        if (k >= 0) {
            if (k != 0)
                goto filter;
            goto create;
        }
        {
            struct G *pg = &D_003DD490;
            int r;
            t = func_001C9940(pg->x3F10, -1);
            r = func_001C99D8(pg->x3F0C, 1, &t);
            if (k == -2) {
                if (func_0015E938(pg, 0x405, oid, 0, r)) {
                    goto skip;
                }
                obj.x4 = 1;
            } else {
                if (func_0015E958(pg, 0x805, oid, 0, r)) {
                    goto skip;
                }
                obj.x4 = 0;
            }
            obj.x8 = t;
            pif = &obj;
            goto create;
        }
        {
        filter:
            if (func_0013FAD0(k, as) == 0) {
                if (cid == 0x84 || cid == 0x85) {
                    plo1 = func_0013E3C8(cid, psw, oid, -1);
                    if (cid == 0x84)
                        plo1->x50 = 1;
                    else
                        plo1->x50 = 1;
                    VCALL1(plo1, 14, pbis);
                    if (pifParent)
                        VCALL2(pifParent, 1, 1, &plo1);
                    continue;
                }
                goto skip;
            }
        }
    create:
        if (func_0015E988(cid, 6)) {
            func_001C9B08(&D_003DD490, psw, cid, oid, n, pbis, pif);
        } else {
            plo2 = func_0013E3C8(cid, psw, oid, n);
            VCALL1(plo2, 14, pbis);
            if (pif)
                VCALL2(pif, 1, 1, &plo2);
        }
        continue;
    skip:
        func_001179B8(pbis, cb);
    }
}
