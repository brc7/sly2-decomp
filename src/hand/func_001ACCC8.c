struct PMF { short delta; short index; void (*pfn)(void *); };
struct P { char pad[0x58]; int x58; float x5C; int pad2; int x64; };
struct S { char pad[0x10]; void *x10; char pad2[0x24 - 0x14]; struct P *x24; char pad3[0x40 - 0x28]; int x40; int x44; int pad4; int x4C; };
struct MC { char pad[8]; int x8; char pad2[0xFD58 - 0xC]; int xFD58; char pad3[0xFD6C - 0xFD5C]; int xFD6C; };
struct CLOCK { char pad[0x3C]; float t3c; };
extern struct MC D_002E65E0;
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
extern char D_003D9ED0[];
extern char D_002F633C[];
extern const struct PMF D_002A6D88;
int func_0020E040(void *g);
int func_001ACC80(struct S *s);
int GrfHandleJoyBtn(void *p, int n);
void func_001AD168(struct S *s);
void func_001ACFE0(struct S *s);
int func_001AB088(void *p);
void func_001AAF90(void *p, struct PMF pmf);
void func_001ACB58(struct S *s, int n);
void func_001ACBB8(struct S *s);
void func_001660F8(void);
void func_001AF660(void *p);
void func_001ACCC8(struct S *s)
{
    struct P *p = s->x24;
    int c;
    int r;
    if (!p)
        return;
    if (p->x58 != 2)
        return;
    c = p->x64;
    if (c != 2)
        return;
    if (func_0020E040(D_003DD490))
        return;
    if (D_002DDED0.t3c - s->x24->x5C <= 0.1f)
        return;
    r = func_001ACC80(s);
    if (r != 1) {
        if (r == c) {
            if (s->x10 && GrfHandleJoyBtn(s->x10, 0x80)) {
                func_001AD168(s);
                return;
            }
            if (s->x10 && GrfHandleJoyBtn(s->x10, 0x10)) {
                func_001ACFE0(s);
                return;
            }
        }
    } else {
        if (s->x10 && GrfHandleJoyBtn(s->x10, 0x80)) {
            func_001ACFE0(s);
            return;
        }
    }
    switch (s->x40) {
    case 0x20:
    case 0x22:
        if (s->x44 == 1)
            return;
        func_001ACBB8(s);
        return;
    case 0x11:
    case 0x12:
    case 0x15:
    case 0x18:
    case 0x2E:
        if (D_002E65E0.x8 != 6)
            return;
        func_001ACBB8(s);
        return;
    case 0x13:
        if (func_001AB088(D_002F633C))
            return;
        if (!D_002E65E0.xFD6C) {
            func_001AAF90(D_002F633C, D_002A6D88);
            func_001ACB58(s, 0x16);
        } else
            func_001ACB58(s, 0x14);
        return;
    case 0x16:
        if (func_001AB088(D_002F633C))
            return;
        if (!D_002E65E0.xFD6C) {
            func_001ACBB8(s);
            return;
        }
        func_001ACB58(s, 0x17);
        return;
    case 0x1A:
        if (func_001AB088(D_002F633C))
            return;
        if (!D_002E65E0.xFD6C) {
            func_001ACBB8(s);
            switch (s->x4C) {
            case 1:
                func_001660F8();
                return;
            case 2:
                func_001AF660(D_003D9ED0);
                return;
            }
            return;
        }
        if (s->x4C == 3)
            func_001ACB58(s, 0x23);
        else
            func_001ACB58(s, 0x1D);
        return;
    case 0x21:
        if (func_001AB088(D_002F633C))
            return;
        D_002E65E0.xFD58 = 0;
        if (!D_002E65E0.xFD6C) {
            func_001ACBB8(s);
            return;
        }
        func_001ACB58(s, 0x1D);
        return;
    }
}
