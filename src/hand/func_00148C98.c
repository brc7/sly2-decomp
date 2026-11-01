struct K { int a; int b; };
extern const struct K D_002A4E80;
extern char D_003DD490[];
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct PG { char pad[0x18C4]; int x18C4; int x18C8; };
extern struct PG *D_002E1E40;
struct E { int pad[2]; int x8; void *pxC; char pad2[0x30]; };
struct SO { struct E ae[2]; char pad[0x9C - 0x80]; int x9C; char pad2[0xAC - 0xA0]; struct SO *pnext; };
struct LN { struct SO *pso; struct LN *pnext; };
struct X { char pad[0x60]; int x60; struct LN *x64; };
struct P {
    char pad[0xEC]; struct X *xEC; char pad1[0x1E0 - 0xF0]; int x1E0; float x1E4; float x1E8;
    char pad2[0x2A8 - 0x1EC]; int x2A8; char pad3[0x2F4 - 0x2AC]; int x2F4; int x2F8;
};
void func_001F36E0(struct P *);
void func_001490A0(struct P *, int);
int func_00149100(struct P *, int);
void func_00149AB8(void *, void *, struct K, int);

static inline int IOther(struct X *px, struct SO *pso)
{
    return px->x60 == pso->ae[0].x8 ? 0 : 1;
}

/** @sly2-annot addr=0x148C98
 * Purpose: vt16: UpdateSo then press/repush state machine against pushing objects (current player etc.)
 * Info: proposed name UpdateButton (T1); class BUTTON slot16; TU button
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00148C98(struct P *p)
{
    func_001F36E0(p);
    switch (p->x1E0) {
    case 2: {
        struct PG *pg = D_002E1E40;
        if (pg && pg->x18C8 == 3 && pg->x18C4 == p->x2F4)
            p->x1E4 = D_002DDED0.t;
        if (0.0f <= p->x1E8 && p->x1E8 < D_002DDED0.t - p->x1E4)
            func_001490A0(p, 4);
        break;
    }
    case 3:
        break;
    case 4: {
        int k = -1;
        switch (p->x2A8) {
        case 0:
        case 2:
        case 3:
            k = 1;
            break;
        case 1:
            k = 0;
            break;
        }
        if (k != -1)
            func_001490A0(p, k);
        break;
    }
    case 0:
    case 1: {
        int k = p->x1E0;
        int oid;
        switch (p->x2A8) {
        case 1:
        case 2:
            break;
        case 3: {
            struct PG *pg = D_002E1E40;
            k = 0;
            if (pg && pg->x18C8 == 3)
                k = pg->x18C4 == p->x2F4;
            break;
        }
        found:
            p->x2F8 = oid;
            func_00149AB8(D_003DD490, p, D_002A4E80, 0);
            k = p->x1E0;
            break;
        case 0: {
            struct LN *pln;
            k = 0;
            for (pln = p->xEC->x64; pln; pln = pln->pnext) {
                struct SO *pso;
                for (pso = pln->pso; pso; pso = pso->pnext) {
                    if (pso->x9C != 10) {
                        int i = IOther(p->xEC, pso);
                        int j = 1 - i;
                        struct E *pe = &pso->ae[i];
                        oid = pso->ae[j].x8;
                        if (pe->pxC == p && func_00149100(p, oid))
                            goto found;
                    }
                }
            }
            break;
        }
        }
    done:
        func_001490A0(p, k);
        break;
    }
    }
}
