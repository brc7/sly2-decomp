typedef int qword __attribute__((mode(TI)));
struct VTE { short d,pad; void (*fn)(void *,int); };
struct POS { char pad[0x70]; qword pos; };
struct SHAPE {
    struct POS *pos; int pad4; float radius; int padC;
    unsigned long flags; char pad18[0xB0-0x18];
};
struct SO { char pad[0xE0]; int count; struct SHAPE *shapes; };
struct S { char pad[0xC]; struct VTE *vt; char pad10[0x1DC-0x10]; float radius; };
struct Q { int enabled,pad4; float radius; struct SO *so; };
extern float D_002C0D7C;
qword func_001A62E8(void *);
int func_001A93C8(qword,struct SHAPE *,int mask,int triangleCount,void *triangles,int capacity,void *contacts,float radius);
static inline void Trigger(struct S *p)
{
    struct VTE *m=&p->vt[76];
    m->fn((char *)p+m->d,1);
}
static inline qword ToVu(qword a)
{
    qword b;
    __asm__("qmtc2 %1,%0":"=j"(b):"r"(a));
    return b;
}
static inline qword Sub(qword a,qword b)
{
    qword d;
    __asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));
    return d;
}
static inline float LenSq(qword a)
{
    qword d,one;
    float f;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));
    __asm__("mtc1 %1,%0":"=f"(f):"r"(d));
    return f;
}
/** @sly2-annot addr=0x1C5B20
 * Purpose: test one collector (PZI) against the prize: in range -> collect
 * Info: proposed name FCheckDprizeCollect (T1); TU prize
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int FCheckDprizeCollect(struct S *p,struct Q *q)
{
    char scratch[0x30] __attribute__((aligned(16)));
    qword pos;
    int i;
    float distance,maximum;
    if(!q->enabled||!q->so)return 0;
    pos=ToVu(func_001A62E8(p));
    distance=LenSq(Sub(pos,func_001A62E8(q->so)));
    maximum=D_002C0D7C;
    if(maximum*maximum<distance)return 0;
    if(distance<q->radius*q->radius) {
        Trigger(p);
        return 1;
    }
    for(i=0;i<q->so->count;i++) {
        struct SHAPE *s=&q->so->shapes[i];
        if(s->flags&(1UL<<50))continue;
        if(s->flags&(1UL<<49)) {
            float r=p->radius+s->radius;
            qword here=func_001A62E8(p);
            if(LenSq(Sub(here,s->pos->pos))<r*r) {
                Trigger(p);
                return 1;
            }
        } else {
            qword here=func_001A62E8(p);
            if(func_001A93C8(here,s,0x7FFFFFFF,0,0,1,scratch,p->radius)) {
                Trigger(p);
                return 1;
            }
        }
    }
    return 0;
}

#define OFF(t,m) ((unsigned int)&((t*)0)->m)
typedef char trigger_layout[(sizeof(void*)==4 && sizeof(qword)==16 && __alignof__(qword)==16 && sizeof(unsigned long)==8 && sizeof(struct VTE)==8 && OFF(struct VTE,d)==0 && OFF(struct VTE,fn)==4 && OFF(struct POS,pos)==0x70 && sizeof(struct SHAPE)==0xB0 && OFF(struct SHAPE,pos)==0 && OFF(struct SHAPE,radius)==8 && OFF(struct SHAPE,flags)==0x10 && OFF(struct SO,count)==0xE0 && OFF(struct SO,shapes)==0xE4 && OFF(struct S,vt)==0xC && OFF(struct S,radius)==0x1DC && sizeof(struct Q)==0x10 && OFF(struct Q,enabled)==0 && OFF(struct Q,radius)==8 && OFF(struct Q,so)==0xC)?1:-1];
