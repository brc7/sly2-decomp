// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct S { char pad[0x10]; struct S *x10; char pad2[0x3C]; int x50; char pad3[0xC]; float m[16]; } S;
void InitLo(S *); void *func_002333B4(void *, int, int); void func_002313F0(S *);
void InitXfm(S *p)
{
    float *m; InitLo(p); if (p->x10) p->x50 = *(int *)((char *)p->x10 + 0x60); m = p->m; func_002333B4(m, 0, 0x40); m[0] = m[5] = m[10] = m[15] = 1.0f; func_002313F0(p);
}
