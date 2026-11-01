struct ENTRY;
struct S { char pad[0x2C0]; long mask; };
struct ENTRY *PanimbindBest(struct S *p, long mask);
struct ENTRY *func_00209BC0(struct S *p)
{
    return PanimbindBest(p, p->mask);
}
