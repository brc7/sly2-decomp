struct U { char pad[0x40]; int n; };
struct T { char pad[0x60]; struct U *pu; };
struct S { char pad[0x200]; struct T *pt; };
static inline int PresentValue(void){register int r __asm__("$4");__asm__("addiu %0,$zero,%1":"=r"(r):"i"(1));return r;}
/** @sly2-annot addr=0x1FE848
 * Purpose: True if dialog (optionally of given id) is playing
 * Info: proposed name FDialogPlaying (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FE848(struct S *p, int n)
{
    struct U *pu; int f;
    if (p->pt) {
        pu = p->pt->pu;
        f = 0;
        if (n == -1 || pu->n == n) f = PresentValue();
        return f;
    }
    return 0;
}

typedef char guard_int_size[(sizeof(int)==4)?1:-1];
typedef char guard_pointer_size[(sizeof(void*)==4)?1:-1];
typedef char guard_selector[((unsigned)&((struct U*)0)->n==0x40)?1:-1];
typedef char guard_subobject[((unsigned)&((struct T*)0)->pu==0x60)?1:-1];
typedef char guard_owner[((unsigned)&((struct S*)0)->pt==0x200)?1:-1];
typedef char guard_selector_size[(sizeof(struct U)==0x44)?1:-1];
typedef char guard_subobject_size[(sizeof(struct T)==0x64)?1:-1];
typedef char guard_owner_size[(sizeof(struct S)==0x204)?1:-1];
