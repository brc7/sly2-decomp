struct XB { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
  virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10();
  virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
  virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
  virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
};
struct X : XB { char pad10[0x10]; X *next; };
struct B { X *first; int a, b; };
struct S { char pad[0x3038]; B ab[199]; };
extern "C" void func_001C4A60(S *p, int k);
/** @sly2-annot addr=0x2102A0
 * Purpose: 0x1C4A60(p,0) then vt24 on every LO in the 199 class buckets (+0x3038)
 * Info: proposed name RefreshSwObjects (T2); TU sw
 * Notes: names_v2 v1 proposed InsertButtonPos__FP6BUTTONP3PNTP6VECTOR (0.008) | runtime: per-class list
 *   stride at SW+0x3038 is 12 bytes
 * Confidence: low (draft) [T2 0.25] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7_1F8DD8+runtime-corrected
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
extern "C" void func_002102A0(S *p)
{
    int i;
    func_001C4A60(p, 0);
    for (i = 0; i < 199; i++) {
        X *x = ((struct B *)((char *)p + 0x3038))[i].first;
        for (; x; x = x->next)
            x->v24();
    }
}
