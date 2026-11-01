struct K { float a; float b; };
struct Q { char pad[0x2C]; struct K k1; struct K k2; char p1[0x24]; struct Q *next; };
struct E { int a; int b; int c; int d; struct Q *pqFirst; void *pqLast; int ib; int x1c; float g; int x24; int x28; float h; };
struct S;
float SMaxShape(int n);
void func_00181B60(struct S *p, struct E *pe)
{
    float g = SMaxShape(pe->x24);
    int c = pe->d;
    float r = g * 0.5f / (float)c;
    int m;
    float fm, gap, step;
    struct K k1, k2;
    struct Q *q;
    r = pe->g < r ? pe->g : r;
    m = pe->c < c ? pe->c : c;
    fm = (float)m;
    gap = (g - 2.0f * (float)c * r) / (float)(c - m + 1);
    step = gap + 2.0f * r;
    k1.a = (2.0f * fm - 1.0f) * r - step * (float)(m - 1);
    k1.b = (2.0f * fm - 1.0f) * r + fm * gap - step * (float)(m - 1);
    k2.a = k1.a - fm * r;
    k2.b = k1.b + fm * r;
    for (q = pe->pqFirst; q; q = q->next) {
        q->k1 = k1;
        q->k2 = k2;
        k1.a += step;
        k1.b += step;
        k2.a += step;
        k2.b += step;
    }
}
