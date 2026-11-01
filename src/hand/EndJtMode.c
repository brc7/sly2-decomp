struct S { char pad[0x38]; int *p; };
void SetJtModeState__FP6JTMODEi(struct S *, int);
void EndJtMode(struct S *p) {
    if (p->p) {
        switch (*p->p) {
        case 0: case 8: case 9: case 10: return;
        }
    }
    SetJtModeState__FP6JTMODEi(p, 2);
}
