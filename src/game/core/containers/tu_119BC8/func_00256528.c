// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { int *x0; char pad[0x24]; int x28; } B; typedef struct { int *x0; int x4, x8; B *xC; int x10; char pad[0xC]; int x20; } A;
void func_0011A070(void *, void *);
void func_00256528(A *a, B *b, int c) { a->x0 = b->x0; a->xC = b; a->x10 = c; func_0011A070(&b->x28, a); a->x20 = *a->x0; }
