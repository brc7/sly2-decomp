// Render thread of the loading screen: waits for the start semaphore, then keeps drawing a pulsing
// (scale + opacity) quad every frame until the frame-end semaphore is signalled.
//
// Matching notes: the function must start directly with the outer `for(;;)` (no pointer locals such as
// `VIFS *builder = &D_002DEA28;` before it). Then the loop header is the first basic block, gcse has no
// pre-header to hoist the `lui`s of D_003D3A3C/D_002DEDBC into (they stay local, which gives the $v1 after the
// WaitSema call), and the inner loop keeps enough insns for loop.c to leave the `m_fCheckCnt = 1` constant alone.
#include "vifs.h"
#include "gifs.h"
struct SEMA {int count,maxCount,initialCount,waitThreads;uint attributes,option;};
struct M2 {float a[8];M2(){}void Identity();};
extern "C" void *func_002333B4(void *,int,int);
inline void M2::Identity(){func_002333B4(this,0,sizeof(*this));a[0]=a[3]=1.0f;}
struct CLOCK {float t,dt,ratio,realRatio,realDt,realT;char pad18[0x24];float frameT,frameDt;ulong tick;};
struct FR {int n0;void *p4;char pad8[0x1C];void *packet;char pad28[8];int flags;};
extern VIFS D_002DEA28;
extern GIFS D_002DEA00;
extern FR *D_002DEA4C;
extern CLOCK D_002DDED0;
extern float *D_002DD314;
extern int D_003D3A3C,D_003D3A40,D_003D3A44,D_002DEDBC,D_002DEB18;
extern uint D_002BE538;
extern int D_002C10A0,D_002C10A4,D_002C10A8,D_002C10AC;
extern float D_002C10B0,D_002C10B4,D_002C10C0[3],D_002BBB60[2];
extern QW D_002DF260[];
extern "C" int func_00234A60(int);
extern "C" int func_00234A40(int);
extern "C" int ReferSemaStatus(int,SEMA *);
extern "C" float func_00245000(float);
extern "C" void func_001C93E0(GIFS *,M2 *,float);
extern "C" void CloseFrame__Fv(void);
extern "C" void MarkClockTick(CLOCK *,int);
void OpenFrame(void);
static inline void Clamp(float *value,const float *bounds){float low=bounds[0],high=bounds[1];if(*value<low)*value=low;else if(high<*value)*value=high;}
static inline void BeginFrame()
{
    MarkClockTick(&D_002DDED0,1);
    OpenFrame();
    uchar *pb=D_002DEA28.m_pb;
    D_002DEA4C->packet=pb;
    D_002DEA28.m_fCheckCnt=1;
    D_002DEA28.AddDmaCnt();
    D_002DEA28.AddVifDirect(0x25,D_002DF260,0);
    D_002DEA28.EndDmaCnt();
}
/** @sly2-annot addr=0x1C9190
 * Purpose: thread: wait start sema, draw pulsing quad each frame until end sema
 * Info: proposed name LoadingScreenThread (T1); TU prog
 * Notes: runtime5b: 80x80 pulsing quad, 1 s fade-in; ticks the clock paused (S55->S56)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources: r5_1AE130+runtime
 * See: docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen,
 *   docs/engine/main-loop-startup.md#6-threads, docs/guides/port-plan.md#11-known-facts-that-drive-the-design
 * @end-sly2-annot */
extern "C" void LoadingScreenThread(void *)
{
    SEMA status;M2 matrix;
    for(;;){
        func_00234A60(D_003D3A3C);
        D_002DEDBC=0;
        for(;;){
            BeginFrame();
            ReferSemaStatus(D_003D3A40,&status);
            if(status.count!=0)break;
            float t=-*D_002DD314+D_002DDED0.frameT;
            float scale=1.0f+D_002C10B0*func_00245000(t*D_002C10B4);
            matrix.Identity();
            matrix.a[0]=D_002C10A0*scale;
            matrix.a[3]=D_002C10A4*scale;
            matrix.a[4]=D_002C10A8;matrix.a[5]=D_002C10AC;
            float alpha=D_002C10C0[0]+t*(D_002C10C0[1]+t*D_002C10C0[2]);
            Clamp(&alpha,D_002BBB60);
            func_001C93E0(&D_002DEA00,&matrix,alpha);
            CloseFrame__Fv();D_002BE538++;
        }
        do{D_002DEA4C->flags&=-2;}while(0);
        CloseFrame__Fv();D_002BE538++;
        do {ReferSemaStatus(D_002DEB18,&status);}while(status.count==0);
        func_00234A60(D_003D3A40);
        func_00234A40(D_003D3A44);
    }
}
