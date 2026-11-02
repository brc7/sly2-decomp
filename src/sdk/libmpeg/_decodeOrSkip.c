// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { char pad[0x184]; int x184; } SYS; typedef struct { char pad[0x40]; SYS *sys; } MP;
int _decodeOrSkipFrame(MP *, int, int); int _decodeOrSkipField(MP *, int, int);
int _decodeOrSkip(MP *mp, int a, int b) { if (mp->sys->x184 == 3) return _decodeOrSkipFrame(mp, a, b); return _decodeOrSkipField(mp, a, b); }
