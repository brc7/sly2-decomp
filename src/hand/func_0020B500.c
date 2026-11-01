struct G;
extern struct G D_003DD490;
typedef struct ALLOCREQ { int cb; int nKind; } ALLOCREQ;
struct S { char pad[0x50]; int c; void **apv; };
void func_001A5C48(struct S *p);
int func_0015E9C8(struct G *pg, int a, int b, int c, int cMax, void **apv);
void *func_001AE1D0(ALLOCREQ *preq);
/** @sly2-annot addr=0x20B500
 * Purpose: cid 0x7D vt15: list all STEPGUARD objects (cid 0x38)
 * Info: proposed name PostCid125GuardMgrLoad (T2); class CID125 slot15; TU stepguard
 * Notes: Cid7D->Cid125 (CID 0x7D=125, hex-literal) | verifier PLAUSIBLE: Placeholder class (CID 0x7D vtable
 *   slot 15); 'GuardMgr' is a role guess. PostLoLoad, count STEPGUARDs (cid 0x38) via
 *   CploFindSwObjectsByClass, alloc array, fill it. | T2 forced: verifier: placeholder name; verifier: coined
 *   class word without descriptor
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020B500(struct S *p)
{
    ALLOCREQ req;
    func_001A5C48(p);
    p->c = func_0015E9C8(&D_003DD490, 0x605, 0x38, 0, 0, 0);
    req.cb = p->c * 4;
    req.nKind = 0x10;
    p->apv = func_001AE1D0(&req);
    func_0015E9C8(&D_003DD490, 0x405, 0x38, 0, p->c, p->apv);
}
