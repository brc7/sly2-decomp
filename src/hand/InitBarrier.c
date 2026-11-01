struct S { char pad[0x148]; unsigned long grf; };
void InitSo(struct S *);
void InitBarrier(struct S *p) { InitSo(p); p->grf |= 0x8000L << 19; }
