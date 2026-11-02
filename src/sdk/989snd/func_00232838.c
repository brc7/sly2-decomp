// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct CB { void (*fn)(int, long); int pad; long arg; } CB;
extern int D_002C4640, D_002C4680, D_002C4684, D_002C46B8;
extern int *D_002C4660[2];
extern int D_002C4668[2];
extern CB *D_002C4670[2];
extern int D_00443840[];
extern char D_00443800[];
extern char D_002AA1C8[], D_002AA5E8[], D_002AA610[], D_002AA670[];
void func_0021EC18(const char *, ...);
void *func_0023320C(void *, void *, int);
int func_00231C48(void);
void func_00234CA0(int);
void func_00231F38(void *, int);
int func_00237488(void *);
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *);
void func_00232BB8(void);

void func_00232838(int cmd, int size, unsigned char *buf, void (*fn)(int, long), long arg)
{
    int len;
    int tries;
    int held = 0;
    int i;
    unsigned char *p;

    if (!D_002C4684 && !D_002C4640 && !size && !fn) {
        func_00231F38(D_00443840, 1);
        while (func_00237488(D_00443800)) {
            if (!D_002C46B8)
                func_0021EC18(D_002AA1C8);
            func_00231C48();
            func_00234CA0(0);
        }
        func_00237288(D_00443800, cmd, 1, 0, 0, D_00443840, 0xC, 0, 0);
        return;
    }
    tries = 0;
    len = size + 4;
    if (len % 4)
        len += 4 - len % 4;
    if (len > 0x200) {
        func_0021EC18(D_002AA5E8);
        while (1)
            ;
    }
    while (*D_002C4660[D_002C4680] == 0x100 || D_002C4668[D_002C4680] < len) {
        if (D_002C4684) {
            D_002C4684 = 0;
            held = 1;
        }
        func_00231C48();
        if (tries == 1 && !D_002C46B8)
            func_0021EC18(D_002AA610, D_002C4680, *D_002C4660[D_002C4680]);
        tries++;
    }
    if (tries && !D_002C46B8)
        func_0021EC18(D_002AA670, tries);
    if (held)
        D_002C4684 = 1;
    p = (unsigned char *)D_002C4660[D_002C4680] - (D_002C4668[D_002C4680] - 0x1000);
    *(short *)p = cmd;
    p += 2;
    *(short *)p = size;
    p += 2;
    if (cmd != 0x68) {
        for (i = 0; i < size; i++)
            p[i] = buf[i];
    } else {
        for (i = 0; i < 0xC; i++)
            p[i] = buf[i];
        func_0023320C(p + 0xC, *(void **)(buf + 0xC), *(int *)(buf + 8));
    }
    D_002C4668[D_002C4680] -= len;
    D_002C4670[D_002C4680][*D_002C4660[D_002C4680]].fn = fn;
    D_002C4670[D_002C4680][*D_002C4660[D_002C4680]].arg = arg;
    func_00232BB8();
}
