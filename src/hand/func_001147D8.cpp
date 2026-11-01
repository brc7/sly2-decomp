struct RGBA { unsigned char r, g, b, a; RGBA(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_) { r = r_; g = g_; b = b_; a = a_; } };
struct SLOTHEAP { int a[10]; };
extern RGBA D_0029A7C0[16];
extern char D_002DD500[], D_002DD520[], D_0027A778[], D_0029A790[];
extern int D_002DD2E0, D_002DD2E4, D_002DD2E8, D_002DD2EC, D_002DD2F0, D_002DD2F4;
extern SLOTHEAP D_004520A8, D_004520D0, D_004520F8, D_00452120, D_00452148, D_00452170;
extern "C" {
void func_001EAD98(void *p, void *q);
void func_001EB050(void *p, void *q);
void func_0011CCA8(SLOTHEAP *psh, int cb, int c, int cbAlign);
void func_001147D8(int fInit, int nPriority);
}
inline void *operator new(unsigned int, void *p) { return p; }
/** @sly2-annot addr=0x1147D8
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU tu_1147D8
 * Info: proposed name StaticInit_tu_1147D8 (T1); TU tu_1147D8
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)
 * See: docs/engine/core-library.md#6-source-files,
 *   docs/guides/naming-workflow.md#1-where-things-stand-master_names_v2,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_001147D8(int fInit, int nPriority) { if (nPriority == 0xFFFF && fInit) { RGBA *p = D_0029A7C0;
 new (p) RGBA(0,0,0,0xF); p++;
 new (p) RGBA(0x67,0x67,0x67,0x7D); p++;
 new (p) RGBA(0x67,0x67,0x67,0x54); p++;
 new (p) RGBA(0x67,0x67,0x67,0x3F); p++;
 new (p) RGBA(0x67,0x67,0x67,0x2A); p++;
 new (p) RGBA(0x67,0x67,0x67,0x76); p++;
 new (p) RGBA(0x67,0x67,0x67,0x6E); p++;
 new (p) RGBA(0x67,0x67,0x67,0x1F); p++;
 new (p) RGBA(0x67,0x67,0x67,0x4D); p++;
 new (p) RGBA(0x67,0x67,0x67,0x80); p++;
 new (p) RGBA(0x67,0x67,0x67,0x47); p++;
 new (p) RGBA(0x67,0x67,0x67,0x12); p++;
 new (p) RGBA(0x67,0x67,0x67,0x60); p++;
 new (p) RGBA(0x67,0x67,0x67,0x36); p++;
 new (p) RGBA(0,0,0,0x80); p++;
 new (p) RGBA(0x67,0x67,0x67,0);
 func_001EAD98(D_002DD500, D_0027A778);
 func_001EB050(D_002DD520, D_0029A790);
 if (++D_002DD2E0 == 1) func_0011CCA8(&D_004520A8, 0x14, 0x2000, 0x10);
 if (++D_002DD2E4 == 1) func_0011CCA8(&D_004520D0, 0x18, 0x800, 0x10);
 if (++D_002DD2E8 == 1) func_0011CCA8(&D_004520F8, 0x28, 0x200, 0x10);
 if (++D_002DD2EC == 1) func_0011CCA8(&D_00452120, 0x1C, 0x800, 0x10);
 if (++D_002DD2F0 == 1) func_0011CCA8(&D_00452148, 0xC, 0xC8, 0x10);
 if (++D_002DD2F4 == 1) func_0011CCA8(&D_00452170, 0x14, 0x100, 0x10); } }
