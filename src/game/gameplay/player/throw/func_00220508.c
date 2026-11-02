typedef int qword __attribute__((mode(TI)));
struct S { int a; int k; char pad[0x20]; float r; char pad2[0xB4]; qword e0; char pad3[0x10]; qword p100; qword p110; float t; float f; int g; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
qword func_001A62E8(void *q);
static inline float DivideBits(int bits,float denominator){register float value asm("$f0");register float rate asm("$f2")=denominator;__asm__ ("mtc1 %1,%0\n\tdiv.s %0,%0,%2":"=&f"(value):"r"(bits),"f"(rate));return value;}
static inline float ClockNow(void){float value;__asm__ __volatile__ ("lwc1 %0,%1":"=f"(value):"m"(D_002DDED0.t));return value;}
static inline void PublishStart(struct S *p,int active,float now,float span){asm("sw %3,%0\n\tswc1 %4,%1\n\tswc1 %5,%2":"=m"(p->g),"=m"(p->t),"=m"(p->f):"r"(active),"f"(now),"f"(span));}
static inline int Enabled(void){register int value asm("$3");asm("addiu %0,$zero,1":"=r"(value));return value;}
/** @sly2-annot addr=0x220508
 * Purpose: for states 4..6: store launch/target points, flight time = distance / speed, start time = now,
 *   active=1
 * Info: proposed name StartJtthrowFlight (T2); TU throw
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/player.md#throw-0x21f2e0-0x220d40
 * @end-sly2-annot */
void func_00220508(struct S *p, void *q)
{
    int k = p->k;
    if (k < 7) if (k >= 4) {
        qword v = func_001A62E8(q), a = p->e0, d, r, t;
        int len;
        int enabled=Enabled();
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(v));
        p->p110 = v;
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(r), "=&j"(t) : "0"(d));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(r));
        p->p100 = a;
        __asm__("qmfc2.ni %0, %1" : "=r"(len) : "j"(r));
        {
            float f = DivideBits(len,p->r);
            float now=ClockNow();
            PublishStart(p,enabled,now,f);
        }
    }
}
