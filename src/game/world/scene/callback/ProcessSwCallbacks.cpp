struct CRef;
extern "C" void func_00129EC8(CRef *);
struct CRef {
    int m_tagk; int tag;
    ~CRef() { func_00129EC8(this); }
};
struct NODE;
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void GcPair_Unlink(void *pv);
}
struct SPM {
    NODE *p;
    ~SPM();
};
struct NODE {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { GcPair_Unlink(pv); func_0011CD98(D_004520A8, pv); }
};
struct O { int cref; };
extern "C" void func_00257078(O *, int);
struct SP {
    O *p;
    SP(const SP &r);
    ~SP() { O *q = p; if (q && --q->cref == 0) func_00257078(q, 3); }
};
struct OSP { O *p; };
extern "C" {
int PvNewFrame(void);
SP func_00257058(int);
void EvalSpliceEventCallback(void *pv, NODE *pn, O *po, int n);
}

struct CObj { virtual void Dummy(); };
struct CB {
    CObj *pobj;
    void (CObj::*pmf)(int);
    int arg;
    CB *pnext;
};
struct EV {
    float t;
    OSP *posp;
    NODE *pnode;
    int nC;
};
struct LIST { void *pvFirst; void *pvLast; int ib; };
struct CLOCK { float t; };
extern CLOCK D_002DDED0;

struct MGR {
    char pad[0x39F0];
    LIST levs;          /* 39F0 */
    char poolEv[0x3C90 - 0x39FC];
    char poolCb[0x3CA8 - 0x3C90];
    LIST lcbs;          /* 3CA8 */
};

extern "C" {
void ClearSl(LIST *pl);
int FIsSlEmpty(LIST *pl);
void RemoveSlEntry(LIST *pl, void *pv);
void ProcessSwCallbacks(MGR *p);
}

/** @sly2-annot addr=0x149C80
 * Purpose: Drain the deferred-callback list (calling each member function, repeating while new ones are
 *   posted), then run all Splice scheduled callbacks whose time has come (EvalSpliceEventCallback) and free
 *   them
 * Info: proposed name ProcessSwCallbacks (T1); TU callback
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/overview.md#5-per-frame-update-and-render,
 *   docs/engine/world-sw.md#6-deferred-callbacks-and-the-splice-schedule
 * @end-sly2-annot */
void ProcessSwCallbacks(MGR *p)
{
    while (!FIsSlEmpty(&p->lcbs)) {
        CB *pcb = (CB *)p->lcbs.pvFirst;
        ClearSl(&p->lcbs);
        while (pcb) {
            void (CObj::*pmf)(int) = pcb->pmf;
            (pcb->pobj->*pmf)(pcb->arg);
            CB *pcbNext = pcb->pnext;
            func_0011CD98(p->poolCb, pcb);
            pcb = pcbNext;
        }
    }

    EV *pev;
    while ((pev = (EV *)p->levs.pvFirst) != 0) {
        if (D_002DDED0.t < pev->t)
            break;
        pev->nC = -1;
        OSP *posp = pev->posp;
        if (posp) {
            EvalSpliceEventCallback(posp, pev->pnode, posp->p, 0);
        } else {
            int x = PvNewFrame();
            SP sp = func_00257058(x);
            EvalSpliceEventCallback(p, pev->pnode, sp.p, 0);
        }
        RemoveSlEntry(&p->levs, pev);
        NODE *pn = pev->pnode;
        if (--pn->cref == 0)
            delete pn;
        func_0011CD98(p->poolEv, pev);
    }
}
