typedef struct {
    char pad[0x18];
    float g;
    char pad1c[0x30 - 0x1C];
    unsigned int fl;
} SUB;
typedef struct {
    char pad[0x20];
    void *x20;
    unsigned int fl;
    int x28;
} ITEM;
typedef struct {
    char pad[0x150];
    SUB *psub;
    char pad154[0x4F8 - 0x154];
    int c;
    ITEM *a[8];
    float x51C;
    char pad520[0x540 - 0x520];
    float x540;
    float x544;
    int pad548;
    float x54C;
    float x550;
} OBJ;
int func_001D4E08(ITEM *);
void func_001F0660(void *, int);
int GrfConsumePressed__5POCTLUif(SUB *, int, float);
void func_001D2150(OBJ *p)
{
    int i;
    SUB *ps = p->psub;
    float g = ps->g;
    p->x544 = g;
    if (ps->fl & 0x40) {
        p->x550 = p->x51C;
        p->x54C = p->x540;
    } else {
        p->x54C = g;
        p->x550 = 0.0f;
    }
    for (i = 0; i < p->c; i++) {
        ITEM *q = p->a[i];
        if (func_001D4E08(q)) {
            if (p->psub->fl & q->fl)
                func_001F0660(q->x20, 0x67);
            else if (GrfConsumePressed__5POCTLUif(p->psub, q->x28, 0.0f))
                func_001F0660(q->x20, 0x68);
        }
    }
}
