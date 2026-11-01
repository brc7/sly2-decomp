typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct E { char pad[0x1C]; float radius; };
struct A { char pad[0x10]; qword v10; char pad2[0x30 - 0x20]; struct E *pe30; };
struct B { char pad[0x10]; int state; char pad2[0x38 - 0x14]; struct E *pe38; };
struct Entry { int kind; int unused; void *object; };
union FLOATWORD { float value; int bits; };
struct Param { int pad[2]; union FLOATWORD *value; };
struct S { char pad[0x190]; qword v190; char pad2[0x1E0 - 0x1A0]; qword v1E0; char pad3[0xFC4 - 0x1F0]; struct Param *param; };
extern qword D_002A2DD0;
qword func_00204E38(struct S *, struct E *, float);
qword func_00204ED8(struct S *, struct E *, float);
void UpdateLeap(struct B *);
void func_00209618(struct Entry *entry, struct S *s)
{
    U factor, value;
    register qword scalar __asm__("vf1");
    qword extra;
    register qword neg __asm__("vf2");
    {register qword *zeroAddress __asm__("v0")=&D_002A2DD0;
     register qword zero __asm__("v0");
     __asm__("lq %0,0(%1)":"=r"(zero):"r"(zeroAddress),"m"(*zeroAddress));
     __asm__("sq %1,%0":"=m"(value.q):"r"(zero));}
    extra=value.q;
    { register int kind __asm__("v1");
      __asm__("lw %0,%1":"=r"(kind):"m"(entry->kind));
    switch (kind) {
    case 0:
    case 1:
        {
            struct E *e;
            if (kind == 0) e = (struct E *)entry->object;
            else e = ((struct A *)entry->object)->pe30;
            if (e) {
                value.q = func_00204E38(s, e, e->radius);
                extra = func_00204ED8(s, e, e->radius);
            }
        }
        break;
    case 2:
        {
            struct B *b;
            UpdateLeap((struct B *)entry->object);
            b = (struct B *)entry->object;
            if (b->state > 0 && (b->state < 3 || b->state == 4)) break;
            value.q = func_00204E38(s, b->pe38, b->pe38->radius);
        }
        break;
    case 3: {
        register struct A *object __asm__("v0")=(struct A *)entry->object;
        register qword vector __asm__("v0");
        __asm__("lq %0,16(%1)":"=r"(vector):"r"(object),"m"(object->v10));
        value.q=vector; }
        break;
    }
    }
    {
      register qword current __asm__("vf2")=value.q;
      if (s->param) {
        register union FLOATWORD *wordAddress __asm__("v0")=s->param->value;
        register int parameterWord __asm__("v0");
        __asm__("lw %0,0(%2)\n\tqmtc2.ni %0,%1":"=r"(parameterWord),"=j"(scalar):"r"(wordAddress),"m"(wordAddress->bits));
        factor.q=scalar;
        __asm__("vmulx.xyzw %0,%0,%1x":"+j"(current):"j"(factor.q));
        value.q=current;
      }
    }
    s->v1E0 = extra;
    __asm__ __volatile__("qmtc2.ni %1, %0" : "=j"(scalar) : "r"(-1.0f));
    factor.q=scalar;
    { register qword source __asm__("vf3")=value.q;
      __asm__("vmulx.xyzw %0,%1,%2x":"=j"(neg):"j"(source),"j"(factor.q)); }
    s->v190 = neg;
}
