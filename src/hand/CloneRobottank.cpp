extern char D_002C94A0[];
struct OBJ;struct X;extern "C" void func_001D4D30(X*,OBJ*);
extern "C" void *func_001AE4F8(int);
struct A { int x0; int x4; A() {} };
struct B : A {
    int x8;
    char *vt;
};
struct X : B {
    X(OBJ *p) {
 // Keep header zeroing before the remaining constructor stores.
 register X *receiver __asm__("$4");
 __asm__("daddu %0,%2,$zero\n\tsw $zero,%1"
         : "=&r"(receiver), "=m"(x0) : "r"(this) : "memory");
 x4=0;vt=D_002C94A0;func_001D4D30(receiver,p);
}
    void *operator new(unsigned int cb) { return func_001AE4F8(cb); }

    char pad[0xB0 - 0x10];
};
struct OBJ { char pad[0x4F8]; int c; X *a[1]; };
extern "C" {
void ClonePo(OBJ *);
void func_001D4DE0(X *, X *);
/** @sly2-annot addr=0x1D1410
 * Purpose: CID 77 vt41 Clone: ClonePo then deep-copy each weapon record (new 0xB0 object, vtable D_002C94A0,
 *   init 0x1D4D30, copy 0x9C bytes)
 * Info: proposed name CloneRobottank (T1); class ROBOTTANK slot41; TU tu_1D1370
 * Notes: names_v2 v1 proposed FBounceFlying__FP6FLYINGP2SOP6VECTORT2 (0.003) | class identity from whole_game
 *   WE names/object strings: CloneRcCar -> CloneRobottank
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void CloneRobottank(OBJ *p)
{
    int i;
    ClonePo(p);
    for (i = 0; i < p->c; i++) {
        X *old = p->a[i];
        X *n = new X(p);
        func_001D4DE0(n, old);
        p->a[i] = n;
    }
}
}

#define OFF(T,F) ((unsigned int)&((T*)0)->F)
typedef char native_layout_0[(sizeof(int)==4)?1:-1];
typedef char native_layout_1[(sizeof(void*)==4)?1:-1];
typedef char native_layout_2[(sizeof(A)==8)?1:-1];
typedef char native_layout_3[(sizeof(B)==0x10)?1:-1];
typedef char native_layout_4[(sizeof(X)==0xB0)?1:-1];
typedef char native_layout_5[(OFF(A,x0)==0)?1:-1];
typedef char native_layout_6[(OFF(A,x4)==4)?1:-1];
typedef char native_layout_7[(OFF(B,x8)==8)?1:-1];
typedef char native_layout_8[(OFF(B,vt)==0xC)?1:-1];
typedef char native_layout_9[(OFF(X,x0)==0)?1:-1];
typedef char native_layout_10[(OFF(X,x4)==4)?1:-1];
typedef char native_layout_11[(OFF(X,vt)==0xC)?1:-1];
typedef char native_layout_12[(OFF(OBJ,c)==0x4F8)?1:-1];
typedef char native_layout_13[(OFF(OBJ,a)==0x4FC)?1:-1];
