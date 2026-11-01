typedef int qword __attribute__((mode(TI)));
typedef union { qword r[4]; float m[4][4]; } M4;
struct M3 {qword r[3];};
union FRAME {M4 matrix;M3 basis;};
struct JOB;
struct O;
union SOURCE {O *object;float deadline;};
union MOTIONDATA {qword r[4];M3 rotation;};
struct O {
 FRAME local;M4 world;float g80,g84;float *pg88,*pg8C;JOB *job;int k94;
 short k98,k9A;unsigned tag;O *parent;int priority;SOURCE source;
 unsigned short force,damping;qword qB0;MOTIONDATA data;
};
extern qword D_002A2DD0;
extern "C" void *func_0023320C(void *d, void *s, int n);
extern "C" void *func_002333B4(void *p, int c, int n);
extern "C" M4 *func_0022CCD0(M4 *pm, struct O *pa, const M3 *basis, qword v);
extern "C" M4 *func_0022CB90(M4 *pr, struct O *pa, const M3 *basis, qword c, qword d);
extern "C" M4 *func_0022CC00(M4 *pm, struct O *po, qword v);
extern "C" qword func_0022CA18(struct O *pa, const M4 *matrix);
extern "C" qword func_0011BFF0(const M3 *rotation);
extern "C" void EvaluateBezierPos(float g0, float g1, float g2, qword q0, qword q1, qword q2, qword q3, qword *pa, qword *pb, qword *pc);
static __inline__ void Eval(struct O *po, qword *pa, qword *pb, qword *pc)
{
    EvaluateBezierPos(po->g84 - po->g80, *po->pg88 - po->g80, *po->pg8C, po->data.r[0], po->data.r[1], po->data.r[2], po->data.r[3], pa, pb, pc);
}
static inline qword SelectedValue(qword input){qword value;__asm__("por %0,$zero,%1":"=r"(value):"r"(input));return value;}
static inline qword DefaultPoint(){qword value;__asm__("lq %0,0(%1)":"=r"(value):"0"(&D_002A2DD0),"m"(D_002A2DD0));return value;}
extern "C" M4 *func_0022CDC0(M4 *pr, struct O *po)
{
    M4 m, m2;
    switch (po->k98) {
    case 0: case 3: case 10: case 12: case 13: case 14: case 15: case 16: case 19:
        func_002333B4(pr, 0, 0x40);
        break;
    default:
        switch (po->k94) {
        case 0: case 2: case 3: case 13: case 15:
            break;
        case 8: case 14: {
            func_0022CDC0(&m, po->source.object);
            func_0023320C(pr, &m, 0x40);
            pr->m[3][0] = -pr->m[3][0];
            pr->m[3][1] = -pr->m[3][1];
            pr->m[3][2] = -pr->m[3][2];
            break;
        }
        case 6: case 7:
            switch (po->k98) {
            case 1: case 2: case 7: case 8: case 9: case 11: case 17: case 18: case 20: case 21: {
                func_0022CCD0(&m, po, &po->local.basis, po->qB0);
                func_0023320C(pr, &m, 0x40);
                break;
            }
            case 5: case 6:
                if (po->k9A == 5) {
                    func_002333B4(pr, 0, 0x40);
                } else {
                    qword q, t;
                    register qword initial __asm__("$2");
                    switch (po->k9A) {
                    case 0:
                        initial = func_0022CA18(po, &po->local.matrix);
                        break;
                    case 1:
                        Eval(po, &m.r[0], 0, 0);
                        initial = m.r[0];
                        break;
                    case 2: case 3: case 4:
                        initial = po->data.r[0];
                        break;
                    case 5:
                        initial = func_0011BFF0(&po->data.rotation);
                        break;
                    default:
                        initial = DefaultPoint();
                        break;
                    }
                    q=SelectedValue(initial);
                    switch (po->k9A) {
                    case 3: case 4:
                        t = po->data.r[1];
                        break;
                    case 1:
                        Eval(po, 0, &m.r[0], 0);
                        t = m.r[0];
                        break;
                    case 0: case 2: case 5: default:
                        t = D_002A2DD0;
                        break;
                    }
                    func_0022CB90(&m, po, &po->local.basis, q, t);
                    func_0023320C(pr, &m, 0x40);
                }
                break;
            }
            break;
        case 1: case 4: case 5: case 9: case 10: case 11: case 12: {
            switch (po->k98) {
            case 1: case 2: case 7: case 8: case 9: case 11: case 17: case 18: case 20: case 21:
                m.r[0] = po->qB0;
                break;
            case 5: case 6:
                {qword *output=&m.r[0];qword value;
                switch (po->k9A) {
                case 3: case 4:
                    value = po->data.r[1];
                    break;
                case 1:
                    Eval(po, 0, &m.r[1], 0);
                    value = m.r[1];
                    break;
                case 0: case 2: case 5: default:
                    value = D_002A2DD0;
                    break;
                }
                *output=value;
                }
                break;
            }
            func_0022CC00(&m2, po, m.r[0]);
            func_0023320C(pr, &m2, 0x40);
            break;
        }
        }
        break;
    }
    return pr;
}
