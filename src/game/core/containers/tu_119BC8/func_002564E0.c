// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { int x0; int x4; } B; typedef struct { B *p0; char pad[0x20]; int x24; } A;
void func_0011A070(void *, void *);
void func_002564E0(A *a, B *b) { a->p0 = b; func_0011A070(&b->x4, a); a->x24 = b->x0; }
