// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
extern int *gCommBusy; extern int gAwaitingInts;
void SyncDCache(void *, void *);
void snd_PrepareReturnBuffer(int *buf, int n)
{
    int *e;
    gAwaitingInts = n;
    gCommBusy = buf;
    buf[n + 1] = 0;
    buf[0] = 0;
    SyncDCache(buf, (char *)buf + 3);
    e = &buf[n + 1];
    SyncDCache(e, (char *)e + 3);
}
