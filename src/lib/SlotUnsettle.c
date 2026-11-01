// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { char pad[0x98]; unsigned short x98; } B;
void func_002303A0(void *, B *, int);
void SlotUnsettle(void *a, B *b)
{
    switch ((short)b->x98) {
    case 6: case 10: func_002303A0(a, b, 7); break;
    case 12: func_002303A0(a, b, 0xB); break;
    case 19: func_002303A0(a, b, 0x12); break;
    case 3: func_002303A0(a, b, 1); break;
    }
}
