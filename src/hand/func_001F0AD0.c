struct E { int pad[5]; float g; };
float GRandInRange__Fff(float, float);
struct E *func_001F0AD0(void *p, int n, struct E **ape)
{
    float sum = 0.0f;

    int i;
    for (i = 0; i < n; i++) sum += ape[i]->g;
    sum = GRandInRange__Fff(0.0f, sum);
    for (i = 0; i < n; i++) {
        sum -= ape[i]->g;
        if (sum <= 0.0f) return ape[i];
    }
    return 0;
}
