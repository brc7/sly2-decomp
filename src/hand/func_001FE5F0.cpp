struct VTE { short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; VTE *pvt; };
struct Ref;
extern "C" void func_00259B48(Ref *, X *);
struct Ref {
    X *px;
    Ref &operator=(X *p) { func_00259B48(this, p); return *this; }
    operator X *() { return px; }
};
struct D { char pad[0x48]; int n48; char pad2[0x54 - 0x4C]; int n54; };
struct P { char pad[0x200]; Ref r200; char pad2[0x22C - 0x204]; X *a[6]; int n244; int n248; int n24C; };
struct G;
extern G D_003DD490;
extern char D_004435F0[];
extern "C" {
void func_001FE738(P *, int);
D *func_001FCFA0(P *, int);
X *func_001FC138(D *, int);
void func_00227728(void *, int, int);
X *func_0015ED78(G *, int, int, int);
int func_001FD0C0(P *, int);
X *func_001F67E0(X *, int);
}
/** @sly2-annot addr=0x1FE5F0
 * Purpose: Stop current dialog, create dialog STREAMSND kind 0, notify subtitles (0x227728), bind the speaker
 *   object SMs for 6 lip-sync roles
 * Info: proposed name StartDialog (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
extern "C" X *func_001FE5F0(P *p, int k, int f)
{
    D *pd;
    func_001FE738(p, k);
    pd = func_001FCFA0(p, k);
    if (!pd) return 0;
    p->r200 = func_001FC138(pd, 0);
    p->n244 = 0;
    p->n248 = 0;
    p->n24C = 0;
    if (!f) func_00227728(D_004435F0, k, pd->n48);
    if (pd->n48 != -1 && pd->n54) {
        register X *po __asm__("$19")=func_0015ED78(&D_003DD490, 5, pd->n48, 0);
        if (po) {
            register int i __asm__("$18")=0;
            register int primaryRole __asm__("$20")=3;
            register X **slot __asm__("$16")=p->a;
            for(;i<6;i++,slot++) {
                if(i==primaryRole)p->a[3]=po;
                else *slot=func_001F67E0(po,func_001FD0C0(p,i));
                { X *current=*slot;
                  if(current) { VTE &entry=current->pvt[1];
                    entry.pfn((char *)current+entry.d); } }
            }
        }
    }
    return p->r200;
}
