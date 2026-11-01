typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct N { qword *pplane; unsigned short ach[2]; };
struct B { int c; struct N *an; int x8, xc; int x10; int x14; void **al; };
struct G { char pad[0x45C8]; void *x45C8; struct B *pbsp; };
extern struct G D_003DD490;
void func_002564B8(void *p);
static __inline__ int ISide(qword plane, qword pos)
{
    U d, one;
    qword td, tone;
    float dist;
    int i;
    func_002564B8(&d);
    func_002564B8(&one);
    i = 1;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddaz.x $ACC, %1, %0z\n\tvmsubw.x %0, %1, %2w" : "=j"(td), "=&j"(tone) : "j"(plane), "j"(pos));
    __asm__("mtc1 %1, %0" : "=f"(dist) : "r"(td));
    one.q = tone;
    d.q = td;
    if (0.0f < dist) {
    } else
        i = 0;
    return i;
}
void *PpathzoneFromPos(qword pos, int f)
{
    struct B *pb = D_003DD490.pbsp;
    struct N *pn, *pnEnd, *an;
    void **al;
    unsigned int c;
    if (!pb)
        return 0;
    pn = pb->an;
    pnEnd = pn + pb->c;
    al = pb->al;
    an = pn;
    while (pn != pnEnd) {
        c = pn->ach[ISide(*pn->pplane, pos)];
        if (c & 0x8000)
            goto leaf;
        pn = &an[c];
    }
    if (!f && pb->x10)
        return 0;
    return D_003DD490.x45C8;
leaf:
    return al[c & 0x7FFF];
}
