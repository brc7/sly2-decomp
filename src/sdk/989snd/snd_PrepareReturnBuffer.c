// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
extern int *gCommBusy; extern int gAwaitingInts;
void func_00234F48(void *, void *);
void snd_PrepareReturnBuffer(int *buf, int n)
{
    int *e;
    gAwaitingInts = n;
    gCommBusy = buf;
    buf[n + 1] = 0;
    buf[0] = 0;
    func_00234F48(buf, (char *)buf + 3);
    e = &buf[n + 1];
    func_00234F48(e, (char *)e + 3);
}
