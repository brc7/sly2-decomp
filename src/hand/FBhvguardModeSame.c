struct S { int k; int a[3]; int n10; int b[2]; int n1C; };
int FBhvguardModeSame(struct S *a, struct S *b) {
    if (a->k != b->k) return 0;
    switch (a->k) {
    case 9: case 10: case 11: return a->n1C == b->n1C;
    case 4: return a->n10 == b->n10;
    case 5: return a->n10 == b->n10;
    case 3: return a->n10 == b->n10;
    case 13: return a->n10 == b->n10;
    case 2: return a->n10 == b->n10;
    case 14: return a->n10 == b->n10;
    case 18: return a->n10 == b->n10;
    case 6: return a->n10 == b->n10;
    }
    return 1;
}
