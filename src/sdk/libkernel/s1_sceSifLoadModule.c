// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
/* sceSifLoadModuleBuffer: load an IRX image already in EE memory (main.cpp passes the .irx images); the work is
   _sceSifLoadModuleBuffer's (0x238F38). Was named s1_sceSifLoadModule (its callee _sceSifLoadModule). */
int _sceSifLoadModuleBuffer(const char *, int, const char *, int *);
// Also known as: s1_sceSifLoadModule (upstream readable / Sly 1 name).
int sceSifLoadModuleBuffer(const char *path, int argc, const char *argp)
{
    int r;
    return _sceSifLoadModuleBuffer(path, argc, argp, &r);
}
