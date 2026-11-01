struct AR { int cb; int cbAlign; };
struct X { int id; unsigned char b4; signed char cch; char pad[2]; unsigned short c; char pad2[2]; int *pv; unsigned int cqw; };
struct Y { char pad[0xC]; int *pv; };
void func_00117968(void *pbis, int cb, void *pv);
void func_001179B8(void *pbis, int cb);
int FMatchLoadFilter__FiPs(int cch, void *pw);
struct Y *func_001ED5D0(int id);
void *func_001AE1D0(struct AR *par);
void func_001ED278(int *p);
void func_001EA8A8(void *pbis, struct X *px)
{
    union { short aw[0x80]; struct AR ar; } u;
    unsigned int cb;
    func_00117968(pbis, 0x10, px);
    cb = px->c * sizeof(int);
    if (px->cch > 0) {
        func_00117968(pbis, px->cch * 2, u.aw);
        if (!FMatchLoadFilter__FiPs(px->cch, u.aw)) {
            if (px->b4 != 1)
                func_001179B8(pbis, cb);
            px->b4 = 0;
            return;
        }
    }
    if (px->b4 != 1) {
        struct Y *py = func_001ED5D0(px->id);
        if (py) {
            px->pv = py->pv;
            func_001179B8(pbis, cb);
        } else {
            int *pn;
            u.ar.cb = cb;
            u.ar.cbAlign = 0x10;
            px->pv = func_001AE1D0(&u.ar);
            func_00117968(pbis, cb, px->pv);
            for (pn = px->pv; pn < px->pv + px->c; pn++)
                func_001ED278(pn);
        }
    }
    px->cqw = (cb + 15) >> 4;
}
