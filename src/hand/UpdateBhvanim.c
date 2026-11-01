struct S { char pad[0x30]; int n30; char pad2[0xC]; void *p40; };
void UpdateBhv(struct S *p);
int FAnimBindingDone(void *p);
void SetBhvanimState(struct S *p, int n);
void UpdateBhvanim(struct S *p) {
    int s;
    UpdateBhv(p);
    do {
        s = p->n30;
        if (s == 0 && FAnimBindingDone(p->p40)) SetBhvanimState(p, 1);
    } while (s != p->n30);
}
