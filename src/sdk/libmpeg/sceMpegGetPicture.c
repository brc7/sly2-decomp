// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { char pad[0xC4]; int f_c4; char p2[0xEC - 0xC8]; int out; int f_f0; int f_f4; int size; } DEC;
typedef struct { char pad[0x40]; DEC *d; } sceMpeg;
int _getpic(sceMpeg *);
int sceMpegGetPicture(sceMpeg *mp, void *out, int size)
{
    DEC *d = mp->d;
    d->f_c4 = 1;
    d->out = ((unsigned int)out & 0x0FFFFFFF) | 0x20000000;
    d->size = size;
    d->f_f4 = 0;
    d->f_f0 = 0;
    return _getpic(mp);
}
