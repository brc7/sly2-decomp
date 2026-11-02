typedef int qword __attribute__((mode(TI)));
struct V {qword q;};
struct M4 { qword r[4];  };
extern "C" qword func_0022CAD0(qword,qword);
extern "C" M4 func_0022CCD0(void *,void *,V);
extern "C" M4 func_0022CB90(void *a,void *b,qword c,qword d) return m {
    V v;v.q=func_0022CAD0(c,d);
    m=func_0022CCD0(a,b,v);
}
