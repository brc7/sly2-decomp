// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002DD2B0, D_002DD2B4, D_002DD2B8, D_002DD2BC; extern char D_00452120[]; void ResetSlotheap(void *);
void GcShutdownProcPool(void) { D_002DD2B0 = 0; D_002DD2B8 = 0; D_002DD2BC = 0; D_002DD2B4 = 0; ResetSlotheap(D_00452120); }
