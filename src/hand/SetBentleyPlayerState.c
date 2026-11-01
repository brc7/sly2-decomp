void SetBentleyState(void *, int);
void SetBentleyPlayerState(void *p, unsigned int n) {
    switch (n) {
    case 0: SetBentleyState(p, 0); break;
    case 1: SetBentleyState(p, 2); break;
    case 2: SetBentleyState(p, 3); break;
    case 3: SetBentleyState(p, 4); break;
    case 4: SetBentleyState(p, 9); break;
    case 5: SetBentleyState(p, 10); break;
    case 6: SetBentleyState(p, 11); break;
    case 7: SetBentleyState(p, 12); break;
    case 8: SetBentleyState(p, 13); break;
    }
}
