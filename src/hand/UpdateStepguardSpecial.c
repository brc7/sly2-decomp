extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3D40[3];
struct S { char pad[0xFBC]; int k; float t; };
void SetStepguardSpecial(struct S *p, int n);
void UpdateStepguardSpecial(struct S *p)
{
    switch (p->k) {
    case 8:
    case 13:
        if (D_002DDED0.t - p->t > D_002C3D40[0])
            SetStepguardSpecial(p, -1);
        break;
    case 34:
    case 37:
        if (D_002DDED0.t - p->t > D_002C3D40[1])
            SetStepguardSpecial(p, -1);
        break;
    case 21:
        if (D_002DDED0.t - p->t > D_002C3D40[2])
            SetStepguardSpecial(p, -1);
        break;
    }
}
