struct S { char pad[0x194]; void *p194; void *p198; };
int func_00131968(void *p);
int func_001F07E8(void *p);
int GetSmaGoal(void *p);
int func_0017F550(struct S *p) { if (p->p194) return func_00131968(p->p194); { int f = 1; if (p->p198) f = func_001F07E8(p->p198) == GetSmaGoal(p->p198); return f; } }
