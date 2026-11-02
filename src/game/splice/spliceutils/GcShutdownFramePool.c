// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002DD2A0, D_002DD2A4, D_002DD2A8, D_002DD2AC; extern char D_004520F8[]; void ResetSlotheap(void *);
void GcShutdownFramePool(void) { D_002DD2A0 = 0; D_002DD2A8 = 0; D_002DD2AC = 0; D_002DD2A4 = 0; ResetSlotheap(D_004520F8); }
