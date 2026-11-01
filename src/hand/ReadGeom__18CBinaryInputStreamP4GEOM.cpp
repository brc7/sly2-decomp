typedef int qword __attribute__((mode(TI)));
struct BIS;
struct AREQ { int cb; int cbAlign; };
struct EDGE { unsigned short ipos0, ipos1, iface, ifaceOther; EDGE *pedgeNext; EDGE *pedgeOther; };
struct FACE { qword plane; EDGE *pedgeFirst; EDGE *pedgeOther; int pad; int ipos; };
struct GEOM { int cpos; qword *apos; int cface; FACE *aface; int cedge; EDGE *aedge; };
extern "C" {
int func_00117C20(BIS *pbis);
float func_00117E38(BIS *pbis);
int func_00117BD0(BIS *pbis);
void func_00117EB0(BIS *pbis, qword *pv);
void *func_001AE1D0(AREQ *preq);
void *func_001AE2A0(AREQ *preq);
void InitPlaneFromPositions(FACE *pface, qword a, qword b, qword c);
void ReadGeom__18CBinaryInputStreamP4GEOM(BIS *pbis, GEOM *pg);
}
static inline void *PvAlloc1D0(int cb) { AREQ req; req.cbAlign = 0x10; req.cb = cb; return func_001AE1D0(&req); }
static inline void *PvAlloc2A0(int cb) { AREQ req; req.cbAlign = 0x10; req.cb = cb; return func_001AE2A0(&req); }
void ReadGeom__18CBinaryInputStreamP4GEOM(BIS *pbis, GEOM *pg) { int i;
 pg->cpos = func_00117C20(pbis);
 pg->apos = (qword *)PvAlloc1D0(pg->cpos * 16);
 for (i = 0; i < pg->cpos; i++) func_00117EB0(pbis, &pg->apos[i]);
 pg->cface = func_00117C20(pbis);
 pg->aface = (FACE *)PvAlloc2A0(pg->cface * 32);
 pg->cedge = func_00117C20(pbis);
 pg->aedge = (EDGE *)PvAlloc2A0(pg->cedge * 16); EDGE *pedge = pg->aedge;
 for (int iface = 0; iface < pg->cface; iface++) {
   FACE *pf = &pg->aface[iface];
   int ai[3];
   ai[0] = func_00117C20(pbis); ai[1] = func_00117C20(pbis); ai[2] = func_00117C20(pbis);
   InitPlaneFromPositions(pf, pg->apos[ai[0]], pg->apos[ai[1]], pg->apos[ai[2]]);
   pf->ipos = ai[0];
   for (int j = 0; j < 4; j++) func_00117E38(pbis);
   int cedge = func_00117BD0(pbis);
   for (int k = 0; k < cedge; k++) {
     EDGE *pe = pedge++;
     pe->ipos0 = func_00117C20(pbis); pe->ipos1 = func_00117C20(pbis); pe->iface = iface;
     pe->ifaceOther = func_00117C20(pbis);
     if (k == 0) pf->pedgeFirst = pe; else pe[-1].pedgeNext = pe;
     FACE *pfO = &pg->aface[pe->ifaceOther];
     pe->pedgeOther = pfO->pedgeOther;
     pfO->pedgeOther = pe;
   }
 } }
