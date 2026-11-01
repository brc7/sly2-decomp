typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0x80]; qword v; };
struct S { char pad[0xE8]; struct T *pt; };
qword GetSoDvGravity(struct S *p) { if (!p->pt) return D_002A2DD0; return p->pt->v; }
