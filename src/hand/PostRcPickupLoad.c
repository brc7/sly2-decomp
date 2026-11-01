struct S { char p0[0x160]; void *p160; int a164; };
void PostSoLoad(struct S *p);
void ReplaceSma(void *a, struct S *p, int b, int c, int *pn);
void SetRcPickupState(struct S *p, int n);
void PostRcPickupLoad(struct S *p) {
    PostSoLoad(p);
    ReplaceSma(p->p160, p, -1, 0, &p->a164);
    SetRcPickupState(p, 0);
}
