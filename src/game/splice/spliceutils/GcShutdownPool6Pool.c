// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002DD2D0, D_002DD2D4, D_002DD2D8, D_002DD2DC; extern char D_00452170[]; void ResetSlotheap(void *);
void GcShutdownPool6Pool(void) { D_002DD2D0 = 0; D_002DD2D8 = 0; D_002DD2DC = 0; D_002DD2D4 = 0; ResetSlotheap(D_00452170); }
