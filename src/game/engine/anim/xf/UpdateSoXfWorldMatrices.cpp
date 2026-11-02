// Update linked world matrices through the VU matrix microprogram.
typedef int qword __attribute__((mode(TI)));
struct M4 {qword r[4];};
struct X {M4 local,world;char pad80[0x20];X *parent;int priority;char pada8[0x58];};
struct S {char pad[0x50];int count;X *array;};
extern M4 D_002A2D30;
extern "C" void func_0022FA20(S *);
/** @sly2-annot addr=0x22FAA0
 * Purpose: recomputes linked XF world matrices with the VU matrix microprogram
 * Info: proposed name UpdateSoXfWorldMatrices (T1); TU xf
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/animation.md#devtools-findings-v3-2026-11-01,
 *   docs/engine/animation.md#animation-format-summary-anim_lab-v31,
 *   docs/formats/meshes-scene.md#3-transforms-cstatic
 * @end-sly2-annot */
extern "C" void UpdateSoXfWorldMatrices(S *p) {
    if(p->count>0) {
        X *x=p->array,*last=p->array+p->count-1;
        register qword v1 __asm__("vf1"),v2 __asm__("vf2"),v3 __asm__("vf3"),v4 __asm__("vf4");
        register qword v5 __asm__("vf5"),v6 __asm__("vf6"),v7 __asm__("vf7"),v8 __asm__("vf8");
        qword n0,n1,n2,n3,p0,p1,p2,p3;
        if(x->parent) {X *parent=x->parent;v1=parent->world.r[0];v2=parent->world.r[1];v3=parent->world.r[2];v4=parent->world.r[3];}
        else {v1=D_002A2D30.r[0];v2=D_002A2D30.r[1];v3=D_002A2D30.r[2];v4=D_002A2D30.r[3];}
        v5=x->local.r[0];v6=x->local.r[1];v7=x->local.r[2];v8=x->local.r[3];
        while(x<=last) {
            int sync;
            __asm__ __volatile__("vcallms 0x60" : "=j"(v5),"=j"(v6),"=j"(v7),"=j"(v8) : "j"(v1),"j"(v2),"0"(v5),"1"(v6),"2"(v7),"3"(v8));
            if(x<last) {
                X *nx=x[1].parent;
                if(nx!=x) {
                    p0=nx->world.r[0];p1=nx->world.r[1];p2=nx->world.r[2];p3=nx->world.r[3];
                }
                n0=x[1].local.r[0];n1=x[1].local.r[1];n2=x[1].local.r[2];n3=x[1].local.r[3];
            }
            if(x->parent && x->priority<x->parent->priority)x->priority=x->parent->priority;
            __asm__ __volatile__("cfc2.i %0,$vi0":"=r"(sync):"j"(v3),"j"(v4),"j"(v5),"j"(v6),"j"(v7),"j"(v8));
            x->world.r[0]=v5;x->world.r[1]=v6;x->world.r[2]=v7;x->world.r[3]=v8;
            if(x[1].parent!=x) {v1=p0;v2=p1;v3=p2;v4=p3;}
            else {v1=v5;v2=v6;v3=v7;v4=v8;}
            ++x;
            v5=n0;v6=n1;v7=n2;v8=n3;
        }
    }
    func_0022FA20(p);
}
