// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int _sceSifLoadModule(const char *, int, const char *, int *);
int s1_sceSifLoadModule(const char *path, int argc, const char *argp)
{
    int r;
    return _sceSifLoadModule(path, argc, argp, &r);
}
