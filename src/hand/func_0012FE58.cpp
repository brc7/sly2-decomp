struct KEYID { int id : 20; int sub : 10; int pad : 2; };
struct O {
    char pad[0xC];
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual int Get(int sub);
};
extern char D_003DD490[];
extern "C" {
O *func_0015E938(void *pg, int k, int id, void *p);
void func_0012FE58(void *a, void *b, KEYID *pk, O **ppo, int *pn);
}
/** @sly2-annot addr=0x12FE58
 * Purpose: Resolves a channel key: PloFindSwObject(oid) under the root (or world), then the object's
 *   sub-target via its virtual Get
 * Info: proposed name ResolveAsegKeyObject (T2); TU aseg
 * Notes: names_v2 B proposed PostAsegLoad__FP4ASEG (0.856)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void func_0012FE58(void *a, void *b, KEYID *pk, O **ppo, int *pn)
{
    if (b)
        *ppo = func_0015E938(D_003DD490, 1, pk->id, b);
    else
        *ppo = func_0015E938(D_003DD490, 4, pk->id, a);
    if (!*ppo)
        *pn = 0;
    else
        *pn = (*ppo)->Get(pk->sub);
}
