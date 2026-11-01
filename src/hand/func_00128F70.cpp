struct X;
struct Y;
struct SPX { X *p; ~SPX(); SPX &operator=(X *px); };
struct SPY { Y *p; ~SPY(); SPY &operator=(Y *py); };
struct O {
    int cref;
    int x4;
    SPX spx;
    int xC;
    int x10;
    SPY spy14;
    SPY spy18;
    ~O();
};
struct SPO {
    O *p;
    void Set(O *po);
    SPO() {}
    SPO(O *po) { Set(po); }
    SPO(const SPO &);
    ~SPO() { O *po = p; if (po && --po->cref == 0) delete po; }
    O *operator->() { return p; }
};
extern "C" {
O *func_00256F48();
SPO func_00128F70(O *poSrc, X *px);
}
/** @sly2-annot addr=0x128F70
 * Purpose: New proc (0x256F48) copying frame pointer and param/body lists
 * Info: proposed name CProc_CloneTo (T1); class CProc; TU proc
 * Notes: agent qualifier: (returns clone)
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#proc-0x128f70-0x1291a8,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
SPO func_00128F70(O *poSrc, X *px)
{
    SPO sp;
    sp.Set(func_00256F48());
    if (px)
        sp->spx = px;
    else
        sp->spx = poSrc->spx.p;
    sp->spy14 = poSrc->spy14.p;
    sp->xC = poSrc->xC;
    sp->x10 = poSrc->x10;
    sp->spy18 = poSrc->spy18.p;
    return SPO(sp.p);
}
