struct CRef { int tagk; int tag; };
void func_002013D8(struct CRef *pref, void *p, int a, int b, int *pn, int *an);
unsigned long GrfFromSgfArray(int c, int *a);
void func_00129EC8(struct CRef *pref);
unsigned long func_00207B10(void *p) { int an[16]; struct CRef ref; int c; unsigned long m; func_002013D8(&ref, p, 0x1010, 0x10, &c, an); m = GrfFromSgfArray(c, an); func_00129EC8(&ref); return m; }
