typedef int qword __attribute__((mode(TI)));
struct BIS;
struct AREQ { int cb; int cbAlign; };
struct EDGE { unsigned short ipos0, ipos1, iface, ifaceOther; EDGE *pedgeNext; EDGE *pedgeOther; };
struct FACE { qword plane; EDGE *pedgeFirst; EDGE *pedgeOther; int pad; int ipos; };
struct GEOM { int cpos; qword *apos; int cface; FACE *aface; int cedge; EDGE *aedge; };
extern "C" {
int U16Read__18CBinaryInputStream(BIS *pbis);
float F32Read__18CBinaryInputStream(BIS *pbis);
int U8Read__18CBinaryInputStream(BIS *pbis);
void ReadVector__18CBinaryInputStreamP6VECTOR(BIS *pbis, qword *pv);
void *func_001AE1D0(AREQ *preq);
void *func_001AE2A0(AREQ *preq);
void InitPlaneFromPositions(FACE *pface, qword a, qword b, qword c);
void ReadGeom__18CBinaryInputStreamP4GEOM(BIS *pbis, GEOM *pg);
}
static inline void *PvAlloc1D0(int cb) { AREQ req; req.cbAlign = 0x10; req.cb = cb; return func_001AE1D0(&req); }
static inline void *PvAlloc2A0(int cb) { AREQ req; req.cbAlign = 0x10; req.cb = cb; return func_001AE2A0(&req); }
/** @sly2-annot addr=0x117FD0
 * Purpose: Reads a GEOM: positions, faces (planes), edges with twin links
 * Info: proposed name CBinaryInputStream_ReadGeom (T1); class CBinaryInputStream; TU bis
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_A1+names_v2-signature(verified)+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/formats/level-objects.md#4-common-building-blocks-cparse,
 *   docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void ReadGeom__18CBinaryInputStreamP4GEOM(BIS *pbis, GEOM *pg) { int i;
 pg->cpos = U16Read__18CBinaryInputStream(pbis);
 pg->apos = (qword *)PvAlloc1D0(pg->cpos * 16);
 for (i = 0; i < pg->cpos; i++) ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pg->apos[i]);
 pg->cface = U16Read__18CBinaryInputStream(pbis);
 pg->aface = (FACE *)PvAlloc2A0(pg->cface * 32);
 pg->cedge = U16Read__18CBinaryInputStream(pbis);
 pg->aedge = (EDGE *)PvAlloc2A0(pg->cedge * 16); EDGE *pedge = pg->aedge;
 for (int iface = 0; iface < pg->cface; iface++) {
   FACE *pf = &pg->aface[iface];
   int ai[3];
   ai[0] = U16Read__18CBinaryInputStream(pbis); ai[1] = U16Read__18CBinaryInputStream(pbis); ai[2] = U16Read__18CBinaryInputStream(pbis);
   InitPlaneFromPositions(pf, pg->apos[ai[0]], pg->apos[ai[1]], pg->apos[ai[2]]);
   pf->ipos = ai[0];
   for (int j = 0; j < 4; j++) F32Read__18CBinaryInputStream(pbis);
   int cedge = U8Read__18CBinaryInputStream(pbis);
   for (int k = 0; k < cedge; k++) {
     EDGE *pe = pedge++;
     pe->ipos0 = U16Read__18CBinaryInputStream(pbis); pe->ipos1 = U16Read__18CBinaryInputStream(pbis); pe->iface = iface;
     pe->ifaceOther = U16Read__18CBinaryInputStream(pbis);
     if (k == 0) pf->pedgeFirst = pe; else pe[-1].pedgeNext = pe;
     FACE *pfO = &pg->aface[pe->ifaceOther];
     pe->pedgeOther = pfO->pedgeOther;
     pfO->pedgeOther = pe;
   }
 } }
