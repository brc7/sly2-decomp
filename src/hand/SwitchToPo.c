struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x18 - 0x10]; int oid; };
struct G { char pad[0x3EFC]; struct LO *plo3EFC; struct LO *plo3F00; };
struct CRef { int m_tagk; int m_tag; int pad[2]; };
struct LO *PpoCur(struct G *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct LO *plo, int n);
struct CRef *func_001D8B20(struct CRef *pret, struct CRef *pref, const char *s, int n);
void func_00129EC8(struct CRef *p);
void UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND(struct G *p, int a, int b, int c);
void func_001C48C0(struct G *p);
#define VCALL(o, n, a) ((void (*)(void *, int))(o)->vt[n].pf)((char *)(o) + (o)->vt[n].d, a)
/** @sly2-annot addr=0x1C4940
 * Purpose: Make PO current: OnActive(0) on old effective PO, set SW+0x3EFC, OnActive(1) on new; if STEPPLAYER
 *   set Splice var "oidPoCurrent"; update grab candidates and followers
 * Info: proposed name SwitchToPo (T1); TU po
 * Notes: names_v2 C proposed SetRopRops__FP3ROP4ROPS (0.638)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void SwitchToPo(struct G *p, struct LO *plo)
{
    struct LO *cur = PpoCur(p);
    struct LO *nw = p->plo3F00;
    if (!nw) nw = plo;
    if (cur != nw && cur) VCALL(cur, 78, 0);
    p->plo3EFC = plo;
    if (cur != nw && nw) VCALL(nw, 78, 1);
    if (plo && FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x34)) {
        struct CRef ret, ref;
        ref.m_tagk = 0x15;
        ref.m_tag = plo->oid;
        func_001D8B20(&ret, &ref, "oidPoCurrent", 0);
        func_00129EC8(&ret);
        func_00129EC8(&ref);
    }
    UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND(p, 0, 0, 0);
    func_001C48C0(p);
}
