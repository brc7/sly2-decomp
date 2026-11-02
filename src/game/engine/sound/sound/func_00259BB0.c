// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { int pad[2]; short d; short p; void (*f)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;
void func_00259BB0(O **pp, O *o) { if (o) o->vt->f((char *)o + o->vt->d); *pp = o; }
