struct BIS;
struct AREQ { int cb; int cbAlign; };
struct OBJ { virtual void Load(BIS *pbis); };
struct ENT { unsigned n : 20; unsigned k : 10; unsigned f30 : 1; unsigned f31 : 1; OBJ *pobj; };
struct ENT2 { int a; int b; OBJ *pobj; };
struct X { char pad[0x5C]; int c; ENT *aent; int c2; ENT2 *aent2; char pad2[0x74 - 0x6C]; float g74;
           char pad3[0x98 - 0x78]; int x98, x9C, xA0, xA4, xA8, xAC; };
extern "C" {
float func_00117E38(BIS *pbis);
int func_00117D50(BIS *pbis);
int func_00117DB8(BIS *pbis);
int func_00117D00(BIS *pbis);
int func_00117BD0(BIS *pbis);
void *func_001AE1D0(AREQ *preq);
void *func_001AE2A0(AREQ *preq);
OBJ *PacNewKind(int id);
OBJ *PacNewKind2(int id);
void LoadAsegEventsFromBrx__FP4ASEGP18CBinaryInputStreamiPiPP2EA(X *px, BIS *pbis, int n, int *pa, int *pb);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(X *px, BIS *pbis);
void LoadAsegFromBrx(X *px, BIS *pbis);
}
void LoadAsegFromBrx(X *px, BIS *pbis) { int i; AREQ req;
 px->g74 = func_00117E38(pbis);
 px->c = func_00117D50(pbis);
 req.cbAlign = 0x10; req.cb = px->c * 8;
 px->aent = (ENT *)func_001AE1D0(&req);
 for (i = 0; i < px->c; i++) {
   ENT *pe = &px->aent[i];
   pe->n = func_00117DB8(pbis);
   pe->k = func_00117D50(pbis);
   int id = func_00117D00(pbis);
   if (id != -1) { pe->pobj = PacNewKind(id); pe->pobj->Load(pbis); pe->f31 = 0; }
   id = func_00117D00(pbis);
   if (id != -1) { pe->pobj = PacNewKind2(id); pe->pobj->Load(pbis); pe->f31 = 1; }
   pe->f30 = func_00117BD0(pbis) != 0;
 }
 px->c2 = func_00117BD0(pbis);
 req.cbAlign = 0x10; req.cb = px->c2 * 12;
 px->aent2 = (ENT2 *)func_001AE2A0(&req);
 for (i = 0; i < px->c2; i++) {
   ENT2 *pe = &px->aent2[i];
   pe->a = func_00117BD0(pbis);
   pe->b = func_00117DB8(pbis);
   pe->pobj = PacNewKind2(func_00117D00(pbis));
   pe->pobj->Load(pbis);
 }
 LoadAsegEventsFromBrx__FP4ASEGP18CBinaryInputStreamiPiPP2EA(px, pbis, 0, &px->x98, &px->x9C);
 LoadAsegEventsFromBrx__FP4ASEGP18CBinaryInputStreamiPiPP2EA(px, pbis, 1, &px->xA0, &px->xA4);
 LoadAsegEventsFromBrx__FP4ASEGP18CBinaryInputStreamiPiPP2EA(px, pbis, 0, &px->xA8, &px->xAC);
 LoadOptionsFromBrx__FPvP18CBinaryInputStream(px, pbis); }
