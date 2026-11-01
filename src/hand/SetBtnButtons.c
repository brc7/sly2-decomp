struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct P { char pad[0x90]; int state; float tState; };
void TriggerBtn(struct P *p);
void UntriggerBtn(struct P *p);
void SetBtnButtons(struct P *p, int state)
{
    int f1, f2; int old = p->state; if (old == state) return; f1 = old == 0 && state == 1; f2 = old != -1 && state == 0;
    p->state = state;
    p->tState = D_002DDED0.t;
    if (f1) TriggerBtn(p);
    if (f2) UntriggerBtn(p);
}
