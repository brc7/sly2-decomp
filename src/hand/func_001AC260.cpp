typedef unsigned int uint;
struct FRAME;extern "C" void func_001AE4A0(FRAME*,int);
struct FRAME {FRAME()__asm__("func_001AE478");~FRAME(){func_001AE4A0(this,2);}};
struct REQUEST {uint bytes,alignment;};
struct X;
extern "C" void func_00258248(X *px);
struct X { uint x0; uint x4; int x8; char pad[0x110 - 0xC]; X() { x8 = 2; func_00258248(this); } };
struct Y;extern "C" void func_00184950(Y*,int);
struct Y {int flags;unsigned char*cursor;int left,consumed;unsigned char*raw;int rawBytes,decompFlags,pending,word20;unsigned char*window;int word28,word2C;void*vt;int capacity;unsigned char*buffer;int handle3C,word40,word44;uint word48;int word4C,word50,word54,word58,word5C,word60;char tail64[0xC];Y(int cb,void*pv,int flags) __asm__("func_001848A8");~Y(){func_00184950(this,2);}};
struct K {uint x0,x4;char pad[0x34-8];uint x34;int pad2;uint x3C;};
extern "C" {
extern char D_002A6CB8[];
int func_0021ECD8(char *buf, const char *fmt, ...);
int func_001AC7A0(void *p, char *name, int mode, int *ph);
void *func_001AE318(REQUEST *pmsg);
int func_00184D60(Y *py, X *px);
int func_001AC8A8(void *p, int h, char *buf, int n, int *pn);
int func_00117898(Y *py, int cb, char *buf);
void CStreamBis_Close(Y *py);
int func_001AC808(void *p, int h);
int func_001AC260(void *p, const char *a, K *k, const char *b);
}
static inline int SetKey(X *px, K *k)
{
    if (!k)
        return 0;
    uint v;
    uint k34 = k->x34;
    uint k3C = k->x3C;
    px->x8 = 2;
    px->x0 = (k->x0 ^ k34) + 0xCA0;
    v = k->x4 ^ k3C;
    px->x4 = v;
    return v != 0;
}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
typedef char X_x0_Offset[FIELD_OFFSET(X,x0)==0?1:-1];
typedef char X_x4_Offset[FIELD_OFFSET(X,x4)==4?1:-1];
typedef char X_x8_Offset[FIELD_OFFSET(X,x8)==8?1:-1];
typedef char K_x0_Offset[FIELD_OFFSET(K,x0)==0?1:-1];
typedef char K_x4_Offset[FIELD_OFFSET(K,x4)==4?1:-1];
typedef char K_x34_Offset[FIELD_OFFSET(K,x34)==52?1:-1];
typedef char K_x3C_Offset[FIELD_OFFSET(K,x3C)==60?1:-1];
typedef char Y_flags_Offset[FIELD_OFFSET(Y,flags)==0?1:-1];
typedef char Y_cursor_Offset[FIELD_OFFSET(Y,cursor)==4?1:-1];
typedef char Y_left_Offset[FIELD_OFFSET(Y,left)==8?1:-1];
typedef char Y_consumed_Offset[FIELD_OFFSET(Y,consumed)==12?1:-1];
typedef char Y_raw_Offset[FIELD_OFFSET(Y,raw)==16?1:-1];
typedef char Y_rawBytes_Offset[FIELD_OFFSET(Y,rawBytes)==20?1:-1];
typedef char Y_decompFlags_Offset[FIELD_OFFSET(Y,decompFlags)==24?1:-1];
typedef char Y_pending_Offset[FIELD_OFFSET(Y,pending)==28?1:-1];
typedef char Y_word20_Offset[FIELD_OFFSET(Y,word20)==32?1:-1];
typedef char Y_window_Offset[FIELD_OFFSET(Y,window)==36?1:-1];
typedef char Y_word28_Offset[FIELD_OFFSET(Y,word28)==40?1:-1];
typedef char Y_word2C_Offset[FIELD_OFFSET(Y,word2C)==44?1:-1];
typedef char Y_vt_Offset[FIELD_OFFSET(Y,vt)==48?1:-1];
typedef char Y_capacity_Offset[FIELD_OFFSET(Y,capacity)==52?1:-1];
typedef char Y_buffer_Offset[FIELD_OFFSET(Y,buffer)==56?1:-1];
typedef char Y_handle3C_Offset[FIELD_OFFSET(Y,handle3C)==60?1:-1];
typedef char Y_word40_Offset[FIELD_OFFSET(Y,word40)==64?1:-1];
typedef char Y_word44_Offset[FIELD_OFFSET(Y,word44)==68?1:-1];
typedef char Y_word48_Offset[FIELD_OFFSET(Y,word48)==72?1:-1];
typedef char Y_word4C_Offset[FIELD_OFFSET(Y,word4C)==76?1:-1];
typedef char Y_word50_Offset[FIELD_OFFSET(Y,word50)==80?1:-1];
typedef char Y_word54_Offset[FIELD_OFFSET(Y,word54)==84?1:-1];
typedef char Y_word58_Offset[FIELD_OFFSET(Y,word58)==88?1:-1];
typedef char Y_word5C_Offset[FIELD_OFFSET(Y,word5C)==92?1:-1];
typedef char Y_word60_Offset[FIELD_OFFSET(Y,word60)==96?1:-1];
typedef char Y_tail64_Offset[FIELD_OFFSET(Y,tail64)==100?1:-1];
typedef char Size_0[sizeof(FRAME)==1?1:-1];
typedef char Size_1[sizeof(REQUEST)==8?1:-1];
typedef char Size_2[sizeof(X)==272?1:-1];
typedef char Size_3[sizeof(K)==64?1:-1];
typedef char Size_4[sizeof(Y)==112?1:-1];
typedef char Size_5[sizeof(uint)==4?1:-1];
typedef char Size_6[sizeof(int)==4?1:-1];
typedef char Size_7[sizeof(void*)==4?1:-1];
int func_001AC260(void *p, const char *a, K *k, const char *b)
{
    FRAME frame;
    char name[0x50];
    int ret;
    func_0021ECD8(name, D_002A6CB8, a, b);
    int h = -1;
    ret = func_001AC7A0(p, name, 0x202, &h);
    if (ret == 0) {
        X x;
        REQUEST msg;
        REQUEST *pmsg = &msg;
        pmsg->bytes = 0x403F;
        pmsg->alignment = 0x10;
        void *pv = func_001AE318(pmsg);
        Y y(0x403F, pv, 0);
        if (!SetKey(&x, k) || !func_00184D60(&y, &x))
            ret = 0x27;
        else {
            char buf[0x1000];
            int n;
            int out;
            while ((n = func_00117898(&y, 0x1000, buf)) != 0) {
                ret = func_001AC8A8(p, h, buf, n, &out);
                if (ret)
                    goto done;
                if (out != n) {
                    ret = 7;
                    goto done;
                }
            }
            ret = 0;
        }
    done:
        CStreamBis_Close(&y);
        int r2 = func_001AC808(p, h);
        if (!ret)
            ret = r2;
    }
    return ret;
}
