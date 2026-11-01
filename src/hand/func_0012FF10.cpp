struct KEYID { int id : 20; int sub : 10; unsigned int f30 : 1; unsigned int f31 : 1; };
struct R { char pad[0x94]; void *pv; };
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
    virtual R *Get(int sub);
};
struct E { KEYID k; int x; };
struct T { char pad[0x5C]; int c; E *ae; };
struct O2 : O { };
extern "C" E *func_0012FF10(T *t, O *po, void *pv, int fCheck);
/** @sly2-annot addr=0x12FF10
 * Purpose: Find the ASEG channel entry (+0x60[], count +0x5C, 8 B each) that animates this object (oid +0x18)
 *   and XF component kind; optional check of the entry flag bit30; skips bit31 entries
 * Info: proposed name PchnFindAseg (T1); TU aseg
 * Notes: v3: superseded PeFindAsegKeyForObject | top50_round (0.60): Sly 1 aseg.cpp PchnFindAseg(paseg,palo)
 *   loops achn and matches FMatchesLoName(palo,pchn->oid). Sly 2 entry = {oid:20, ixf:10, f30, f31}; it also
 *   requires po->vt51(ixf) (SO::PxfFromId) ->+0x94 (component kind) == kind. Ve
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r1_116918+top50_round
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8,
 *   docs/reference/functions/animation.md#dirblend-0x1a0300-0x1a0b18,
 *   docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
E *func_0012FF10(T *t, O *po, void *pv, int fCheck)
{
    int i;
    for (i = 0; i < t->c; i++) {
        E *pe = &t->ae[i];
        if (fCheck && !pe->k.f30)
            continue;
        if (pe->k.id != ((int *)po)[6])
            continue;
        if (pe->k.f31)
            continue;
        if (po->Get(pe->k.sub)->pv == pv)
            return pe;
    }
    return 0;
}
