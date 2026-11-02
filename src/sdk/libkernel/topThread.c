// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char mode; unsigned char data; } TopReq;
typedef struct { int count; int pad; TopReq req[512]; } TopArg;
extern int D_00448940;
extern char D_002AA7B0[];
int func_00234A60(int);
int func_00234950(int);
int func_002348D0(int);
int func_00234990(int);
int func_002369E0(const char *, ...);
void topThread(TopArg *ta)
{
    int idx;
    while (1) {
        func_00234A60(D_00448940);
        idx = ta->count & 0x1FF;
        ta->count = idx + 1;
        switch (ta->req[idx].mode) {
        case 0:
            func_00234950(ta->req[idx].data);
            break;
        case 1:
            func_002348D0(ta->req[idx].data);
            break;
        case 2:
            func_00234990(ta->req[idx].data);
            break;
        default:
            func_002369E0(D_002AA7B0);
            break;
        }
    }
}
