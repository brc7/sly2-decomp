struct S { char pad[0xEC]; void *p; int n; };
int FTaskComplete(void *p);
int func_00203758(struct S *p) { if (p->p) return FTaskComplete(p->p); return p->n; }
