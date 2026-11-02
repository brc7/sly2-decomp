// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { char pad[0x28]; int f; } IMG;
typedef struct { char pad[0x1C8]; IMG *a0; IMG *b0; int p0[2]; IMG *a1; IMG *b1; int p1[2]; IMG *a2; IMG *b2; } DEC;
typedef struct { char pad[0x40]; DEC *d; } sceMpeg;
int sceMpegClearRefBuff(sceMpeg *mp)
{
    DEC *d = mp->d;
    if (d->a0) d->a0->f = 0;
    if (d->a1) d->a1->f = 0;
    if (d->a2) d->a2->f = 0;
    if (d->b0) d->b0->f = 0;
    if (d->b1) d->b1->f = 0;
    if (d->b2) d->b2->f = 0;
    return 1;
}
