typedef struct CRef { int m_tagk; int m_tag; } CRef;
struct S { char pad[0x54]; int n; char pad2[0xD8 - 0x58]; int fd8; };
extern char D_003D4A60[];
void func_002581C0(CRef *pref, void *a, void *b);
void func_00129EC8(CRef *pref);
int FTaskComplete(struct S *p)
{
    if (p->n == -1 && p->fd8) {
        CRef ref;
        int n;
        func_002581C0(&ref, &p->fd8, D_003D4A60);
        n = ref.m_tag;
        func_00129EC8(&ref);
        return n;
    }
    return p->n >= 2;
}
