// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef int qword __attribute__((mode(TI)));
typedef struct { void (*func)(void *, void *); void *data; void *gp; } CmdHandler;
typedef struct { unsigned char size; unsigned char dsize; unsigned short pad; void *dest; int fid; int opt; } CmdHdr;
typedef struct {
    CmdHdr *pktbuf;
    int x4, x8;
    CmdHandler *sys;
    int nsys;
    CmdHandler *usr;
    int nusr;
} CmdData;
extern CmdData D_0044EDD8;
void func_00234E10(void);
static inline void *SetGP(void *gp)
{
    void *old;
    __asm__ volatile("move %0, $gp\n\tmove $gp, %1" : "=&r"(old) : "r"(gp) : "$28", "memory");
    return old;
}
static inline void *SetHandlerGP(void *const *home)
{
    void *old;
    __asm__ volatile("lw $3,%1\n\tmove %0,$gp\n\tmove $gp,$3" : "=&r"(old) : "m"(*home) : "$3", "$28", "memory");
    return old;
}
#define FID (((CmdHdr *)buf)->fid)
int _sceSifCmdIntrHdlr(void)
{
    qword buf[7];
    CmdData *cd = &D_0044EDD8;
    register CmdHdr *pkt asm("$7") = cd->pktbuf;
    int size = *(volatile unsigned char *)&pkt->size;
    void (*func)(void *, void *);
    int id;
    void *old;
    register qword *src asm("$6");
    register int i asm("$4");

    if (size == 0)
        return 0;
    size = (size + 15) / 16;
    pkt->size = 0;
    src = (qword *)pkt;
    i = size;
    if (size > 0) {
        qword *dest = buf;
        do { *dest++ = *src++; } while (--i);
    }
    func_00234E10();
    {
        register int firstId asm("$3") = FID;
        if (firstId < 0) {
            register int rawId asm("$2"),count asm("$4");
            register unsigned mask asm("$3");
            __asm__("lw %0,%3\n\tli %1,%5\n\tlw %2,%4"
                :"=&r"(rawId),"=&r"(mask),"=r"(count)
                :"m"(((CmdHdr *)buf)->fid),"m"(cd->nsys),"i"(0x7FFFFFFF));
            id = rawId & mask;
            if (id < count) {
                func = cd->sys[id].func;
                if (func) {
                    old = SetHandlerGP(&cd->sys[id].gp);
                    func(buf, cd->sys[id].data);
                    SetGP(old);
                }
            }
        } else {
            register int rawId asm("$5"),count asm("$2");
            __asm__("lw %0,%2\n\tlw %1,%3"
                :"=&r"(rawId),"=r"(count)
                :"m"(((CmdHdr *)buf)->fid),"m"(cd->nusr));
            id = rawId;
            if (id < count) {
                func = cd->usr[id].func;
                if (func) {
                    old = SetHandlerGP(&cd->usr[id].gp);
                    func(buf, cd->usr[id].data);
                    SetGP(old);
                }
            }
        }
    }
    __asm__ volatile("sync\n\tei" : : : "memory");
    return 0;
}

#define FIELD_OFFSET(T,m) ((unsigned int)&(((T*)0)->m))
typedef char Cmd_Pointer_Width[sizeof(void*)==4?1:-1];
typedef char Cmd_Int_Width[sizeof(int)==4?1:-1];
typedef char Cmd_Short_Width[sizeof(unsigned short)==2?1:-1];
typedef char Cmd_Byte_Width[sizeof(unsigned char)==1?1:-1];
typedef char Cmd_Qword_Width[sizeof(qword)==16?1:-1];
typedef char Cmd_Qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char Cmd_Handler_Size[sizeof(CmdHandler)==12?1:-1];
typedef char Cmd_Handler_Function[FIELD_OFFSET(CmdHandler,func)==0?1:-1];
typedef char Cmd_Handler_Data[FIELD_OFFSET(CmdHandler,data)==4?1:-1];
typedef char Cmd_Handler_GP[FIELD_OFFSET(CmdHandler,gp)==8?1:-1];
typedef char Cmd_Header_Size[sizeof(CmdHdr)==16?1:-1];
typedef char Cmd_Header_Byte[FIELD_OFFSET(CmdHdr,size)==0?1:-1];
typedef char Cmd_Header_FID[FIELD_OFFSET(CmdHdr,fid)==8?1:-1];
typedef char Cmd_Data_Size[sizeof(CmdData)==28?1:-1];
typedef char Cmd_Data_Packet[FIELD_OFFSET(CmdData,pktbuf)==0?1:-1];
typedef char Cmd_Data_System[FIELD_OFFSET(CmdData,sys)==12?1:-1];
typedef char Cmd_Data_System_Count[FIELD_OFFSET(CmdData,nsys)==16?1:-1];
typedef char Cmd_Data_User[FIELD_OFFSET(CmdData,usr)==20?1:-1];
typedef char Cmd_Data_User_Count[FIELD_OFFSET(CmdData,nusr)==24?1:-1];
typedef char Cmd_Copy_Bytes[sizeof(qword[7])==112?1:-1];
