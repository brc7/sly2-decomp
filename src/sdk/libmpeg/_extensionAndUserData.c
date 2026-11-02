// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// (Claude agent) near miss 2/115: before 'jal _sceMpegNextBit(d,4)' target sets a0 then a1 (a1 in slot); ours a1 then a0.
typedef struct { char p0[0x878]; int stop; } D;
unsigned int _sceMpegNextBit(D *d, int n);
unsigned int _sceMpegPeepBit(D *d, int n);
void func_002544C0(D *d, int n);
void func_00254620(D *d);
void flushByteBoundary(D *d);
void func_002540A0(D *d);
void _sequenceExtension(D *d);
void _sequenceDisplayExtension(D *d);
void _quantMatrixExtension(D *d);
void _copyrightExtension(D *d);
void func_00254090(D *d);
void _pictureDisplayExtension(D *d);
void _pictureCodingExtension(D *d);
void func_00254298(D *d);
void func_002542A8(D *d);
static inline void _nextStartCode(D *d)
{
    flushByteBoundary(d);
    while (_sceMpegPeepBit(d, 24) != 1 && !d->stop)
        func_002544C0(d, 8);
}
void _extensionAndUserData(D *d)
{
    void (*tbl[11])(D *) = {
        func_002540A0, _sequenceExtension, _sequenceDisplayExtension, _quantMatrixExtension,
        _copyrightExtension, func_00254090, func_002540A0, _pictureDisplayExtension,
        _pictureCodingExtension, func_00254298, func_002542A8,
    };
    unsigned int code;

    _nextStartCode(d);
    while ((code = _sceMpegPeepBit(d, 32)) == 0x1B5 || code == 0x1B2) {
        if (code == 0x1B5) {
            unsigned int id;
            func_00254620(d);
            id = _sceMpegNextBit(d, 4);
            id = id > 10 ? 0 : id;
            tbl[id](d);
            _nextStartCode(d);
        } else {
            func_00254620(d);
            _nextStartCode(d);
        }
    }
}
