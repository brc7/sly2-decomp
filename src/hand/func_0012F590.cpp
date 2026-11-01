extern "C" {
typedef struct BIS BIS;
typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct L0 { int n; int a, b, c; float x, y, z; } L0;
typedef struct L20 { int a, b; float g; } L20;
typedef struct ENT { float g0; int kind; union { int n; float g; void *pv; int *pn; } u8; union { int n; float g; } uC; } ENT;
float func_00117E38(BIS *pbis);
int func_00117D50(BIS *pbis);
int func_00117DB8(BIS *pbis);
int func_00117D00(BIS *pbis);
int func_00117BD0(BIS *pbis);
void *func_001AE1D0(AREQ *preq);
void *func_001AE2A0(AREQ *preq);
void *func_001AE238(AREQ *preq, void *pv);
void func_00117FB0(BIS *pbis, void *pv);
void func_0012F590(void *px, BIS *pbis, int fTime, int *pc, struct ENT **pa);
}
static __inline__ void *PvAlloc(int cb) { AREQ req; req.cb = cb; req.cbAlign = 0x10; return func_001AE2A0(&req); }
/** @sly2-annot addr=0x12F590
 * Purpose: Reads the ASEG event list (typed U8/S8/S16/S32/F32 payloads), allocates arrays
 * Info: proposed name LoadAsegEventsFromBrx (T1); TU aseg
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_B+names_v2-signature(verified)+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void func_0012F590(void *px, BIS *pbis, int fTime, int *pc, ENT **pa) { int i; int c; ENT *aent;
 c = func_00117BD0(pbis);
 *pc = c;
 aent = (ENT *)PvAlloc(c * 16);
 *pa = aent;
 for (i = 0; i < c; i++) {
   ENT *pe = &aent[i];
   pe->kind = func_00117D00(pbis);
   if (fTime) pe->g0 = func_00117D50(pbis) * (1.0f / 60.0f);
   switch (pe->kind) {
   case 0: { AREQ req; L0 l; l.n = func_00117D50(pbis); l.a = func_00117DB8(pbis); l.b = func_00117DB8(pbis); l.c = func_00117DB8(pbis);
       l.z = func_00117E38(pbis); l.x = func_00117E38(pbis); l.y = func_00117E38(pbis);
       req.cb = 0x1C; req.cbAlign = 0x10; pe->u8.pv = func_001AE238(&req, &l); break; }
   case 3: pe->uC.g = func_00117D50(pbis) * (1.0f / 60.0f); pe->u8.n = func_00117D50(pbis); break;
   case 5: pe->u8.g = func_00117D50(pbis) * (1.0f / 60.0f); pe->uC.g = func_00117E38(pbis); break;
   case 7: case 8: pe->u8.n = func_00117DB8(pbis); pe->uC.g = func_00117E38(pbis); break;
   case 13: case 14: case 15: case 16: pe->u8.n = func_00117DB8(pbis); pe->uC.n = func_00117DB8(pbis); break;
   case 17: { pe->u8.pv = PvAlloc(0x10);
       pe->u8.pn[0] = func_00117DB8(pbis); pe->u8.pn[1] = func_00117DB8(pbis); pe->u8.pn[2] = func_00117DB8(pbis); ((float *)pe->u8.pn)[3] = func_00117E38(pbis); break; }
   case 20: { L20 t; AREQ req; AREQ *preq; t.a = func_00117DB8(pbis); t.b = func_00117DB8(pbis); t.g = func_00117E38(pbis);
       preq = &req; req.cb = 0xC; preq->cbAlign = 0x10; pe->u8.pv = func_001AE238(preq, &t); break; }
   case 1: case 2: case 21: pe->u8.n = func_00117D50(pbis); break;
   case 4: case 11: case 12: case 22: pe->u8.n = func_00117DB8(pbis); break;
   case 24: { AREQ req; char *p; req.cb = 0x18; req.cbAlign = 0x10; p = (char *)func_001AE1D0(&req);
       func_00117FB0(pbis, p); func_00117FB0(pbis, p + 8); func_00117FB0(pbis, p + 0x10); pe->u8.pv = p; break; }
   }
 } }
