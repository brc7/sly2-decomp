typedef struct { int x0; int x4; int x8; void *vptr; } BASE;
extern char D_003D9E70[];
extern BASE D_003D9E80, D_003D9ED0, D_003DA0C0;
extern char D_002D8668[], D_002D37D0[], D_002CA490[];
extern char D_003DA0F0[], D_003DA150[], D_003DA3C0[], D_003DA4F0[], D_003DA530[];
extern int D_002DD2E0, D_002DD2E4, D_002DD2E8, D_002DD2EC, D_002DD2F0, D_002DD2F4;
extern char D_004520A8[], D_004520D0[], D_004520F8[], D_00452120[], D_00452148[], D_00452170[];
void *func_001D93D8(void *); /* constructor: returns this */
void *GADGETMENU_ctor(void *); /* constructor: returns this */
void *func_001DB7A8(void *); /* constructor: returns this */
void *func_001DCC58(void *); /* constructor: returns this */
void *func_001DD648(void *); /* constructor: returns this */
void *func_001DD7D8(void *); /* constructor: returns this */
void func_0011CCA8(void *, int, int, int);
void func_00258348(void *, int);
static inline void InitBase(BASE *p, void *vt)
{
    p->x0 = 0;
    p->x4 = 0;
    p->vptr = vt;
}
/** @sly2-annot addr=0x1DE008
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU screen
 * Info: proposed name StaticInit_screen (T1); TU screen
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r6b_rest
 * @end-sly2-annot */
void StaticInit_screen(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init) {
            func_001D93D8(D_003D9E70);
            InitBase(&D_003D9E80, D_002D8668);
            InitBase(&D_003D9ED0, D_002D37D0);
            InitBase(&D_003DA0C0, D_002CA490);
            GADGETMENU_ctor(D_003DA0F0);
            func_001DB7A8(D_003DA150);
            func_001DCC58(D_003DA3C0);
            func_001DD648(D_003DA4F0);
            func_001DD7D8(D_003DA530);
            if (++D_002DD2E0 == 1)
                func_0011CCA8(D_004520A8, 0x14, 0x2000, 0x10);
            if (++D_002DD2E4 == 1)
                func_0011CCA8(D_004520D0, 0x18, 0x800, 0x10);
            if (++D_002DD2E8 == 1)
                func_0011CCA8(D_004520F8, 0x28, 0x200, 0x10);
            if (++D_002DD2EC == 1)
                func_0011CCA8(D_00452120, 0x1C, 0x800, 0x10);
            if (++D_002DD2F0 == 1)
                func_0011CCA8(D_00452148, 0xC, 0xC8, 0x10);
            if (++D_002DD2F4 == 1)
                func_0011CCA8(D_00452170, 0x14, 0x100, 0x10);
        } else {
            func_00258348(D_003DA530, 2);
            func_00258348(D_003DA4F0, 2);
            func_00258348(D_003DA3C0, 2);
            func_00258348(D_003DA150, 2);
            func_00258348(D_003DA0F0, 2);
            func_00258348(&D_003DA0C0, 2);
            func_00258348(&D_003D9ED0, 2);
            func_00258348(&D_003D9E80, 2);
        }
    }
}
