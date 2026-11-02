// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int sceMcOpen(int, int, const char *, int);
extern int _mc_errno;
int sceMcMkdir(int port, int slot, const char *name)
{
    int r = sceMcOpen(port, slot, name, 0x40);
    if (r == 0) _mc_errno = 11;
    return r;
}
