inline void *operator new(unsigned int,void *storage){return storage;}
struct CLOCK {float t,dt;};
struct S;
struct E { struct S *object; int n4; float t8; int pad; E(S *source,const CLOCK&clock):object(source),n4(0),t8(clock.t){} };
struct T { char pad[0xFC]; unsigned int c; struct E a[8]; };
struct S { char pad[0x3A4]; int c3A4; char pad2[0xF64 - 0x3A8]; struct T *pt; };
extern "C" CLOCK D_002DDED0;
extern "C" struct E *func_00154180(struct T *pt, struct S *object);
extern "C" void *func_002332B8(void *d, void *s, int n);

/** @sly2-annot addr=0x2085D0
 * Purpose: Insert plo into the detector target list (8 max): fFront appends; else inserts before the
 *   generated tail (+0x3A4), evicting the oldest generated entry when full
 * Info: proposed name FAddStepguardDetectTarget (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
extern "C" int func_002085D0(struct S *p, struct S *object, int fFront)
{
    struct E *e;
    struct T *pt;
    register unsigned int n __asm__("$16");
    if (!object || object == p)
        return 0;
    e = 0;
    if (!fFront)
        e = func_00154180(p->pt, object);
    if (!e) {
        pt = p->pt;
        n = pt->c;
        if (n >= 8) {
            int c;
            if (!fFront) {
                c = p->c3A4;
                if (c > 0) {
                    p->c3A4 = c - 1;
                    n=8; n-=c; n*=16; n+=0x100;
                    e=(struct E*)((char*)pt+n);
                    func_002332B8(e + 1, e, (c - 1) * 16);
                    goto fill;
                }
            }
        } else {
            if (fFront) {
                register unsigned int offset __asm__("$2")=n*16+0x100;
                e=(struct E*)((char*)pt+offset);
            } else {
                int c = p->c3A4;
                n-=c; n*=16; n+=0x100;
                e=(struct E*)((char*)pt+n);
                func_002332B8(e + 1, e, c * 16);
            }
        fill:
            new(e) E(object,D_002DDED0);
            p->pt->c++;
            return 1;
        }
    }
    return 0;
}
