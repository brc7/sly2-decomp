void SetJtJts__FP2JT3JTS4JTBS(void *p, int n, int m);
void SetJtPlayerState(void *p, int n)
{
    switch (n) {
    case 0: SetJtJts__FP2JT3JTS4JTBS(p, 0, -1); break;
    case 1: SetJtJts__FP2JT3JTS4JTBS(p, 2, -1); break;
    case 2: SetJtJts__FP2JT3JTS4JTBS(p, 7, -1); break;
    case 3: SetJtJts__FP2JT3JTS4JTBS(p, 4, -1); break;
    case 4: SetJtJts__FP2JT3JTS4JTBS(p, 0x12, -1); break;
    case 5: SetJtJts__FP2JT3JTS4JTBS(p, 0x13, -1); break;
    case 6: SetJtJts__FP2JT3JTS4JTBS(p, 0x14, -1); break;
    case 7: SetJtJts__FP2JT3JTS4JTBS(p, 0x17, -1); break;
    case 8: SetJtJts__FP2JT3JTS4JTBS(p, 8, -1); break;
    }
}
