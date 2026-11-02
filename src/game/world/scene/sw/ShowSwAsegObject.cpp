struct X { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
  virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); };
extern "C" X *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id);
extern "C" int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
extern "C" void FadeSoIn(X *p, float f);
extern "C" void FadeSoOut(X *p, float f);
/** @sly2-annot addr=0x210338
 * Purpose: ASEG event 7/8: find object; fade in/out over dt if SO-derived and dt!=0, else vt8 Add / vt9
 *   Remove
 * Info: proposed name ShowSwAsegObject (T1); TU sw
 * Notes: names_v2 v1 proposed InsertButtonMat__FP6BUTTONP3XFMP7MATRIX3 (0.012)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" X *ShowSwAsegObject(void *pv, int fOn, int id, float g)
{
    X *p = PloFindSwObject__FP2SWi3OIDP2LO(pv, 4, id);
    if (p) {
        if (g != 0.0f && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x30)) {
            if (fOn)
                FadeSoIn(p, g);
            else
                FadeSoOut(p, g);
        } else {
            if (fOn)
                p->v8();
            else
                p->v9();
        }
    }
    return p;
}
