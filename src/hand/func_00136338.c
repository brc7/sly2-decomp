struct CLOCK { float t; };
struct P { char pad[0x150]; void *p150; char pad0[0xF20 - 0x154]; int xF20; char pad1[0x1028 - 0xF24]; void *p1028; int pad2[2];
    void *p1034; char pad3[0x13D0 - 0x1038]; int x13D0; char pad4[0x16BC - 0x13D4]; int x16BC; char pad5[0x1700 - 0x16C0];
    float g1700; int pad6[3]; char a1710[4]; int x1714; char pad7[0x181C - 0x1718]; int x181C; };
extern struct CLOCK D_002DDED0;
extern float D_002BC994;
int func_001443D8(struct P *p);
void func_001362F8(struct P *p, int n);
int GrfConsumePressed__5POCTLUif(void *pv, int n, float g);
void func_001357C0(struct P *p, int n);
void func_00136DC0(void *pv, int n);
int func_001F0400(void *pv, int n);
void SnapSma(void *pv, int n);
void func_00136338(struct P *p)
{
    int s;
    do {
        s = p->x16BC;
        switch (s) {
        case 0:
        case 1:
            if (!func_001443D8(p)) {
                func_001362F8(p, -1);
            } else if (p->x16BC == 1) {
                if (D_002BC994 < D_002DDED0.t - p->g1700)
                    func_001362F8(p, 0);
            } else if (GrfConsumePressed__5POCTLUif(p->p150, 8, 0.25f)) {
                func_001357C0(p, 0x80000);
            }
            break;
        case -1:
            if (func_001443D8(p) && p->xF20 != 2) {
                if (D_002BC994 < D_002DDED0.t - p->g1700)
                    func_001362F8(p, 0);
                else
                    func_001362F8(p, 1);
            }
            switch (p->x13D0) {
            case 0:
                func_00136DC0(p->a1710, 0);
                if (p->x181C && p->x1714 != -1 && p->p1034 && func_001F0400(p->p1028, 0x17B) >= 0)
                    SnapSma(p->p1034, 0x17B);
                break;
            case 1: case 2: case 3: case 7:
                func_00136DC0(p->a1710, 1);
                break;
            case 5:
                func_00136DC0(p->a1710, 2);
                break;
            default:
                func_00136DC0(p->a1710, -1);
                break;
            }
            break;
        }
    } while (s != p->x16BC);
}
