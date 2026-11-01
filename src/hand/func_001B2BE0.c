typedef struct S {
    int x0; int x4; int x8; int xC; int x10; int x14; int x18; int x1C;
    int x20; int x24; int x28; int x2C;
} S;
void func_001B2AA0(S *p, int cb, char *pb);
int func_001B2DB8(S *p, int cb, char *pb);

/** @sly2-annot addr=0x1B2BE0
 * Purpose: Write cb bytes into the IOP ring buffer in chunks via CbSend; returns bytes written
 * Info: proposed name CQueueOutputIop_CbWrite (T1); class CQueueOutputIop; TU mpeg
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int func_001B2BE0(S *p, int cb, char *pb)
{
    int c = cb;
    int total = 0;
    int rd;
    int n, r, avail;
    char *dst;

    if (c <= 0 || p->x24 < c) c = p->x24;
    if (p->x4 == 0x10) {
        c &= -(p->xC << 10);
        func_001B2AA0(p, c, pb);
    } else {
        c &= -0x400;
    }
    if (c) do {
        avail = p->x18 - p->x1C;
        if (c < avail) n = c; else n = avail;
        dst = pb + total;
        if (p->x4 == 0x10) {
            n = 0x400;
            r = p->x2C / 1024 / p->xC % 4;
            if (r == 0) {
                dst[1] = 6;
                dst[0x11] = 2;
            } else if (r == 3) {
                dst[0x3F1] = r;
            }
        }
        rd = func_001B2DB8(p, n, dst);
        c -= rd;
        if (!rd) break;
        total += rd;
        p->x1C += rd;
        p->x1C %= p->x18;
        p->x28 += rd;
        p->x24 -= rd;
        p->x2C += rd;
        if (cb <= 0) break;
    } while (c);
    return total;
}
