struct K { short a; short b; int c; };
struct LM { float gMin; float gMax; };
struct BOX { float xMin, yMin, zMin, pad; float xMax, yMax, zMax; };
struct EMITO { void *palo; };
struct EMITB { int n0; int pad4; struct EMITO *pemito; int padC; int kind; char pad14[0x20 - 0x14]; int n20; char pad24[0x30 - 0x24]; struct BOX *pbox; };
struct VTE { short delta; short pad; void (*pfn)(void *, void *); };
struct EMITTER {
    char pad0[0xC]; struct VTE *pvt; char pad10[0x40 - 0x10]; int grf40; char pad44[0xE0 - 0x44];
    int nE0; struct EMITB *pemitb; char padE8[0xF0 - 0xE8];
    int kind; int nF4; struct LM lmF8; int n100; float g104; struct LM lm108; float g110; int n114; int oid118;
    char pad11C[0x130 - 0x11C]; float g130; float g134; float g138; float g13C; float g140;
};
struct ALLOCREQ { int cb; int kind; };
extern char D_003DD490[];
extern const struct K D_002A5140;
extern float D_002A5148;
void func_002586D8(struct EMITTER *p);
void func_00149A50(void *pg, struct EMITTER *p, struct K k, int n);
float func_0011D448(float gMin, float gMax);
void func_0015A150(struct EMITTER *p);
void func_0015A138(struct EMITTER *p, float g);
struct EMITTER *func_0015E938(void *pg, int kind, int oid, int n);
struct EMITTER *func_0013E3C8(int cid, void *p, int oid, int n);
void *func_001AE238(struct ALLOCREQ *preq, void *pv);
void func_001A6CA8(struct EMITTER *p, int n);
void *func_001A6318(struct EMITTER *p);
void func_0011A220(void *pv, struct EMITTER *p);

static __inline__ void *PvAllocCopy(int cb, void *pv)
{
    struct ALLOCREQ req;
    req.cb = cb;
    req.kind = 0x10;
    return func_001AE238(&req, pv);
}

static __inline__ int FPtr(int n) { return !(n & 1); }

/** @sly2-annot addr=0x159500
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/particles-effects.md#1-emitters-emittercpp-0x159118-0x15cd68,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void PostEmitterLoad__FP7EMITTER(struct EMITTER *pemitter)
{
    func_002586D8(pemitter);
    if (pemitter->pemitb->pemito->palo)
        func_00149A50(D_003DD490, pemitter, D_002A5140, 0);
    if (pemitter->kind == -1) {
        if (pemitter->g104 != 0.0f || pemitter->n114)
            pemitter->kind = 3;
        else if (pemitter->g110 != 0.0f)
            pemitter->kind = 1;
        else
            pemitter->kind = 0;
    }
    switch (pemitter->kind) {
    case 2:
        pemitter->g138 = D_002A5148;
        pemitter->g130 = func_0011D448(pemitter->lmF8.gMin, pemitter->lmF8.gMax);
        if (pemitter->n114)
            func_0015A150(pemitter);
        else if (func_0011D448(0.0f, 1.0f) <= pemitter->g104)
            func_0015A138(pemitter, func_0011D448(pemitter->lm108.gMin, pemitter->lm108.gMax));
        break;
    case 3:
        pemitter->g138 = D_002A5148;
        pemitter->g130 = func_0011D448(pemitter->lmF8.gMin, pemitter->lmF8.gMax) * 60.0f;
        if (pemitter->n114)
            func_0015A150(pemitter);
        else if (func_0011D448(0.0f, 1.0f) <= pemitter->g104)
            func_0015A138(pemitter, func_0011D448(pemitter->lm108.gMin, pemitter->lm108.gMax));
        break;
    case 0:
    case 1:
        pemitter->g134 = pemitter->g138 = 1.0f;
        pemitter->g130 = func_0011D448(pemitter->lmF8.gMin, pemitter->lmF8.gMax);
        break;
    }
    pemitter->g13C = 1.0f;
    if (pemitter->pemitb->kind == 1) {
        struct BOX *pbox = pemitter->pemitb->pbox;
        if (pemitter->pemitb->pemito->palo) {
            float dx = __builtin_fabsf(pbox->xMax - pbox->xMin);
            float dy = __builtin_fabsf(pbox->yMax - pbox->yMin);
            float dz = __builtin_fabsf(pbox->zMax - pbox->zMin);
            float g;
            if (dy < dx && dz < dx)
                g = dx;
            else if (dz < dy)
                g = dy;
            else
                g = dz;
            pemitter->g140 = g * 0.5f;
        }
        if (pemitter->n100) {
            if (pemitter->g110 != 0.0f)
                pemitter->g13C = (pbox->xMax - pbox->xMin) * (pbox->yMax - pbox->yMin) * (pbox->zMax - pbox->zMin) * 1e-6f;
            else
                pemitter->g13C = (pbox->yMax - pbox->yMin) * (pbox->zMax - pbox->zMin) * 1e-4f;
        }
    }
    if (pemitter->oid118 != -1) {
        struct EMITTER *pemitterNew = func_0015E938(D_003DD490, 5, pemitter->oid118, 0);
        if (!pemitterNew) {
            struct VTE *pvte;
            void *pthis;
            pemitterNew = func_0013E3C8(0x1D, 0, pemitter->oid118, -1);
            pemitterNew->pemitb = PvAllocCopy(0x100, pemitter->pemitb);
            pemitterNew->pemitb->n0 = 1;
            if (pemitter->pemitb->pemito->palo) {
                pemitterNew->pemitb->pemito = PvAllocCopy(0x14, pemitter->pemitb->pemito);
                pemitterNew->pemitb->n20 = -1;
                pemitterNew->pemitb->pemito->palo = 0;
            }
            pemitterNew->kind = pemitter->kind;
            pemitterNew->nF4 = pemitter->nF4;
            pemitterNew->lmF8 = pemitter->lmF8;
            pemitterNew->n100 = pemitter->n100;
            pemitterNew->g104 = pemitter->g104;
            pemitterNew->lm108 = pemitter->lm108;
            pemitterNew->g110 = pemitter->g110;
            pemitterNew->n114 = pemitter->n114;
            func_001A6CA8(pemitterNew, 1);
            pvte = &pemitterNew->pvt[30];
            pthis = (char *)pemitterNew + pvte->delta;
            pvte->pfn(pthis, func_001A6318(pemitter));
        }
        pemitter->nE0 = 0;
        func_0011A220((char *)pemitterNew + 0x11C, pemitter);
        if (FPtr(pemitter->grf40))
            *(int *)pemitter->grf40 &= ~2;
        else
            pemitter->grf40 &= ~2;
    }
}
