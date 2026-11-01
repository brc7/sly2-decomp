extern char D_002C94A0[];
struct OBJ;
struct X;
extern "C" void func_001D4D30(X *,OBJ *);
extern "C" void *func_001AE4F8(int);
struct A { int x0; };
struct B : A {
    int x4;
    int x8;
    char *vt;
};
struct X : B {
    int x10;
    int x14;
    int x18;
    char pad1c[0x2C - 0x1C];
    int x2C;
    int pad30[2];
    int x38;
    char pad[0xB0 - 0x3C];
};
struct OBJ { char pad[0x4F8]; unsigned int n; X *a[8]; };
static inline X *CreateItem(OBJ *owner) {
 void *allocated=func_001AE4F8(sizeof(X));
 register X *item __asm__("$16");
 register unsigned page __asm__("$3");
 __asm__("lui %0,%%hi(%3)\n\tdaddu %1,%2,$zero"
         : "=&r"(page), "=r"(item) : "r"(allocated), "i"(D_002C94A0));
 register char *vtable __asm__("$3");
 __asm__("addiu %0,%1,%%lo(%2)":"=r"(vtable):"r"(page),"i"(D_002C94A0));
 item->x0=0;
 __asm__("sw %1,%0":"=m"(item->vt):"r"(vtable):"memory");
 register X*receiver __asm__("$4");__asm__ __volatile__("daddu %0,%2,$zero\n\tsw $zero,%1":"=&r"(receiver),"=m"(item->x4):"r"(item));func_001D4D30(receiver,owner);return item;
}
/** @sly2-annot addr=0x1D4B90
 * Purpose: Allocate and append weapon record (0xB0, vtable D_002C94A0) with ids/costs if fewer than 8
 * Info: proposed name AddRobottankWeapon (T2); TU tu_1D1370
 * Notes: names_v2 v1 proposed UpdateMatch__FP5MATCHf (0.001) | class identity from whole_game WE names/object
 *   strings: AddRcCarWeapon -> AddRobottankWeapon
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
extern "C" void func_001D4B90(OBJ *p, int a, int b, int inputC, int inputD)
{
    register int c __asm__("$20")=inputC;
    register int d __asm__("$21")=inputD;
    if (p->n < 8) {
        X *it = CreateItem(p);
        unsigned int index=p->n;
        it->x14=a;
        it->x18=b;
        p->a[index]=it;
        p->n=index+1;
        it->x38=d;
        it->x2C=c;
    }
}

#define OFF(T,F) ((unsigned int)&((T*)0)->F)
typedef char native_layout_0[(sizeof(int)==4)?1:-1];
typedef char native_layout_1[(sizeof(unsigned)==4)?1:-1];
typedef char native_layout_2[(sizeof(void*)==4)?1:-1];
typedef char native_layout_3[(sizeof(A)==4)?1:-1];
typedef char native_layout_4[(sizeof(B)==0x10)?1:-1];
typedef char native_layout_5[(sizeof(X)==0xB0)?1:-1];
typedef char native_layout_6[(OFF(X,x0)==0)?1:-1];
typedef char native_layout_7[(OFF(X,x4)==4)?1:-1];
typedef char native_layout_8[(OFF(X,vt)==0xC)?1:-1];
typedef char native_layout_9[(OFF(X,x10)==0x10)?1:-1];
typedef char native_layout_10[(OFF(X,x14)==0x14)?1:-1];
typedef char native_layout_11[(OFF(X,x18)==0x18)?1:-1];
typedef char native_layout_12[(OFF(X,x2C)==0x2C)?1:-1];
typedef char native_layout_13[(OFF(X,x38)==0x38)?1:-1];
typedef char native_layout_14[(OFF(OBJ,n)==0x4F8)?1:-1];
typedef char native_layout_15[(OFF(OBJ,a)==0x4FC)?1:-1];
