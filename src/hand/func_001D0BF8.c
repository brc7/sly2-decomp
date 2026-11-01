typedef int qword __attribute__((mode(TI)));
typedef struct { qword a[3]; } M3;
typedef struct { void *x0; void *x4; } S;
typedef struct { char pad[0x40]; M3 m; } R;
typedef struct { char pad[0x1C]; void *x1C; } OBJ;
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
R *func_001CF6B0(OBJ *, void *, void *, qword, qword);
void ConvertAloMat(M3 *, int, void *, M3 *);
void func_001CE980(OBJ *);
int func_001D0BF8(OBJ *p, S *s, qword a, qword b)
{
    R *r;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(s->x4, 0x33))
        return 0;
    r = func_001CF6B0(p, p->x1C, s->x0, a, b);
    if (r) {
        M3 m;
        ConvertAloMat(&m, 0, s->x4, &r->m);
        r->m = m;
    }
    func_001CE980(p);
    return 1;
}
