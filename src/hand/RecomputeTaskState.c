struct Y { char pad[0x54]; int n; };
struct X { char pad[0x54]; int n; char pad2[0x94 - 0x58]; int c; struct Y **apy; char pad3[0xF0 - 0x9C]; int fF0; };
void SetTaskStatePropagate(struct X *p, int n);
void RecomputeTaskState(struct X *p, int n)
{
    int nNew; int nState;
    if (p->fF0 != -1) return;
    nState = p->n; nNew = nState;
    if (n >= 0) {
        if (n < 2) {
            nNew = 0;
        } else if (n < 4) {
            int f = 1;
            int i;
            for (i = 0; i < p->c; i++) {
                if (p->apy[i]->n < 2) {
                    f = 0;
                    break;
                }
            }
            if (!f) {
                nNew = 0;
            } else {
                switch (p->n) {
                case 0: nNew = 1; break;
                case 1: case 2: break;
                case 3: nNew = n; break;
                }
            }
        }
    }
    SetTaskStatePropagate(p, nNew);
}
