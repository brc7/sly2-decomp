struct S { char pad[0x28]; unsigned long fl; };
void OnPoActive(struct S *p, int f);
void func_0017FD08(struct S *p, int f);
void func_0017F8C8(struct S *p, int f) { OnPoActive(p, f); if (f) func_0017FD08(p, 1); else if ((int)(p->fl >> 60) & 1) func_0017FD08(p, 0); }
