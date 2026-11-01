struct VTE { short d; short pad; void (*pfn)(); };
struct S { int pad[3]; struct VTE *vt; char pad2[0x208 - 0x10]; int f208; char pad3[0x1800 - 0x20c]; int st; };
void SetBentleyState(struct S *, int);
void func_001367D8(struct S *p) {
    int st;
    do {
        st = p->st;
        switch (st) {
        case 0: break;
        case 1:
            if (p->f208) {
                SetBentleyState(p, 0);
                ((void (*)(void *, int))p->vt[79].pfn)((char *)p + p->vt[79].d, 1);
            }
            break;
        }
    } while (p->st != st && p->st != -1);
}
