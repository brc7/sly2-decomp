struct S { int a, b; float g; };
int CompareJtGrabCand(struct S *a, struct S *b) { if (a->g < b->g) return -1; return 1; }
