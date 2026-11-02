struct E { char pad[0x10]; int f; void *p; };
struct S { char pad[8]; struct E *ae; };
void func_001FB018(void *p);
static inline struct E *Entries(const struct S *p){struct E *value;__asm__("lw %0,8(%1)":"=r"(value):"0"(p),"m"(p->ae));return value;}
static inline void *SoundRead(void *const *slot){register void *value __asm__("$3");__asm__("lw %0,%1":"=r"(value):"m"(*slot));return value;}
static inline struct E *EntryAt(struct E *entries,int index){register struct E *value asm("$2");register int bytes asm("$5")=index*sizeof(struct E);asm("addu %0,%1,%2":"=r"(value):"r"(bytes),"r"(entries));return value;}
/** @sly2-annot addr=0x218748
 * Purpose: mark entry done and stop/pause its sound
 * Info: proposed name FinishSpriteSoundEvent (T2); TU hud
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00218748(struct S *p, int i)
{
    void *q;
    p->ae[i].f = 1;
    q=SoundRead(&EntryAt(Entries(p),i)->p);
    if (q)
        func_001FB018(q);
}
