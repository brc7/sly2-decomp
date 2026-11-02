struct B { int a[12]; };
struct MK { int a; };
extern int D_003DCF9C, D_003DCF98;
extern int D_002DE598;
void func_00166920(struct B *pb);
struct MK *STACKSCOPE_ctor(struct MK *pm);
void STACKSCOPE_ctor_1AE4A0(struct MK *pm, int n);
void func_001572E0(struct B *pb, int n);
void func_001574B8(struct B *pb);
int func_001ECC28(int a, void *p, struct B *pb);
void func_00166BD8(struct B *pb, int a, int b, int c);
void func_00166E90(struct B *pb, int a, int b);
void func_00166968(struct B *pb);
void func_001576B0(struct B *pb);
void func_00157348(struct B *pb, int a, int *pn);
void func_0016F8B8(int a, int b);
/** @sly2-annot addr=0x170730
 * Purpose: Builds and sends (sync) a GIF chain uploading every level bitmap/CLUT of class 1 into GS memory
 *   allocated from the GSB, then TEXFLUSH + FINISH
 * Info: proposed name UploadBitmaps (T1); TU gs
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8+format-doc:textures
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse
 * @end-sly2-annot */
void UploadBitmaps__FP3GSB(void *p) {
    struct B b;
    struct MK mk;
    int n;
    func_00166920(&b);
    STACKSCOPE_ctor(&mk);
    func_001572E0(&b, ((D_003DCF9C * 3 + D_003DCF98) << 3) | 4);
    func_001574B8(&b);
    if (func_001ECC28(1, p, &b)) {
        func_00166BD8(&b, 0, 1, 0xE);
        func_00166E90(&b, 0x3F, 0);
        func_00166E90(&b, 0x61, 0);
        func_00166968(&b);
        func_001576B0(&b);
        func_00157348(&b, 0, &n);
        func_0016F8B8(D_002DE598, n);
    }
    STACKSCOPE_ctor_1AE4A0(&mk, 2);
}
