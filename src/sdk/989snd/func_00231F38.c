// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
extern int *D_002C4640; extern int D_002C4644;
void func_00234F48(void *, void *);
void func_00231F38(int *buf, int n)
{
    int *e;
    D_002C4644 = n;
    D_002C4640 = buf;
    buf[n + 1] = 0;
    buf[0] = 0;
    func_00234F48(buf, (char *)buf + 3);
    e = &buf[n + 1];
    func_00234F48(e, (char *)e + 3);
}
