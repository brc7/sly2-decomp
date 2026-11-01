struct S { char pad[0x18]; void *p18; };
int FBhvpatrolpathAtEnd(struct S *p);
int FBhvgotoArrived(void *p);
int FBhvpatrolpathDone(struct S *p) { if (!FBhvpatrolpathAtEnd(p)) return 0; return FBhvgotoArrived(p->p18); }
