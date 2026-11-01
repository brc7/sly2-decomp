typedef int qword __attribute__((mode(TI)));
struct R { char pad[0x40]; float f40; float f44; char pad2[0x78]; };
int func_001C1700(void *a, void *b, qword q2, qword q3, int c, void **buf);
int ClsgClipEdgeToCnvo(void *e, qword q2, qword q3, int n, struct R *r);
void *func_001BD310(void *a, void *b, qword q2, qword q3)
{
    void *buf[0x80];
    struct R r;
    void *best = 0;
    float min;
    int i, n;
    n = func_001C1700(a, b, q2, q3, 0x80, buf);
    min = 1.0f;
    for (i = 0; i < n; i++) {
        void *e = buf[i];
        if (!ClsgClipEdgeToCnvo(e, q2, q3, 2, &r) || r.f40 != 0.0f) {
            best = e;
            break;
        }
        if (r.f44 >= min) continue;
        min = r.f44;
        best = e;
    }
    return best;
}
