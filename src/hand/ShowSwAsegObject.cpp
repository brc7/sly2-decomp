struct X { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
  virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); };
extern "C" X *PloFindSwObject__FP2SWi3OIDP2LO(void *pv, int k, int id);
extern "C" int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
extern "C" void FadeSoIn(X *p, float f);
extern "C" void FadeSoOut(X *p, float f);
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
