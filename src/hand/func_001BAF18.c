typedef struct VTE { short d; short pad; void (*pfn)(); } VTE;
typedef struct SO { char pad[0xC]; VTE *vt; } SO;
typedef struct I { int kind; int value; } I;
typedef struct E { int id; int unknown4; int actionCount; I *actions; } E;
typedef struct P {
    char pad0[0x158];
    int marker;
    int current;
    int eventCount;
    E *events;
} P;
extern char D_003DD490[];
SO *PloFindSwObject__FP2SWi3OIDP2LO(void *g, int k, int v, P *p);
void ShowSoGlobsById(P *p, int v);
void HideSoGlobsById(P *p, int v);
void ShowSoPartsById(P *p, int v);
void func_001F6B48(P *p, int v);

enum { EVENT_SHIFT = 4 };
void func_001BAF18(P *p, int id)
{
    int iTarget = p->current;
    int cur, j, k;

    if (id == 0x609) {
        iTarget = 0;
    } else if (id == 0x60A) {
        iTarget = p->eventCount;
    } else if (id != -1) {
        for (j = 0; j < p->eventCount; j++) {
            if (p->events[j].id == id) {
                iTarget = j;
                break;
            }
        }
    }
    cur = p->current;
    if (p->current == iTarget) return;
    if (p->current < iTarget) {
        do {
            E *pe;
            p->current = cur + 1;
            pe = &p->events[cur];
            for (k = 0; k < pe->actionCount; k++) {
                I *pi = &pe->actions[k];
                SO *ps = 0;
                if ((0xC3 >> pi->kind) & 1) {
                    ps = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 1, pi->value, p);
                    if (!ps) continue;
                }
                switch ((unsigned int)pi->kind) {
                case 0:
                    ((void (*)(void *))ps->vt[8].pfn)((char *)ps + ps->vt[8].d);
                    break;
                case 1:
                    ((void (*)(void *))ps->vt[9].pfn)((char *)ps + ps->vt[9].d);
                    break;
                case 2:
                    ShowSoGlobsById(p, pi->value);
                    break;
                case 3:
                    HideSoGlobsById(p, pi->value);
                    break;
                case 4:
                    ShowSoPartsById(p, pi->value);
                    break;
                case 5:
                    func_001F6B48(p, pi->value);
                    break;
                }
            }
            cur = p->current;
        } while (cur < iTarget);
        p->marker = 0;
    } else if (iTarget < p->current) {
        register int rewind __asm__("$6") = p->current;
        int fresh;
        do {
            E *pe;
            register int previous __asm__("$2") = rewind - 1;
            register int offset __asm__("$3");
            __asm__("sll %0,%2,%3\n\tsw %2,%1"
                    : "=&r"(offset), "=m"(p->current) : "r"(previous), "i"(EVENT_SHIFT));
            pe = (E *)((char *)p->events + offset);
            k = pe->actionCount;
            while (k-- > 0) {
                I *pi = &pe->actions[k];
                SO *ps = 0;
                if ((0xC3 >> pi->kind) & 1) {
                    ps = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 1, pi->value, p);
                    if (!ps) continue;
                }
                switch ((unsigned int)pi->kind) {
                case 0:
                    ((void (*)(void *))ps->vt[9].pfn)((char *)ps + ps->vt[9].d);
                    break;
                case 1:
                    ((void (*)(void *))ps->vt[8].pfn)((char *)ps + ps->vt[8].d);
                    break;
                case 2:
                    HideSoGlobsById(p, pi->value);
                    break;
                case 3:
                    ShowSoGlobsById(p, pi->value);
                    break;
                case 4:
                    func_001F6B48(p, pi->value);
                    break;
                case 5:
                    ShowSoPartsById(p, pi->value);
                    break;
                }
            }
            fresh = p->current;
            __asm__("daddu %0,%1,$zero" : "=r"(rewind) : "r"(fresh));
        } while (iTarget < fresh);
        p->marker = 0;
    } else {
        p->marker = 0;
    }
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char offset_0_marker[FIELD_OFFSET(P,marker)==344?1:-1];
typedef char offset_0_current[FIELD_OFFSET(P,current)==348?1:-1];
typedef char offset_0_eventCount[FIELD_OFFSET(P,eventCount)==352?1:-1];
typedef char offset_0_events[FIELD_OFFSET(P,events)==356?1:-1];
typedef char offset_1_id[FIELD_OFFSET(E,id)==0?1:-1];
typedef char offset_1_unknown4[FIELD_OFFSET(E,unknown4)==4?1:-1];
typedef char offset_1_actionCount[FIELD_OFFSET(E,actionCount)==8?1:-1];
typedef char offset_1_actions[FIELD_OFFSET(E,actions)==12?1:-1];
typedef char offset_2_kind[FIELD_OFFSET(I,kind)==0?1:-1];
typedef char offset_2_value[FIELD_OFFSET(I,value)==4?1:-1];
typedef char offset_3_vt[FIELD_OFFSET(SO,vt)==12?1:-1];
typedef char offset_4_d[FIELD_OFFSET(VTE,d)==0?1:-1];
typedef char offset_4_pfn[FIELD_OFFSET(VTE,pfn)==4?1:-1];
typedef char size_0[sizeof(P)==360?1:-1];
typedef char size_1[sizeof(E)==16?1:-1];
typedef char size_2[sizeof(I)==8?1:-1];
typedef char size_3[sizeof(SO)==16?1:-1];
typedef char size_4[sizeof(VTE)==8?1:-1];
typedef char size_5[sizeof(int)==4?1:-1];
typedef char size_6[sizeof(short)==2?1:-1];
typedef char size_7[sizeof(void*)==4?1:-1];
typedef char alignment_0[__alignof__(P)==4?1:-1];
typedef char alignment_1[__alignof__(E)==4?1:-1];
typedef char alignment_2[__alignof__(I)==4?1:-1];
typedef char alignment_3[__alignof__(SO)==4?1:-1];
typedef char alignment_4[__alignof__(VTE)==4?1:-1];
typedef char event_shift[sizeof(E)==(1<<EVENT_SHIFT)?1:-1];
