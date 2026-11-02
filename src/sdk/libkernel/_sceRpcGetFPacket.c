// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
struct rpcd { int pad[5]; char *buf; int n; int pad2[2]; int idx; };
void *_sceRpcGetFPacket(struct rpcd *d)
{
    int i = d->idx % d->n;
    char *p = d->buf + (i << 6);
    d->idx = i + 1;
    return p;
}
