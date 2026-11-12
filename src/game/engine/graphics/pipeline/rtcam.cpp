// rtcam.cpp -- the two render targets (D_003E1A78[2]): a 128x128 GS texture each, rendered from a camera object
// (RTCAM, an XFM with a field of view, near/far planes and the objects it renders); the GIF packets that clear the
// target and bind its texture, the camera's projection and clip planes for VU1, and the camera object's own functions
// (claiming a free target, hiding/showing its objects around the render). EE/VU0 asm with register pins.
// TU 0x1BE398..0x1BF110 (16 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_rtcam
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_rtcam
#include "gifs.h"
#include "vifs.h"
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
#include "sly2/stepplayer.h"
#include "sly2/shd.h"
#include "sly2/gs.h"

// 0x1BEFB8 StaticInit_rtcam and GlobalConstructors_rtcam: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

union U{qword q;float f[4];int i[4];};
struct M3{qword r[3];};
struct M4{U r[4];};
struct V3{float f[3];};
struct PLANE{V3 normal;float distance;};


#include "sly2/rtcam.h"   /* RT, RTO, RTCAM (shared with glob.cpp BuildModelDrawPackets) */































struct MATRIX4;
struct SW; struct LO; struct BASIC;
/* The hero: a PO; a STEPPLAYER (cid 0x34) hands on to the object it controls (stepplayer.h jtmode.pstepOther, +0xF40). */
typedef PO POV;

/* The two render targets' lookup rows (26 ints each, -1: none), indexed by GetRenderTargetTexture (rtcam's .data). */
int D_002C0B28[26] = {-1, -1, -1, 15, 15, 16, 16, -1, -1, 17, -1, -1, -1, -1, -1, 15, 16, 17, 15, 16, 17, -1, 15, 16, 17, -1};
int D_002C0B90[26] = {-1, -1, -1, 18, 18, 19, 19, -1, -1, 20, -1, -1, -1, -1, -1, 18, 19, 20, 18, 19, 20, -1, 18, 19, 20, -1};
int *D_002C0BF8[2] = {D_002C0B28, D_002C0B90};
extern "C" {
extern V3 D_002C1F00,D_002C1F10;
M3*GetLoWorldMatrix(RTCAM*);
qword GetLoWorldPos(RTCAM*);
qword*VECTOR_opAssign(qword*,qword);
float tanf(float);
void CombineEyeLookAtProj(M4*,M4*,M4*);
void BeginRpGlob(VIFS *pvifs, int, int);
void SetupRenderTargetView(RT *prt, VIFS *pvifs);
RT *AcquireRenderTarget(RTCAM *pcam);
void InitXfm(RTCAM *pcam);
void LoadXfmFromBrx(RTCAM *pcam, void *pbis);
int IsLoInWorld(LO *plo);
void BuildModelDrawPackets(void *pv, RTCAM *pcam);
POV *GetHeroPo(SW *psw);
}
int IgsAllocGsb(GSB *pgsb, int cb);
uint CeilLog2(uint n);
void BuildSimpleProjectionMatrix(float, float, float, float, float, float, MATRIX4 *pmat);
void AppendDlEntry(void *pdl, void *pv) __asm__("AppendDlEntry__FP2DLPv");
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);
LO *FindSwObject(SW *psw, int grf, OID oid, LO *ploCtx);

// Pack the vertex (x, y) in 12.4 fixed point with z = 0; returns x.
static inline int PackNext(GIFS &gifs, int x, int y)
{
    gifs.PackXYZ(x << 4, y << 4, 0);
    return x;
}
// Level setup of the two render targets (D_003E1A78, D_003E1AA0): allocates each one's 128x128 texture from GS memory
// and builds its packets (clear the target, bind it as a texture with TEX0/CLUT). (inferred)
extern "C" void InitRenderTargets(RT *pt, GSB *px)
{
    register int cbBlock __asm__("$17") = 0x40;
    register int negative __asm__("$19") = -1;
    register int divisor __asm__("$2") = 0x40;
    pt->dx = 0x80;
    pt->dy = 0x80;
    int tbw = pt->dx / divisor;
    int tbp = IgsAllocGsb(px, 0x100);
    pt->tex0.TBP0 = tbp;
    pt->tex0.TBW = tbw;
    pt->tex0.PSM = 0;
    tbp >>= 5;
    pt->tex0.TW = CeilLog2(pt->dx);
    pt->tex0.TH = CeilLog2(pt->dy);
    pt->tex0.TCC = 1;
    int zbp = (px->limit - 0x100) >> 5;
    GIFS gifs;
    int xOff = 0x800 - pt->dx / 2;
    int yOff = 0x800 - pt->dy / 2;
    QW aqw[0x40];
    gifs.AllocStatic(0x40, aqw);
    gifs.AddPrimPack(0, 1, 0xE);
    int w = pt->dx;
    register int m __asm__("$2");
    __asm__("daddu %0,%1,$zero" : "=r"(m) : "r"(cbBlock));
    cbBlock = cbBlock < w;
    if (cbBlock) m = w;
    negative = negative < m;
    int pages = m + 63;
    if (negative) pages = m;
    gifs.PackAD(0x4C, tbp | ((ulong)(pages >> 6) << 16));
    gifs.PackAD(0x4E, zbp | 0x30000000);
    gifs.PackAD(0x18, (xOff << 4) | ((ulong)(yOff << 4) << 32));
    gifs.PackAD(0x40, ((ulong)(pt->dx - 1) << 16) | ((ulong)(pt->dy - 1) << 48));
    gifs.PackAD(0x47, 0x30000);
    gifs.PackAD(1, 0);
    gifs.AddPrimPack(6, 1, 5);
    int x = xOff;
    while (x < xOff + pt->dx) {
        gifs.PackXYZ(x << 4, yOff << 4, 0);
        x = PackNext(gifs, x + 0x20, yOff + pt->dy);
    }
    gifs.AddPrimEnd();
    gifs.DetachCopySw(&pt->cqwClear, 0, &pt->aqwClear);
    gifs.AllocStatic(0x40, aqw);
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(6, pt->tex0.word);
    gifs.PackAD(0x14, 0x60);
    gifs.PackAD(8, 5);
    gifs.AddPrimEnd();
    gifs.DetachCopySw(&pt->cqwTex, 0, &pt->aqwTex);
}

// Broadcast a float into vf2.
static inline qword Bcast(float f){register qword q __asm__("vf2");q = VuFromFloat<qword>(f);return q;}
// Return v scaled by k.x, in vf4.
static inline qword Scale0(qword v,qword k){register qword q __asm__("vf4");__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(q):"j"(v),"j"(k));return q;}
// Return v scaled by k.x, in vf1.
static inline qword Scale2(qword v,qword k){register qword q __asm__("vf1");__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(q):"j"(v),"j"(k));return q;}
// Starts rendering into a target: builds the look-at and projection of the target's camera object (RTCAM) and emits
// the VIF setup (BASE/OFFSET, the camera constants for VU1). Called by the BeginRpRenderTarget0/1 pass hooks. (inferred)
extern "C" void SetupRenderTargetView(RT*p,VIFS*vifs){
 U axis0,axis1,axis2,minus2,minus0,copy1;
 M4 view;
 U scalar;
 M4 projection,product;
 PLANE planes[2];
 RTCAM*object;
 M3*basis;
 register float aspect __asm__("$f20");
 float slope;
 if(!p->camera)return;
 object=p->camera->parent?p->camera->parent:p->camera;
 basis=GetLoWorldMatrix(object);
 M4*pv=&view;
 axis0.q=basis->r[0];
 {qword neg=Bcast(-1.0f);
  axis1.q=basis->r[1];
  axis2.q=basis->r[2];
  qword nx=Scale0(axis0.q,neg);
  qword nz=Scale2(axis2.q,neg);
  copy1=axis1;
  scalar.q=neg;
  minus2.q=nz;
  minus0.q=nx;
 }
 {qword position=GetLoWorldPos(object);
 VECTOR_opAssign(&view.r[3].q,position);}
 aspect=(float)p->dx;
 aspect/=(float)p->dy;
 pv->r[0]=minus2;
 pv->r[1]=minus0;
 pv->r[2]=copy1;
 view.r[0].i[3]=view.r[1].i[3]=view.r[2].i[3]=0;
 pv->r[3].f[3]=1.0f;
 slope=tanf(p->camera->fov*0.5f);
 aspect=slope*aspect;
 slope*=4096.0f;
 aspect*=4096.0f;
 BuildSimpleProjectionMatrix((float)p->dx/aspect,(float)p->dy/slope,0.0f,0.0f,p->camera->clipNear,p->camera->clipFar,(MATRIX4*)&projection);
 CombineEyeLookAtProj(pv,&projection,&product);
 {RTCAM*camera=p->camera;
  float z=(camera->clipNear+camera->clipFar)/(camera->clipNear-camera->clipFar);
  float k=1.0f-z;
  planes[0].normal=D_002C1F00;
  planes[0].distance=1.0f/(camera->clipNear*k);
  planes[1].normal=D_002C1F10;
  planes[1].distance=-z/(camera->clipNear*k);
 }
 vifs->AddDmaCnt();
 vifs->AddVifFlush();
 vifs->AddVifDirect(p->cqwClear,p->aqwClear,0);
 vifs->AddVifFlusha();
 vifs->AddVifBaseOffset(0,0);
 vifs->AddVifUnpack(UPK_V4_32,4,&product,2);
 vifs->AddVifUnpack(UPK_V4_32,4,pv,6);
 vifs->AddVifUnpack(UPK_V4_32,2,planes,0);
 vifs->AddVifBaseOffset(0x32,0x1E7);
 vifs->EndDmaCnt();
}

// Pass 8 begin (kind 15): set up render-to-texture slot 0 (target packet + slot camera) then BeginRpGlob. (inferred)
// Also known as: RenderRenderTargets (upstream readable / Sly 1 name; RenderRenderTargets__FP5SCENEP10RENDEROPTSi).
extern "C" void BeginRpRenderTarget0(VIFS *pvifs, int a1, int a2)
{
    SetupRenderTargetView(&((RT *)g_sw.artgt)[0], pvifs);
    BeginRpGlob(pvifs, a1, a2);
}

// Pass 9 begin (kind 18): same for slot 1. (inferred)
extern "C" void BeginRpRenderTarget1(VIFS *pvifs, int a1, int a2)
{
    SetupRenderTargetView(&((RT *)g_sw.artgt)[1], pvifs);
    BeginRpGlob(pvifs, a1, a2);
}

// Initializes a render-target camera (RTCAM, class cid 0x2D, vtable slot 7): XFM init, field of view 1 rad, near 4,
// far 1000. (inferred)
void CID45::Init()
{
    RTCAM *pcam = (RTCAM *)this;
    InitXfm(pcam);
    pcam->fov = 1.0f;
    pcam->clipNear = 4.0f;
    pcam->clipFar = 1000.0f;
}

// Loads an RTCAM from the level file (vtable slot 14): just its transform.
extern "C" void LoadRtcamFromBrx(RTCAM *pcam, void *pbis)
{
    LoadXfmFromBrx(pcam, pbis);
}

// Adds an object to the set the camera renders into its target. (inferred)
extern "C" void AddRtcamObject(RTCAM *rtcam, LO *x)
{
    if (x) rtcam->ap[rtcam->c++] = x;
}

// Adds an object to the camera's set and records how to restore it after the render (flags), calling its vtable slot 9
// (inferred: hides it from the normal scene while it is shown in the target).
extern "C" void AddRtcamObjectShown(RTCAM *rtcam, RTO *po)
{
    if (po) {
        rtcam->shown[rtcam->numShown++] = po;
        po->unknown160 = 1;
        po->unknown15C = 1;
        po->Remove();
    }
}

// Claims a free render target for the camera (returns it, or 0 if both are taken) and prepares the camera's objects
// for it. The HUD calls this to show a 3D object (found with FindObjectForRenderTarget) as a texture. (inferred)
// Also known as: TakeRtgt (upstream readable / Sly 1 name; TakeRtgt__FPv).
extern "C" RT *AcquireRenderTarget(RTCAM *rtcam)
{
    unsigned int i;
    int j;
    if (rtcam->prt) return rtcam->prt;
    for (i = 0; i < 2; i++) {
        RT *pr = &((RT *)g_sw.artgt)[i];
        if (!pr->camera) {
            rtcam->prt = pr;
            break;
        }
    }
    if (rtcam->prt) {
        rtcam->prt->camera = rtcam;
        for (j = 0; j < rtcam->c; j++) {
            LO *o = rtcam->ap[j];
            sptr x;
            rtcam->af[j] = 0;
            if (((int)o->isInWorld & 1) == 0) {   /* `== 0`, not `!`: see the merge guide (C++ bool) */
                rtcam->af[j] = 1;
                o->Add();
                if (!IsLoInWorld(o)) {
                    LO *q;
                    AppendDlEntry(&g_sw.busyRoots, o->root);
                    q = o->root->topObject;
                    q->OnAdd();
                }
            }
            x = (sptr)o->ploupd;
            if ((x ^ 1) & 1) x = ((LOUPD *)x)->grf;
            if (x & 8) {
                rtcam->af[j] |= 2;
                o->SetFlag8(0);
            }
        }
        rtcam->SetFlag8(1);
        rtcam->HandleSpliceEvent(2, 0, 0, 0, 0);
    }
    return rtcam->prt;
}

// AcquireRenderTarget without the result (the form used as a callback). (inferred)
extern "C" void AcquireRenderTargetNoResult(RTCAM *pcam)
{
    AcquireRenderTarget(pcam);
}

// Gives the camera's render target back: restores its objects (vtable slots 9 / 21 by the recorded flags), then
// notifies itself (slot 21 with 0, message 3). (inferred)
// Also known as: GiveRtgt (upstream readable / Sly 1 name; GiveRtgt__FPv).
extern "C" void ReleaseRenderTarget(RTCAM *rtcam)
{
    int i;
    if (!rtcam->prt) return;
    rtcam->prt->camera = 0;
    rtcam->prt = 0;
    for (i = 0; i < rtcam->c; i++) {
        LO *o = rtcam->ap[i];
        if (rtcam->af[i] & 1) o->Remove();
        if (rtcam->af[i] & 2) o->SetFlag8(1);
    }
    rtcam->SetFlag8(0);
    rtcam->HandleSpliceEvent(3, 0, 0, 0, 0);
}

// Returns texture n of the camera's render target (table D_002C0BF8[target][n]), or -1 when it holds none; used by
// the glob renderer to bind the target texture. (inferred)
extern "C" int GetRenderTargetTexture(RTCAM *rtcam, int n)
{
    if (!rtcam->prt) return -1;
    return D_002C0BF8[rtcam->prt - (RT *)g_sw.artgt][n];
}

// Renders the camera (vtable slot 19): when it holds a target, draws each of its objects' globsets into it. (inferred)
extern "C" void RenderRtcam(RTCAM *rtcam)
{
    int i;
    if (rtcam->prt) {
        for (i = 0; i < rtcam->c; i++) BuildModelDrawPackets(&((SO *)rtcam->ap[i])->model, rtcam);
    }
}

// The object a STEPPLAYER hero (cid 0x34) controls, otherwise po itself.
static __inline__ POV *Get(POV *po) { return (po && IsBasicDerivedFrom((BASIC *)po, (CID)0x34)) ? (POV *)((STEPPLAYER *)po)->jtmode.pstepOther : po; }
// Finds the object to show in a render target by id: ids 0x2AE and 0x2AF are searched relative to the hero (the
// player object, or what it controls), others in the whole world. (inferred)
// Also known as: PvRtgtOwnerOfId (upstream readable / Sly 1 name; PvRtgtOwnerOfId__Fi).
extern "C" void FindObjectForRenderTarget(int id)
{
    switch (id) {
    case 0x2AE:{
        POV*object=Get(GetHeroPo(&g_sw));
        (void)FindSwObject(&g_sw,1,(OID)id,(LO *)object);
        break;
    }
    case 0x2AF:
        (void)FindSwObject(&g_sw, 1, (OID)0x2AF, (LO *)GetHeroPo(&g_sw));
        break;
    default:
        (void)FindSwObject(&g_sw, 0x405, (OID)id, 0);
        break;
    }
}
