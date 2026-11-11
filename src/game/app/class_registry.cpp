// class_registry.cpp -- the crt0_splicewrap translation unit (0x100218..0x1147D8), one object as in the original:
// the crt0 fragments, the Splice binding thunks (64 hand-written, 373 generated wrappers from src/generated/splicewrap
// included at their addresses), the class registry's factories (New<Class>, the descriptor's pfnNew, one per
// constructible class) and descriptor getters (Get<Class>ClassDesc, vtable slot 6 GetClassInfo, one per class), and
// the generated static initializer, in address order (825 functions), plus three out-of-line constructors homed here
// from the linkonce area. Its .rodata (the thunks' and wrappers' argument-type arrays) is in function order, as in
// the ELF. See docs/guides/tu-merging.md "class_registry", docs/systems/object-model.md section 2 and
// section 1 (the class tree). The file keeps its old name (the registry part came first); renaming it to
// crt0_splicewrap.cpp is a mechanical follow-up.
// Entry points: the factories and descriptor getters (reached through the class descriptor table, one per class
// id), the Splice thunks and wrappers (reached through the option table g_aeopid when a script reads or sets an
// object option), and the crt0 fragments _start relies on. The factories' and getters' names (New<Class>,
// Get<Class>ClassDesc) and the generated wrappers' bodies are kept as generated (thunks: RefThunk<Class><OPTION> or
// RefThunk_<Native>, naming-workflow.md section 1); only the hand-written functions below carry doc comments.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_crt0_splicewrap
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_crt0_splicewrap
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31 -fno-exceptions -fno-rtti -fpermissive
#include "sly2/slotheap.h"
#include "sly2/basic.h"
#include "sly2/classes.h"
#include "sly2/wr.h"
#include "sly2/sw.h"
#include "sly2/cref.h"

// 0x114680 StaticInit_crt0_splicewrap and GlobalConstructors_crt0_splicewrap: g++-generated for the six engine
// slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

// ---- Splice thunks (hand-written) ----
// Each unpacks its Splice arguments with RefSetArgsFromSplice and calls the native function; the generated
// wrappers of the same block do the same in C. 64 are hand-written: the 33 option thunks that were in this file
// before the merge, and the 31 that had their own files in src/game/app/splice_thunks/, now verbatim in a namespace
// T_<name> each (for their own prototypes) over sly2/cref.h's CRef (an object result through SetBasic, a vector one
// through CRef_SetVector_VU_VECTOR). Their per-file CRef declarations named four members differently; the
// references are now SetTag__4CRef4TAGK, CRef_SetBasic, CRef_SetBool and SetS32__4CRefi (were InitTag__4CRefi,
// __as__4CRefPv, __as__4CRefb, __as__4CRefi, which no config file names), the same addresses.

/* The thunks use sly2/cref.h's CRef; the vector result goes through the C-named setter. */
extern "C" CRef *CRef_SetVector_VU_VECTOR(CRef *pref, qword vec);
CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, ...) __asm__("RefSetArgsFromSplice__FiP4CRefP4OTYPe");

// ---- The generated Splice wrappers (src/generated/splicewrap/*.inc, tools/gen_splicewrap.py output) ----
// Each wrapper is C as the generator wrote it (CRef as a plain struct, constructor/destructor calls spelled out),
// included below at its address inside its own namespace SPLICEWRAP::W_<name> with C linkage: its symbol and those it
// references keep their C names, and the per-wrapper prototypes of the native functions (which differ between
// wrappers) do not meet. -fpermissive (SLY2_CFLAGS above) lets g++ accept the C idioms the drafts use (void * passed
// for an int, arithmetic on void *) with a warning; it changes diagnostics only. m2c_prelude.h's types are repeated
// in SPLICEWRAP because its m2c_macros.h is not on the C++ include path.
#define M2C_PRELUDE_H
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
namespace SPLICEWRAP {
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long s64;
typedef unsigned long u64;
typedef int s128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));
typedef float f32;
typedef double f64;
typedef s32 M2C_UNK;
extern "C" {
#include "splicewrap.h"
}
} // namespace SPLICEWRAP
#define SLY2_SPLICEWRAP_BEGIN(name) namespace SPLICEWRAP { namespace W_##name { extern "C" {
#define SLY2_SPLICEWRAP_END } } }

// ==== The block in address order, 0x100218..0x110AB0: crt0 fragments, thunks and wrappers ====

// ---- crt0 fragments (hand-written assembly in the original crt0, like _start) ----

/* 0x100218: after _start. */
// crt0's _exit: jumps to the kernel Exit syscall wrapper (0x2398D0); libc exit() ends here. Never returns.
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".text\n"
    ".globl _exit\n"
    ".ent _exit\n"
    "_exit:\n"
    "j func_002398D0\n"
    "nop\n"
    ".end _exit\n"
    ".set at\n"
    ".set reorder\n"
);

/* 0x100220: ExitThread syscall tail. */
// crt0's thread root (the entry SetupThread is given in _start): calls the ExitThread syscall (0x23).
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".text\n"
    ".globl _root\n"
    ".ent _root\n"
    "_root:\n"
    "addiu $3, $0, 0x23\n"
    "syscall 0\n"
    ".end _root\n"
    ".set at\n"
    ".set reorder\n"
);

// ---- splice thunk RefThunkBasicDERIVED_FROMQ (was src/game/app/splice_thunks/RefThunkBasicDERIVED_FROMQ.cpp) ----
namespace T_RefThunkBasicDERIVED_FROMQ {

CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, int *an) __asm__("RefSetArgsFromSplice__FiP4CRefP4OTYPe");
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
CRef RefThunkBasicDERIVED_FROMQ(void *p, int c, void *aref) __asm__("RefThunkBasicDERIVED_FROMQ");

// Splice binding for option 3 on BASIC (g_aeopid[2], thunkFn): unpacks the script's 1 argument (Cid) with
// RefSetArgsFromSplice, calls IsBasicDerivedFrom and returns a bool to the script.
CRef RefThunkBasicDERIVED_FROMQ(void *object, int argCount, void *args)
{
    int argType = 7;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, args, &argType, &n);
    ref = IsBasicDerivedFrom__FP5BASIC3CID(object, n) != 0;
    return ref;
}
} // namespace T_RefThunkBasicDERIVED_FROMQ

SLY2_SPLICEWRAP_BEGIN(RefThunkBasicCUSTOM_BOOL_PROPERTY)
#include "../../generated/splicewrap/RefThunkBasicCUSTOM_BOOL_PROPERTY.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkBasicCUSTOM_INT_PROPERTY)
#include "../../generated/splicewrap/RefThunkBasicCUSTOM_INT_PROPERTY.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkBasicCUSTOM_FLOAT_PROPERTY)
#include "../../generated/splicewrap/RefThunkBasicCUSTOM_FLOAT_PROPERTY.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkBasicCUSTOM_CLQ_PROPERTY (was src/game/app/splice_thunks/RefThunkBasicCUSTOM_CLQ_PROPERTY.cpp) ----
namespace T_RefThunkBasicCUSTOM_CLQ_PROPERTY {
extern "C" { void EnsureLoSidebagClq__FP2LO5OPTIDP3CLQ(void *p, int n, void *pu); CRef RefThunkBasicCUSTOM_CLQ_PROPERTY(void *p, int c, void *aref); }
// Splice binding for option 7 on BASIC (g_aeopid[6], thunkFn/thunkUser): unpacks the script's 2 arguments (Basic?,
// Clq) with RefSetArgsFromSplice, calls EnsureLoSidebagClq and returns nothing to the script.
CRef RefThunkBasicCUSTOM_CLQ_PROPERTY(void *object, int argCount, void *args)
{
    int argTypes[2] = {0x15, 9};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, args, argTypes, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    EnsureLoSidebagClq__FP2LO5OPTIDP3CLQ(object, n, &u);
    return ref;
}
} // namespace T_RefThunkBasicCUSTOM_CLQ_PROPERTY

// ---- splice thunk RefThunkBasicCUSTOM_LM_PROPERTY (was src/game/app/splice_thunks/RefThunkBasicCUSTOM_LM_PROPERTY.cpp) ----
namespace T_RefThunkBasicCUSTOM_LM_PROPERTY {
void EnsureLoSidebagLm__FP2LO5OPTIDP2LM(void *p, int n, void *pu) __asm__("EnsureLoSidebagLm__FP2LO5OPTIDP2LM"); CRef RefThunkBasicCUSTOM_LM_PROPERTY(void *p, int c, void *aref) __asm__("RefThunkBasicCUSTOM_LM_PROPERTY");
// Splice binding for option 8 on BASIC (g_aeopid[7], thunkFn/thunkUser): unpacks the script's 2 arguments (Basic?,
// Lm) with RefSetArgsFromSplice, calls EnsureLoSidebagLm and returns nothing to the script.
CRef RefThunkBasicCUSTOM_LM_PROPERTY(void *object, int argCount, void *args)
{
    int argTypes[2] = {0x15, 0xA};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, args, argTypes, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    EnsureLoSidebagLm__FP2LO5OPTIDP2LM(object, n, &u);
    return ref;
}
} // namespace T_RefThunkBasicCUSTOM_LM_PROPERTY

SLY2_SPLICEWRAP_BEGIN(RefThunkBasicCUSTOM_OID_PROPERTY)
#include "../../generated/splicewrap/RefThunkBasicCUSTOM_OID_PROPERTY.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkBasicCUSTOM_VECTOR_PROPERTY (was src/game/app/splice_thunks/RefThunkBasicCUSTOM_VECTOR_PROPERTY.cpp) ----
namespace T_RefThunkBasicCUSTOM_VECTOR_PROPERTY {
extern "C" { void EnsureLoSidebagVector(void *p, int n, qword q); CRef RefThunkBasicCUSTOM_VECTOR_PROPERTY(void *p, int c, void *aref); }
// Splice binding for option 10 on BASIC (g_aeopid[9], thunkFn/thunkUser): unpacks the script's 2 arguments (Basic?,
// Vector) with RefSetArgsFromSplice, calls EnsureLoSidebagVector and returns nothing to the script.
CRef RefThunkBasicCUSTOM_VECTOR_PROPERTY(void *object, int argCount, void *args)
{
    int argTypes[2] = {0x15, 2};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, args, argTypes, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    EnsureLoSidebagVector(object, n, u.q);
    return ref;
}
} // namespace T_RefThunkBasicCUSTOM_VECTOR_PROPERTY

SLY2_SPLICEWRAP_BEGIN(RefThunk_EnsureLoSidebagBasic)
#include "../../generated/splicewrap/RefThunk_EnsureLoSidebagBasic.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SendTriggerEvent)
#include "../../generated/splicewrap/RefThunk_SendTriggerEvent.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SendUntriggerEvent)
#include "../../generated/splicewrap/RefThunk_SendUntriggerEvent.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkLoMATCHES_NAMEQ)
#include "../../generated/splicewrap/RefThunkLoMATCHES_NAMEQ.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetLoParent)
#include "../../generated/splicewrap/RefThunk_SetLoParent.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_MoveLoToObject)
#include "../../generated/splicewrap/RefThunk_MoveLoToObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkLoADD)
#include "../../generated/splicewrap/RefThunkLoADD.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkLoREMOVEFnUser)
#include "../../generated/splicewrap/RefThunkLoREMOVEFnUser.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkLoREMOVEFn)
#include "../../generated/splicewrap/RefThunkLoREMOVEFn.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearSoAnimIndex)
#include "../../generated/splicewrap/RefThunk_ClearSoAnimIndex.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkSoPOINT_IN_BSP_QUICK (was src/game/app/splice_thunks/RefThunkSoPOINT_IN_BSP_QUICK.cpp) ----
namespace T_RefThunkSoPOINT_IN_BSP_QUICK {
CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *an) __asm__("RefSetArgsFromSplice__FiP4CRefP4OTYPe");
extern "C" CRef RefThunkSoPOINT_IN_BSP_QUICK(SO *p, int c, void *aref);
// Splice binding for option 108 on SO (g_aeopid[43], thunkFn): unpacks the script's 1 argument (Vector) with
// RefSetArgsFromSplice, calls virtual and returns a bool to the script.
CRef RefThunkSoPOINT_IN_BSP_QUICK(SO *object, int argCount, void *args)
{
    union { qword q; } u;
    int argType = 2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, args, &argType, &u);
    ref = object->IsPosInside(u.q) != 0;
    return ref;
}
} // namespace T_RefThunkSoPOINT_IN_BSP_QUICK

SLY2_SPLICEWRAP_BEGIN(RefThunkSo_ShowSoGlobsById)
#include "../../generated/splicewrap/RefThunkSo_ShowSoGlobsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSo_HideSoGlobsById)
#include "../../generated/splicewrap/RefThunkSo_HideSoGlobsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSo_ShowSoPartsById)
#include "../../generated/splicewrap/RefThunkSo_ShowSoPartsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSo_HideSoPartsById)
#include "../../generated/splicewrap/RefThunkSo_HideSoPartsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSoPartsZapKindsById)
#include "../../generated/splicewrap/RefThunk_SetSoPartsZapKindsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSoZapResponse)
#include "../../generated/splicewrap/RefThunk_SetSoZapResponse.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_GetSoZapResponseFlags)
#include "../../generated/splicewrap/RefThunk_GetSoZapResponseFlags.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSoFADE_OUT)
#include "../../generated/splicewrap/RefThunkSoFADE_OUT.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSoFADE_IN)
#include "../../generated/splicewrap/RefThunkSoFADE_IN.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSoScrollerSpeeds)
#include "../../generated/splicewrap/RefThunk_SetSoScrollerSpeeds.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSoSaaParams)
#include "../../generated/splicewrap/RefThunk_SetSoSaaParams.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_FindSoSaa)
#include "../../generated/splicewrap/RefThunk_FindSoSaa.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_FindAsegaByOid)
#include "../../generated/splicewrap/RefThunk_FindAsegaByOid.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_FindLiveSma)
#include "../../generated/splicewrap/RefThunk_FindLiveSma.inc"
SLY2_SPLICEWRAP_END

extern "C" void *NewSoXfHandle(void *p, int a, int b);
// Splice binding for option 194 on SO (g_aeopid[77], thunkFn): unpacks the script's 2 arguments (Oid, 0x104C) with
// RefSetArgsFromSplice, calls NewSoXfHandle and returns a value of type 0x2095 to the script.
// Also known as: RefXfHandle (upstream readable / Sly 1 name; RefXfHandle__FP2LOiPv).
extern "C" CRef RefThunk_NewSoXfHandle(void *p, int c, void *aref)
{
    int ak[2] = {6, 0x104c};
    int a, b;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &a, &b);
    ref.SetBasic((BASIC *)NewSoXfHandle(p, a, b));
    return ref;
}

// Also known as: RefSoeHandle (upstream readable / Sly 1 name; RefSoeHandle__FP2LOiP4CRef).
SLY2_SPLICEWRAP_BEGIN(RefThunk_NewSoPartHandle)
#include "../../generated/splicewrap/RefThunk_NewSoPartHandle.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetSoTargetRecordParams(void *p, int, void *);
// Splice binding for option 196 on SO (g_aeopid[79], thunkFn/thunkUser): unpacks the script's 2 arguments (0x104D,
// Lm) with RefSetArgsFromSplice, calls SetSoTargetRecordParams and returns nothing to the script.
extern "C" CRef RefThunk_SetSoTargetRecordParams(void *p, int c, void *aref)
{
    int ak[2] = {0x104d, 0xa};
    union { qword q; } v1;
    int v0;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &v0, &v1);
    ref.Decref();
    ref.InitTag(0x24);
    SetSoTargetRecordParams(p, v0, &v1);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetScrollerMasterSpeeds)
#include "../../generated/splicewrap/RefThunk_SetScrollerMasterSpeeds.inc"
SLY2_SPLICEWRAP_END

extern "C" void AddAmbSound(void *p, int, int, int, float, float);
// Splice binding for option 207 on CID43 (g_aeopid[99], thunkUser): unpacks the script's 5 arguments (0xE, Int, Int,
// Float, Float) with RefSetArgsFromSplice, calls AddAmbSound and returns nothing to the script.
extern "C" CRef RefThunk_AddAmbSound(void *p, int c, void *aref)
{
    int ak[5] = {0xe, 0x5, 0x5, 0x1, 0x1};
    int v0;
    int v1;
    int v2;
    float v3;
    float v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddAmbSound(p, v0, v1, v2, v3, v4);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddAmbLinkedSound)
#include "../../generated/splicewrap/RefThunk_AddAmbLinkedSound.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkAsegAPPLY (was src/game/app/splice_thunks/RefThunkAsegAPPLY.cpp) ----
namespace T_RefThunkAsegAPPLY {
extern "C" { void * ApplyAsegToObject(void *p, int, float, float, int); CRef RefThunkAsegAPPLY(void *p, int c, void *aref); }
// Splice binding for option 214 on ASEG (g_aeopid[106], thunkFn): unpacks the script's 4 arguments (0x2030, Float,
// Float, 0x100B) with RefSetArgsFromSplice, calls ApplyAsegToObject and returns a value of type 0x2086 to the script.
CRef RefThunkAsegAPPLY(void *object, int argCount, void *args)
{
    int argTypes[4] = {0x2030, 0x1, 0x1, 0x100b};
    int v0;
    float v1;
    float v2;
    int v3;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(4, args, argTypes, &v0, &v1, &v2, &v3);
    ref.SetBasic((BASIC *)ApplyAsegToObject(object, v0, v1, v2, v3));
    return ref;
}
} // namespace T_RefThunkAsegAPPLY

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetAsegAutoStart)
#include "../../generated/splicewrap/RefThunk_SetAsegAutoStart.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkAsegENSURE (was src/game/app/splice_thunks/RefThunkAsegENSURE.cpp) ----
namespace T_RefThunkAsegENSURE {
extern "C" { void * EnsureAsegPlayer__FP4ASEGP3ALO4SEEKffi(void *p, int, int, float, float, int); CRef RefThunkAsegENSURE(void *p, int c, void *aref); }
// Splice binding for option 216 on ASEG (g_aeopid[108], thunkFn): unpacks the script's 5 arguments (0x2030, 0x100C,
// Float, Float, 0x100B) with RefSetArgsFromSplice, calls EnsureAsegPlayer and returns a value
// of type 0x2086 to the script.
CRef RefThunkAsegENSURE(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x2030, 0x100c, 0x1, 0x1, 0x100b};
    int v0;
    int v1;
    float v2;
    float v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.SetBasic((BASIC *)EnsureAsegPlayer__FP4ASEGP3ALO4SEEKffi(object, v0, v1, v2, v3, v4));
    return ref;
}
} // namespace T_RefThunkAsegENSURE

SLY2_SPLICEWRAP_BEGIN(RefThunkAsegFIND_LABEL)
#include "../../generated/splicewrap/RefThunkAsegFIND_LABEL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetAsegaPriority)
#include "../../generated/splicewrap/RefThunk_SetAsegaPriority.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkAsegaSEEK)
#include "../../generated/splicewrap/RefThunkAsegaSEEK.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkAsegaRETRACT)
#include "../../generated/splicewrap/RefThunkAsegaRETRACT.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_EvaluateXfSlotValue)
#include "../../generated/splicewrap/RefThunk_EvaluateXfSlotValue.inc"
SLY2_SPLICEWRAP_END
// Also known as: RefSoundMethod (upstream readable / Sly 1 name; RefSoundMethod__FP5BASICiP4CRef).
SLY2_SPLICEWRAP_BEGIN(RefThunk_PauseSnd)
#include "../../generated/splicewrap/RefThunk_PauseSnd.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ResumeSnd)
#include "../../generated/splicewrap/RefThunk_ResumeSnd.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StopSnd)
#include "../../generated/splicewrap/RefThunk_StopSnd.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSndRange)
#include "../../generated/splicewrap/RefThunk_SetSndRange.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_StartDialog (was src/game/app/splice_thunks/RefThunk_StartDialog.cpp) ----
namespace T_RefThunk_StartDialog {
extern "C" { void *StartDialog(void *p, int a, int b); CRef RefThunk_StartDialog(void *p, int c, void *aref); }
// Splice binding for option 264 on CID152 (g_aeopid[137], thunkFn): unpacks the script's 2 arguments (0x10, 0x105D)
// with RefSetArgsFromSplice, calls StartDialog and returns a value of type 0x2097 to the script.
CRef RefThunk_StartDialog(void *object, int argCount, void *args)
{
    int argTypes[2] = {0x10, 0x105D};
    int a, b;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, args, argTypes, &a, &b);
    ref.SetBasic((BASIC *)StartDialog(object, a, b));
    return ref;
}
} // namespace T_RefThunk_StartDialog

SLY2_SPLICEWRAP_BEGIN(RefThunk_StopDialog)
#include "../../generated/splicewrap/RefThunk_StopDialog.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_IsDialogPlaying)
#include "../../generated/splicewrap/RefThunk_IsDialogPlaying.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_GetDialogStreamDuration)
#include "../../generated/splicewrap/RefThunk_GetDialogStreamDuration.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddMusicLayer2Request)
#include "../../generated/splicewrap/RefThunk_AddMusicLayer2Request.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveMusicLayer2Request)
#include "../../generated/splicewrap/RefThunk_RemoveMusicLayer2Request.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddMusicLayer0Request)
#include "../../generated/splicewrap/RefThunk_AddMusicLayer0Request.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveMusicLayer0Request)
#include "../../generated/splicewrap/RefThunk_RemoveMusicLayer0Request.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_PushMusic)
#include "../../generated/splicewrap/RefThunk_PushMusic.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StopMusic)
#include "../../generated/splicewrap/RefThunk_StopMusic.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_GetMusicTime)
#include "../../generated/splicewrap/RefThunk_GetMusicTime.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSndmgrMasterVolume)
#include "../../generated/splicewrap/RefThunk_SetSndmgrMasterVolume.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSndmgrChannelVolume)
#include "../../generated/splicewrap/RefThunk_SetSndmgrChannelVolume.inc"
SLY2_SPLICEWRAP_END
// Also known as: RefCmJolt (upstream readable / Sly 1 name; RefCmJolt__FP5BASICiP4CRef).
SLY2_SPLICEWRAP_BEGIN(RefThunkCmJOLT)
#include "../../generated/splicewrap/RefThunkCmJOLT.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkCmCUT)
#include "../../generated/splicewrap/RefThunkCmCUT.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearTnCpd)
#include "../../generated/splicewrap/RefThunk_ClearTnCpd.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepPhysByOid)
#include "../../generated/splicewrap/RefThunk_SetStepPhysByOid.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardAttackOid)
#include "../../generated/splicewrap/RefThunk_AddStepguardAttackOid.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddAndResolveStepguardAttackOid)
#include "../../generated/splicewrap/RefThunk_AddAndResolveStepguardAttackOid.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearStepguardAttacks)
#include "../../generated/splicewrap/RefThunk_ClearStepguardAttacks.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetStepguardAnimBinding(void *p, void *, int, int);
// Splice binding for option 369 on STEPGUARD (g_aeopid[243], thunkUser): unpacks the script's 3 arguments (0x9010,
// Oid, 0x100B) with RefSetArgsFromSplice, calls SetStepguardAnimBinding and returns nothing to the script.
extern "C" CRef RefThunk_SetStepguardAnimBinding(void *p, int c, void *aref)
{
    int ak[3] = {0x9010, 0x6, 0x100b};
    CRef v0;
    int v1;
    int v2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    SetStepguardAnimBinding(p, &v0, v1, v2);
    return ref;
}

extern "C" void SetAndResolveStepguardAnimBinding(void *p, void *, int, int);
// Splice binding for option 369 on STEPGUARD (g_aeopid[243], thunkFn): unpacks the script's 3 arguments (0x9010,
// Oid, 0x100B) with RefSetArgsFromSplice, calls SetAndResolveStepguardAnimBinding and returns nothing to the script.
extern "C" CRef RefThunk_SetAndResolveStepguardAnimBinding(void *p, int c, void *aref)
{
    int ak[3] = {0x9010, 0x6, 0x100b};
    CRef v0;
    int v1;
    int v2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    SetAndResolveStepguardAnimBinding(p, &v0, v1, v2);
    return ref;
}

extern "C" void SetStepguardAnimBindingKind3(void *p, void *, int, int, int, int);
// Splice binding for option 370 on STEPGUARD (g_aeopid[244], thunkUser): unpacks the script's 5 arguments (0x9010,
// Oid, Oid, Oid, Oid) with RefSetArgsFromSplice, calls SetStepguardAnimBindingKind3 and returns nothing to the script.
extern "C" CRef RefThunk_SetStepguardAnimBindingKind3(void *p, int c, void *aref)
{
    int ak[5] = {0x9010, 0x6, 0x6, 0x6, 0x6};
    CRef v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    SetStepguardAnimBindingKind3(p, &v0, v1, v2, v3, v4);
    return ref;
}

extern "C" void SetAndResolveStepguardAnimBindingKind3(void *p, void *, int, int, int, int);
// Splice binding for option 370 on STEPGUARD (g_aeopid[244], thunkFn): unpacks the script's 5 arguments (0x9010,
// Oid, Oid, Oid, Oid) with RefSetArgsFromSplice, calls SetAndResolveStepguardAnimBindingKind3 and returns nothing to the
// script.
extern "C" CRef RefThunk_SetAndResolveStepguardAnimBindingKind3(void *p, int c, void *aref)
{
    int ak[5] = {0x9010, 0x6, 0x6, 0x6, 0x6};
    CRef v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    SetAndResolveStepguardAnimBindingKind3(p, &v0, v1, v2, v3, v4);
    return ref;
}

extern "C" void SetStepguardAnimBindingKind2(void *p, void *, int, float, float, float, float);
// Splice binding for option 371 on STEPGUARD (g_aeopid[245], thunkUser): unpacks the script's 6 arguments (0x9010,
// Oid, Float, Float, Float, Float) with RefSetArgsFromSplice, calls SetStepguardAnimBindingKind2 and returns nothing to
// the script.
extern "C" CRef RefThunk_SetStepguardAnimBindingKind2(void *p, int c, void *aref)
{
    int ak[6] = {0x9010, 0x6, 0x1, 0x1, 0x1, 0x1};
    CRef v0;
    int v1;
    float v2;
    float v3;
    float v4;
    float v5;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(6, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5);
    ref.Decref();
    ref.InitTag(0x24);
    SetStepguardAnimBindingKind2(p, &v0, v1, v2, v3, v4, v5);
    return ref;
}

extern "C" void SetAndResolveStepguardAnimBindingKind2(void *p, void *, int, float, float, float, float);
// Splice binding for option 371 on STEPGUARD (g_aeopid[245], thunkFn): unpacks the script's 6 arguments (0x9010,
// Oid, Float, Float, Float, Float) with RefSetArgsFromSplice, calls SetAndResolveStepguardAnimBindingKind2 and returns
// nothing to the script.
extern "C" CRef RefThunk_SetAndResolveStepguardAnimBindingKind2(void *p, int c, void *aref)
{
    int ak[6] = {0x9010, 0x6, 0x1, 0x1, 0x1, 0x1};
    CRef v0;
    int v1;
    float v2;
    float v3;
    float v4;
    float v5;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(6, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5);
    ref.Decref();
    ref.InitTag(0x24);
    SetAndResolveStepguardAnimBindingKind2(p, &v0, v1, v2, v3, v4, v5);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepguardHurtReactions)
#include "../../generated/splicewrap/RefThunk_SetStepguardHurtReactions.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetAndResolveStepguardHurtReactions_Opt372)
#include "../../generated/splicewrap/RefThunk_SetAndResolveStepguardHurtReactions_Opt372.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepguardKnockdownReactions)
#include "../../generated/splicewrap/RefThunk_SetStepguardKnockdownReactions.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetAndResolveStepguardHurtReactions_Opt373)
#include "../../generated/splicewrap/RefThunk_SetAndResolveStepguardHurtReactions_Opt373.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepguardKoReactions)
#include "../../generated/splicewrap/RefThunk_SetStepguardKoReactions.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetAndResolveStepguardKoReactions)
#include "../../generated/splicewrap/RefThunk_SetAndResolveStepguardKoReactions.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardDetectTargetBack)
#include "../../generated/splicewrap/RefThunk_AddStepguardDetectTargetBack.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardPreferClass)
#include "../../generated/splicewrap/RefThunk_AddStepguardPreferClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveStepguardPreferClass)
#include "../../generated/splicewrap/RefThunk_RemoveStepguardPreferClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearStepguardGeneratedTargets)
#include "../../generated/splicewrap/RefThunk_ClearStepguardGeneratedTargets.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveStepguardDetectTarget)
#include "../../generated/splicewrap/RefThunk_RemoveStepguardDetectTarget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearStepguardTargetLists)
#include "../../generated/splicewrap/RefThunk_ClearStepguardTargetLists.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardIgnoreClass)
#include "../../generated/splicewrap/RefThunk_AddStepguardIgnoreClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveStepguardIgnoreClass)
#include "../../generated/splicewrap/RefThunk_RemoveStepguardIgnoreClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearStepguardIgnoreClasses)
#include "../../generated/splicewrap/RefThunk_ClearStepguardIgnoreClasses.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardForceClass)
#include "../../generated/splicewrap/RefThunk_AddStepguardForceClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveStepguardForceClass)
#include "../../generated/splicewrap/RefThunk_RemoveStepguardForceClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearStepguardForceClasses)
#include "../../generated/splicewrap/RefThunk_ClearStepguardForceClasses.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StepguardHasSgf)
#include "../../generated/splicewrap/RefThunk_StepguardHasSgf.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetStepguardTargetLists(void *p, void *, void *);
// Splice binding for option 426 on STEPGUARD (g_aeopid[302], thunkFn): unpacks the script's 2 arguments (0xA002,
// 0xA002) with RefSetArgsFromSplice, calls SetStepguardTargetLists and returns nothing to the script.
extern "C" CRef RefThunk_SetStepguardTargetLists(void *p, int c, void *aref)
{
    int ak[2] = {0xa002, 0xa002};
    CRef v0;
    CRef v1;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &v0, &v1);
    ref.Decref();
    ref.InitTag(0x24);
    SetStepguardTargetLists(p, &v0, &v1);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardLootCoins)
#include "../../generated/splicewrap/RefThunk_AddStepguardLootCoins.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardLootItem_Opt433)
#include "../../generated/splicewrap/RefThunk_AddStepguardLootItem_Opt433.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddStepguardLootItem_Opt434)
#include "../../generated/splicewrap/RefThunk_AddStepguardLootItem_Opt434.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StartGuardScript)
#include "../../generated/splicewrap/RefThunk_StartGuardScript.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetGuardRoutineMode)
#include "../../generated/splicewrap/RefThunk_SetGuardRoutineMode.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_IsGuardRunningScript)
#include "../../generated/splicewrap/RefThunk_IsGuardRunningScript.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetStepguardEscortPoint(void *p, int n, qword q);
// Splice binding for option 446 on STEPGUARD (g_aeopid[322], thunkFn): unpacks the script's 2 arguments (0x2002,
// Vector) with RefSetArgsFromSplice, calls SetStepguardEscortPoint and returns nothing to the script.
extern "C" CRef RefThunk_SetStepguardEscortPoint(void *p, int c, void *aref)
{
    int ak[2] = {0x2002, 2};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    SetStepguardEscortPoint(p, n, u.q);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_RestartStepguard)
#include "../../generated/splicewrap/RefThunk_RestartStepguard.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkStepguard_SetDetectorTargetStateById)
#include "../../generated/splicewrap/RefThunkStepguard_SetDetectorTargetStateById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkStepguard_GetDetectorTargetStateById)
#include "../../generated/splicewrap/RefThunkStepguard_GetDetectorTargetStateById.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_CalculateDetectorTargetState (was src/game/app/splice_thunks/RefThunk_CalculateDetectorTargetState.cpp) ----
namespace T_RefThunk_CalculateDetectorTargetState {
extern "C" { int CalculateDetectorTargetState(void *p, int, int); CRef RefThunk_CalculateDetectorTargetState(void *p, int c, void *aref); }
// Splice binding for option 466 on DETECTOR (g_aeopid[347], thunkFn): unpacks the script's 2 arguments (0x2002,
// 0x106B) with RefSetArgsFromSplice, calls CalculateDetectorTargetState and returns a value of type 0x106B to the
// script.
CRef RefThunk_CalculateDetectorTargetState(void *object, int argCount, void *args)
{
    int argTypes[2] = {0x2002, 0x106b};
    int v0;
    int v1;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, args, argTypes, &v0, &v1);
    ref = CalculateDetectorTargetState(object, v0, v1);
    return ref;
}
} // namespace T_RefThunk_CalculateDetectorTargetState

SLY2_SPLICEWRAP_BEGIN(RefThunkDetector_SetDetectorTargetStateById)
#include "../../generated/splicewrap/RefThunkDetector_SetDetectorTargetStateById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkDetector_GetDetectorTargetStateById)
#include "../../generated/splicewrap/RefThunkDetector_GetDetectorTargetStateById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetGuardAttackOids)
#include "../../generated/splicewrap/RefThunk_SetGuardAttackOids.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetGuardAttackBinding)
#include "../../generated/splicewrap/RefThunk_SetGuardAttackBinding.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetAndResolveGuardAttackBinding)
#include "../../generated/splicewrap/RefThunk_SetAndResolveGuardAttackBinding.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetGuardAttackBindingKind2(void *p, int, float, float, float, float);
// Splice binding for option 371 on CID120 (g_aeopid[365], thunkUser): unpacks the script's 5 arguments (Oid, Float,
// Float, Float, Float) with RefSetArgsFromSplice, calls SetGuardAttackBindingKind2 and returns nothing to the script.
extern "C" CRef RefThunk_SetGuardAttackBindingKind2(void *p, int c, void *aref)
{
    int ak[5] = {0x6, 0x1, 0x1, 0x1, 0x1};
    int v0;
    float v1;
    float v2;
    float v3;
    float v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    SetGuardAttackBindingKind2(p, v0, v1, v2, v3, v4);
    return ref;
}

extern "C" void SetAndResolveGuardAttackBindingKind2(void *p, int, float, float, float, float);
// Splice binding for option 371 on CID120 (g_aeopid[365], thunkFn): unpacks the script's 5 arguments (Oid, Float,
// Float, Float, Float) with RefSetArgsFromSplice, calls SetAndResolveGuardAttackBindingKind2 and returns nothing to the
// script.
extern "C" CRef RefThunk_SetAndResolveGuardAttackBindingKind2(void *p, int c, void *aref)
{
    int ak[5] = {0x6, 0x1, 0x1, 0x1, 0x1};
    int v0;
    float v1;
    float v2;
    float v3;
    float v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    SetAndResolveGuardAttackBindingKind2(p, v0, v1, v2, v3, v4);
    return ref;
}

// ---- splice thunk RefThunkVolCHECK_POINT (was src/game/app/splice_thunks/RefThunkVolCHECK_POINT.cpp) ----
namespace T_RefThunkVolCHECK_POINT {

CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *a0) __asm__("RefSetArgsFromSplice__FiP4CRefP4OTYPe");
int IsPointInVol(void *p, qword q) __asm__("IsPointInVol");
CRef RefThunkVolCHECK_POINT(void *p, int c, void *aref) __asm__("RefThunkVolCHECK_POINT");

// Splice binding for option 108 on VOL (g_aeopid[366], thunkFn): unpacks the script's 1 argument (Vector) with
// RefSetArgsFromSplice, calls IsPointInVol and returns a bool to the script.
CRef RefThunkVolCHECK_POINT(void *object, int argCount, void *args)
{
    int argTypes[1] = {2};
    union { qword q; } u;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, args, argTypes, &u);
    ref = IsPointInVol(object, u.q) != 0;
    return ref;
}
} // namespace T_RefThunkVolCHECK_POINT

extern "C" int IsSphereInVol(void *p, qword q, float g);
// Splice binding for option 474 on VOL (g_aeopid[367], thunkFn): unpacks the script's 2 arguments (Vector, Float)
// with RefSetArgsFromSplice, calls IsSphereInVol and returns a bool to the script.
extern "C" CRef RefThunk_IsSphereInVol(void *p, int c, void *aref)
{
    int ak[2] = {2, 0x1};
    union { qword q; } u;
    float g;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &u, &g);
    ref = IsSphereInVol(p, u.q, g) != 0;
    return ref;
}

// ---- splice thunk RefThunkSmAPPLY (was src/game/app/splice_thunks/RefThunkSmAPPLY.cpp) ----
namespace T_RefThunkSmAPPLY {
void *ApplySmToObject(void *p, int a, int b, int c) __asm__("ApplySmToObject"); CRef RefThunkSmAPPLY(void *p, int c, void *aref) __asm__("RefThunkSmAPPLY");
// Splice binding for option 214 on SM (g_aeopid[368], thunkFn): unpacks the script's 3 arguments (0x2030, Oid,
// 0x100B) with RefSetArgsFromSplice, calls ApplySmToObject and returns a value of type 0x2088 to the script.
CRef RefThunkSmAPPLY(void *object, int argCount, void *args)
{
    int argTypes[3] = {0x2030, 6, 0x100B};
    int a, b, d;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, args, argTypes, &a, &b, &d);
    ref.SetBasic((BASIC *)ApplySmToObject(object, a, b, d));
    return ref;
}
} // namespace T_RefThunkSmAPPLY

// ---- splice thunk RefThunk_ApplySmWithKind (was src/game/app/splice_thunks/RefThunk_ApplySmWithKind.cpp) ----
namespace T_RefThunk_PsmaApplySmEx {
void * ApplySmWithKind(void *p, int, int, int, int) __asm__("ApplySmWithKind"); CRef RefThunk_ApplySmWithKind(void *p, int c, void *aref) __asm__("RefThunk_ApplySmWithKind");
// Splice binding for option 477 on SM (g_aeopid[369], thunkFn): unpacks the script's 4 arguments (0x2030, Oid,
// 0x100B, 0x1054) with RefSetArgsFromSplice, calls ApplySmWithKind and returns a value of type 0x2088 to the script.
CRef RefThunk_ApplySmWithKind(void *object, int argCount, void *args)
{
    int argTypes[4] = {0x2030, 0x6, 0x100b, 0x1054};
    int v0;
    int v1;
    int v2;
    int v3;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(4, args, argTypes, &v0, &v1, &v2, &v3);
    ref.SetBasic((BASIC *)ApplySmWithKind(object, v0, v1, v2, v3));
    return ref;
}
} // namespace T_RefThunk_PsmaApplySmEx

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSmPendingApply)
#include "../../generated/splicewrap/RefThunk_SetSmPendingApply.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetSmaGoalIfReachable)
#include "../../generated/splicewrap/RefThunk_SetSmaGoalIfReachable.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_EstimateSmaTimeToState)
#include "../../generated/splicewrap/RefThunk_EstimateSmaTimeToState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSmaSEEK)
#include "../../generated/splicewrap/RefThunkSmaSEEK.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSmaRETRACT)
#include "../../generated/splicewrap/RefThunkSmaRETRACT.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkBombPRIME)
#include "../../generated/splicewrap/RefThunkBombPRIME.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkBombDETONATE)
#include "../../generated/splicewrap/RefThunkBombDETONATE.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddMissileSpecialObject)
#include "../../generated/splicewrap/RefThunk_AddMissileSpecialObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddMissileSpecialClass)
#include "../../generated/splicewrap/RefThunk_AddMissileSpecialClass.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_FireMissile (was src/game/app/splice_thunks/RefThunk_FireMissile.cpp) ----
namespace T_RefThunk_FireMissile {
struct LO { char pad[0xC]; VTE *pvt; };
extern "C" { CRef RefThunk_FireMissile(LO *p, int c, void *aref); }
// Splice binding for option 508 on MISSILE (g_aeopid[405], thunkFn): unpacks the script's 2 arguments (0x2030,
// Vector) with RefSetArgsFromSplice, calls virtual slot 78 (FireMissile) and returns nothing to the script.
CRef RefThunk_FireMissile(LO *object, int argCount, void *args)
{
    int argTypes[2] = {0x2030, 2};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, args, argTypes, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    VTE *pvte = &object->pvt[78];
    ((void (*)(void *, int, void *))pvte->pfn)((char *)object + pvte->d, n, &u);
    return ref;
}
} // namespace T_RefThunk_FireMissile

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddButtonAseg)
#include "../../generated/splicewrap/RefThunk_AddButtonAseg.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddButtonOffAseg)
#include "../../generated/splicewrap/RefThunk_AddButtonOffAseg.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkButtonSET_SM_GOAL)
#include "../../generated/splicewrap/RefThunkButtonSET_SM_GOAL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddButtonEventListener)
#include "../../generated/splicewrap/RefThunk_AddButtonEventListener.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkButtonTRIGGER)
#include "../../generated/splicewrap/RefThunkButtonTRIGGER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkButtonUNTRIGGER)
#include "../../generated/splicewrap/RefThunkButtonUNTRIGGER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddVolbtnAseg)
#include "../../generated/splicewrap/RefThunk_AddVolbtnAseg.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddVolbtnOffAseg)
#include "../../generated/splicewrap/RefThunk_AddVolbtnOffAseg.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkVolbtnSET_SM_GOAL)
#include "../../generated/splicewrap/RefThunkVolbtnSET_SM_GOAL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddVolbtnEventListener)
#include "../../generated/splicewrap/RefThunk_AddVolbtnEventListener.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkVolbtnTRIGGER)
#include "../../generated/splicewrap/RefThunkVolbtnTRIGGER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkVolbtnUNTRIGGER)
#include "../../generated/splicewrap/RefThunkVolbtnUNTRIGGER.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_AddLasenBeam (was src/game/app/splice_thunks/RefThunk_AddLasenBeam.cpp) ----
namespace T_RefThunk_AddLasenBeam {
extern "C" { void AddLasenBeam(void *p, int, float, float, float, int, int, float, float, float, int); CRef RefThunk_AddLasenBeam(void *p, int c, void *aref); }
// Splice binding for option 541 on CID85 (g_aeopid[455], thunkUser): unpacks the script's 10 arguments (Oid, Float,
// Float, Float, 0x100B, Oid, Float, Float, Float, 0x100B) with RefSetArgsFromSplice, calls AddLasenBeam and returns
// nothing to the script.
CRef RefThunk_AddLasenBeam(void *object, int argCount, void *args)
{
    int argTypes[10] = {0x6, 0x1, 0x1, 0x1, 0x100b, 0x6, 0x1, 0x1, 0x1, 0x100b};
    int v0;
    float v1;
    float v2;
    float v3;
    int v4;
    int v5;
    float v6;
    float v7;
    float v8;
    int v9;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(10, args, argTypes, &v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9);
    ref.Decref();
    ref.InitTag(0x24);
    AddLasenBeam(object, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    return ref;
}
} // namespace T_RefThunk_AddLasenBeam

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBarrierObject)
#include "../../generated/splicewrap/RefThunk_AddBarrierObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBarrierClass)
#include "../../generated/splicewrap/RefThunk_AddBarrierClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_LaunchSoFromLauncher)
#include "../../generated/splicewrap/RefThunk_LaunchSoFromLauncher.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkEmitterPAUSE)
#include "../../generated/splicewrap/RefThunkEmitterPAUSE.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkEmitterPAUSE_FOREVER)
#include "../../generated/splicewrap/RefThunkEmitterPAUSE_FOREVER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkEmitterUNPAUSE)
#include "../../generated/splicewrap/RefThunkEmitterUNPAUSE.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddEmitterToHeroFollowers)
#include "../../generated/splicewrap/RefThunk_AddEmitterToHeroFollowers.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkExplEXPLODE)
#include "../../generated/splicewrap/RefThunkExplEXPLODE.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkExplEXPLODE_OVR (was src/game/app/splice_thunks/RefThunkExplEXPLODE_OVR.cpp) ----
namespace T_RefThunkExplEXPLODE_OVR {
extern "C" { void ExplodeExplParams(void *p, int, int, qword, qword, float, float); CRef RefThunkExplEXPLODE_OVR(void *p, int c, void *aref); }
// Splice binding for option 616 on EXPL (g_aeopid[535], thunkFn): unpacks the script's 6 arguments (0x1026, 0x2002,
// Vector, Vector, Float, Float) with RefSetArgsFromSplice, calls ExplodeExplParams and returns nothing to the
// script.
CRef RefThunkExplEXPLODE_OVR(void *object, int argCount, void *args)
{
    int argTypes[6] = {0x1026, 0x2002, 0x2, 0x2, 0x1, 0x1};
    union { qword q; } v2;
    union { qword q; } v3;
    int v0;
    int v1;
    float v4;
    float v5;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(6, args, argTypes, &v0, &v1, &v2, &v3, &v4, &v5);
    ref.Decref();
    ref.InitTag(0x24);
    ExplodeExplParams(object, v0, v1, v2.q, v3.q, v4, v5);
    return ref;
}
} // namespace T_RefThunkExplEXPLODE_OVR

SLY2_SPLICEWRAP_BEGIN(RefThunk_StartGuardScriptOnGuard)
#include "../../generated/splicewrap/RefThunk_StartGuardScriptOnGuard.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptSetContext)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptSetContext.inc"
SLY2_SPLICEWRAP_END

extern "C" void AddGuardScriptSetWalkTimes(void *p, void *pv);
// Splice binding for option 623 on CID113 (g_aeopid[596], thunkFn/thunkUser): unpacks the script's 1 argument (Lm)
// with RefSetArgsFromSplice, calls AddGuardScriptSetWalkTimes and returns nothing to the script.
extern "C" CRef RefThunk_AddGuardScriptSetWalkTimes(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 0xA;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    ref.Decref();
    ref.InitTag(0x24);
    AddGuardScriptSetWalkTimes(p, &u);
    return ref;
}

extern "C" void AddGuardScriptSetPauseTimes(void *p, void *pv);
// Splice binding for option 624 on CID113 (g_aeopid[597], thunkFn/thunkUser): unpacks the script's 1 argument (Lm)
// with RefSetArgsFromSplice, calls AddGuardScriptSetPauseTimes and returns nothing to the script.
extern "C" CRef RefThunk_AddGuardScriptSetPauseTimes(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 0xA;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    ref.Decref();
    ref.InitTag(0x24);
    AddGuardScriptSetPauseTimes(p, &u);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptTeleport)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptTeleport.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptMessage)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptMessage.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptWait)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptWait.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptFollowPath)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptFollowPath.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptGoto)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptGoto.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptAnim)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptAnim.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptLabel)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptLabel.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptJump)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptJump.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptLeap)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptLeap.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptBusyOn)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptBusyOn.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptBusyOff)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptBusyOff.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptPlayBinding)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptPlayBinding.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddGuardScriptEnsureRelayAlarm)
#include "../../generated/splicewrap/RefThunk_AddGuardScriptEnsureRelayAlarm.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayTRIGGER)
#include "../../generated/splicewrap/RefThunkRelayTRIGGER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_FireRelayFromPlayer)
#include "../../generated/splicewrap/RefThunk_FireRelayFromPlayer.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayUNTRIGGER)
#include "../../generated/splicewrap/RefThunkRelayUNTRIGGER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_EnsureSggAlarm)
#include "../../generated/splicewrap/RefThunk_EnsureSggAlarm.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveAlarmFromRelay)
#include "../../generated/splicewrap/RefThunk_RemoveAlarmFromRelay.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_IsRelayAlarm)
#include "../../generated/splicewrap/RefThunk_IsRelayAlarm.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearRelayAlarms)
#include "../../generated/splicewrap/RefThunk_ClearRelayAlarms.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ResetRelay)
#include "../../generated/splicewrap/RefThunk_ResetRelay.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayBlockBegin)
#include "../../generated/splicewrap/RefThunk_AddRelayBlockBegin.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayBlockEnd)
#include "../../generated/splicewrap/RefThunk_AddRelayBlockEnd.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStopBlock)
#include "../../generated/splicewrap/RefThunk_AddRelayStopBlock.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_AddRelayWaitBlocks (was src/game/app/splice_thunks/RefThunk_AddRelayWaitBlocks.cpp) ----
namespace T_RefThunk_AddRelayWaitBlocks {
extern "C" { void AddRelayWaitBlocks(void *p, CRef *pref); CRef RefThunk_AddRelayWaitBlocks(void *p, int c, void *aref); }
// Splice binding for option 651 on RELAY (g_aeopid[628], thunkFn/thunkUser): unpacks the script's 1 argument
// (0x8006) with RefSetArgsFromSplice, calls AddRelayWaitBlocks and returns nothing to the script.
CRef RefThunk_AddRelayWaitBlocks(void *object, int argCount, void *args)
{
    int argType = 0x8006;
    CRef r;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, args, &argType, &r);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayWaitBlocks(object, &r);
    return ref;
}
} // namespace T_RefThunk_AddRelayWaitBlocks

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWaitAlarm)
#include "../../generated/splicewrap/RefThunk_AddRelayWaitAlarm.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayRaiseAlarm)
#include "../../generated/splicewrap/RefThunk_AddRelayRaiseAlarm.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStopRelay)
#include "../../generated/splicewrap/RefThunk_AddRelayStopRelay.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelaySetCtlState)
#include "../../generated/splicewrap/RefThunk_AddRelaySetCtlState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelaySetActor)
#include "../../generated/splicewrap/RefThunk_AddRelaySetActor.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelaySignal)
#include "../../generated/splicewrap/RefThunk_AddRelaySignal.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayPlayAseg)
#include "../../generated/splicewrap/RefThunk_AddRelayPlayAseg.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayFireRelay)
#include "../../generated/splicewrap/RefThunk_AddRelayFireRelay.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayCONTEXT)
#include "../../generated/splicewrap/RefThunkRelayCONTEXT.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkRelaySET_SM_GOAL (was src/game/app/splice_thunks/RefThunkRelaySET_SM_GOAL.cpp) ----
namespace T_RefThunkRelaySET_SM_GOAL {
extern "C" { void AddRelayApplySm(void *p, int, int, int, int, int); CRef RefThunkRelaySET_SM_GOAL(void *p, int c, void *aref); }
// Splice binding for option 526 on RELAY (g_aeopid[638], thunkFn/thunkUser): unpacks the script's 5 arguments (Oid,
// Oid, Oid, Bool, Bool) with RefSetArgsFromSplice, calls AddRelayApplySm and returns nothing to the script.
CRef RefThunkRelaySET_SM_GOAL(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x6, 0x6, 0x6, 0x0, 0x0};
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayApplySm(object, v0, v1, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunkRelaySET_SM_GOAL

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWidgetShow)
#include "../../generated/splicewrap/RefThunk_AddRelayWidgetShow.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWidgetHide)
#include "../../generated/splicewrap/RefThunk_AddRelayWidgetHide.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayCLOCK)
#include "../../generated/splicewrap/RefThunkRelayCLOCK.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelaySetTaskState)
#include "../../generated/splicewrap/RefThunk_AddRelaySetTaskState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayHandsOff)
#include "../../generated/splicewrap/RefThunk_AddRelayHandsOff.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayLABEL)
#include "../../generated/splicewrap/RefThunkRelayLABEL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelaySEEK)
#include "../../generated/splicewrap/RefThunkRelaySEEK.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayGotoLabelIf)
#include "../../generated/splicewrap/RefThunk_AddRelayGotoLabelIf.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayCUT)
#include "../../generated/splicewrap/RefThunkRelayCUT.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayCutCamera)
#include "../../generated/splicewrap/RefThunk_AddRelayCutCamera.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayTUNNEL)
#include "../../generated/splicewrap/RefThunkRelayTUNNEL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayFOCUS)
#include "../../generated/splicewrap/RefThunkRelayFOCUS.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayPipeTo)
#include "../../generated/splicewrap/RefThunk_AddRelayPipeTo.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWaitAndReleaseGrab)
#include "../../generated/splicewrap/RefThunk_AddRelayWaitAndReleaseGrab.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayHeroState)
#include "../../generated/splicewrap/RefThunk_AddRelayHeroState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepGrab)
#include "../../generated/splicewrap/RefThunk_AddRelayStepGrab.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepThrowAt)
#include "../../generated/splicewrap/RefThunk_AddRelayStepThrowAt.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepGrabState9)
#include "../../generated/splicewrap/RefThunk_AddRelayStepGrabState9.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepAttachToHands)
#include "../../generated/splicewrap/RefThunk_AddRelayStepAttachToHands.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepGrabState5)
#include "../../generated/splicewrap/RefThunk_AddRelayStepGrabState5.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepGrabAseg)
#include "../../generated/splicewrap/RefThunk_AddRelayStepGrabAseg.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayPAUSE)
#include "../../generated/splicewrap/RefThunkRelayPAUSE.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayGoTo)
#include "../../generated/splicewrap/RefThunk_AddRelayGoTo.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayGoToAlt)
#include "../../generated/splicewrap/RefThunk_AddRelayGoToAlt.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayRunOn)
#include "../../generated/splicewrap/RefThunk_AddRelayRunOn.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayRunOff)
#include "../../generated/splicewrap/RefThunk_AddRelayRunOff.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayFollowPath)
#include "../../generated/splicewrap/RefThunk_AddRelayFollowPath.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWalkTo)
#include "../../generated/splicewrap/RefThunk_AddRelayWalkTo.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_AddRelayStepCarryTo (was src/game/app/splice_thunks/RefThunk_AddRelayStepCarryTo.cpp) ----
namespace T_RefThunk_AddRelayStepCmd29 {
extern "C" { void AddRelayStepCarryTo(void *p, int, float, float, int, int); CRef RefThunk_AddRelayStepCarryTo(void *p, int c, void *aref); }
// Splice binding for option 675 on RELAY (g_aeopid[667], thunkFn/thunkUser): unpacks the script's 5 arguments (Oid,
// Float, Float, Bool, Oid) with RefSetArgsFromSplice, calls AddRelayStepCarryTo and returns nothing to the script.
CRef RefThunk_AddRelayStepCarryTo(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x6, 0x1, 0x1, 0x0, 0x6};
    int v0;
    float v1;
    float v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayStepCarryTo(object, v0, v1, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunk_AddRelayStepCmd29

// ---- splice thunk RefThunk_AddRelayRespawn (was src/game/app/splice_thunks/RefThunk_AddRelayRespawn.cpp) ----
namespace T_RefThunk_AddRelayRespawn {
void AddRelayRespawn(void *p, int, int, int, int, int) __asm__("AddRelayRespawn"); CRef RefThunk_AddRelayRespawn(void *p, int c, void *aref) __asm__("RefThunk_AddRelayRespawn");
// Splice binding for option 628 on RELAY (g_aeopid[668], thunkFn/thunkUser): unpacks the script's 5 arguments (Oid,
// 0x102E, 0x102F, Bool, Oid) with RefSetArgsFromSplice, calls AddRelayRespawn and returns nothing to the script.
CRef RefThunk_AddRelayRespawn(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x6, 0x102e, 0x102f, 0x0, 0x6};
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayRespawn(object, v0, v1, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunk_AddRelayRespawn

// ---- splice thunk RefThunk_AddRelayLaunchTo (was src/game/app/splice_thunks/RefThunk_AddRelayLaunchTo.cpp) ----
namespace T_RefThunk_AddRelayLaunchTo {
void AddRelayLaunchTo(void *p, int, float, float, int, int) __asm__("AddRelayLaunchTo"); CRef RefThunk_AddRelayLaunchTo(void *p, int c, void *aref) __asm__("RefThunk_AddRelayLaunchTo");
// Splice binding for option 676 on RELAY (g_aeopid[669], thunkFn/thunkUser): unpacks the script's 5 arguments (Oid,
// Float, Float, Bool, Oid) with RefSetArgsFromSplice, calls AddRelayLaunchTo and returns nothing to the script.
CRef RefThunk_AddRelayLaunchTo(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x6, 0x1, 0x1, 0x0, 0x6};
    int v0;
    float v1;
    float v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayLaunchTo(object, v0, v1, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunk_AddRelayLaunchTo

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayAddObject)
#include "../../generated/splicewrap/RefThunk_AddRelayAddObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayRemoveObject)
#include "../../generated/splicewrap/RefThunk_AddRelayRemoveObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelaySetPartFlag)
#include "../../generated/splicewrap/RefThunk_AddRelaySetPartFlag.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayClearPartFlag)
#include "../../generated/splicewrap/RefThunk_AddRelayClearPartFlag.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayShowPart)
#include "../../generated/splicewrap/RefThunk_AddRelayShowPart.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayHidePart)
#include "../../generated/splicewrap/RefThunk_AddRelayHidePart.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayJOLT)
#include "../../generated/splicewrap/RefThunkRelayJOLT.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayRumble)
#include "../../generated/splicewrap/RefThunk_AddRelayRumble.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkRelayEXPLODE)
#include "../../generated/splicewrap/RefThunkRelayEXPLODE.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayEyes)
#include "../../generated/splicewrap/RefThunk_AddRelayEyes.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_AddRelaySound (was src/game/app/splice_thunks/RefThunk_AddRelaySound.cpp) ----
namespace T_RefThunk_AddRelaySound {
extern "C" { void AddRelaySound(void *p, int, int, int, int, float, float, float); CRef RefThunk_AddRelaySound(void *p, int c, void *aref); }
// Splice binding for option 231 on RELAY (g_aeopid[680], thunkFn/thunkUser): unpacks the script's 7 arguments (0xE,
// Oid, Int, Int, Float, Float, Float) with RefSetArgsFromSplice, calls AddRelaySound and returns nothing to the
// script.
CRef RefThunk_AddRelaySound(void *object, int argCount, void *args)
{
    int argTypes[7] = {0xe, 0x6, 0x5, 0x5, 0x1, 0x1, 0x1};
    int v0;
    int v1;
    int v2;
    int v3;
    float v4;
    float v5;
    float v6;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(7, args, argTypes, &v0, &v1, &v2, &v3, &v4, &v5, &v6);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelaySound(object, v0, v1, v2, v3, v4, v5, v6);
    return ref;
}
} // namespace T_RefThunk_AddRelaySound

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStopSound)
#include "../../generated/splicewrap/RefThunk_AddRelayStopSound.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_AddRelayCamBlendFrom (was src/game/app/splice_thunks/RefThunk_AddRelayCamBlendFrom.cpp) ----
namespace T_RefThunk_AddRelayCamBlendFrom {
extern "C" { void AddRelayCamBlendFrom(void *p, qword, qword, float, float, int); CRef RefThunk_AddRelayCamBlendFrom(void *p, int c, void *aref); }
// Splice binding for option 677 on RELAY (g_aeopid[682], thunkFn/thunkUser): unpacks the script's 5 arguments
// (Vector, Vector, Float, Float, 0x106D) with RefSetArgsFromSplice, calls AddRelayCamBlendFrom and returns nothing
// to the script.
CRef RefThunk_AddRelayCamBlendFrom(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x2, 0x2, 0x1, 0x1, 0x106d};
    union { qword q; } v0;
    union { qword q; } v1;
    float v2;
    float v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayCamBlendFrom(object, v0.q, v1.q, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunk_AddRelayCamBlendFrom

// ---- splice thunk RefThunk_AddRelayCamBlendTo (was src/game/app/splice_thunks/RefThunk_AddRelayCamBlendTo.cpp) ----
namespace T_RefThunk_AddRelayCamBlendTo {
void AddRelayCamBlendTo(void *p, qword, qword, float, int, int) __asm__("AddRelayCamBlendTo"); CRef RefThunk_AddRelayCamBlendTo(void *p, int c, void *aref) __asm__("RefThunk_AddRelayCamBlendTo");
// Splice binding for option 678 on RELAY (g_aeopid[683], thunkFn/thunkUser): unpacks the script's 5 arguments
// (Vector, Vector, Float, Bool, Bool) with RefSetArgsFromSplice, calls AddRelayCamBlendTo and returns nothing to the
// script.
CRef RefThunk_AddRelayCamBlendTo(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x2, 0x2, 0x1, 0x0, 0x0};
    union { qword q; } v0;
    union { qword q; } v1;
    float v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayCamBlendTo(object, v0.q, v1.q, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunk_AddRelayCamBlendTo

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayPopCamBlend)
#include "../../generated/splicewrap/RefThunk_AddRelayPopCamBlend.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStartJtMode)
#include "../../generated/splicewrap/RefThunk_AddRelayStartJtMode.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayEndJtMode)
#include "../../generated/splicewrap/RefThunk_AddRelayEndJtMode.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayTweenPosObj)
#include "../../generated/splicewrap/RefThunk_AddRelayTweenPosObj.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayTweenAimObj)
#include "../../generated/splicewrap/RefThunk_AddRelayTweenAimObj.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_AddRelayTweenTarget (was src/game/app/splice_thunks/RefThunk_AddRelayTweenTarget.cpp) ----
namespace T_RefThunk_AddRelayTweenTarget {
extern "C" { void AddRelayTweenTarget(void *p, qword, qword, float); CRef RefThunk_AddRelayTweenTarget(void *p, int c, void *aref); }
// Splice binding for option 684 on RELAY (g_aeopid[689], thunkFn/thunkUser): unpacks the script's 3 arguments
// (Vector, Vector, Float) with RefSetArgsFromSplice, calls AddRelayTweenTarget and returns nothing to the script.
CRef RefThunk_AddRelayTweenTarget(void *object, int argCount, void *args)
{
    int argTypes[3] = {0x2, 0x2, 0x1};
    union { qword q; } v0;
    union { qword q; } v1;
    float v2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, args, argTypes, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayTweenTarget(object, v0.q, v1.q, v2);
    return ref;
}
} // namespace T_RefThunk_AddRelayTweenTarget

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayTweenTime)
#include "../../generated/splicewrap/RefThunk_AddRelayTweenTime.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayTweenStart)
#include "../../generated/splicewrap/RefThunk_AddRelayTweenStart.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayButtonPrompt)
#include "../../generated/splicewrap/RefThunk_AddRelayButtonPrompt.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWaitPlayerState)
#include "../../generated/splicewrap/RefThunk_AddRelayWaitPlayerState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStopLine)
#include "../../generated/splicewrap/RefThunk_AddRelayStopLine.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayLine)
#include "../../generated/splicewrap/RefThunk_AddRelayLine.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayClearSubtitle)
#include "../../generated/splicewrap/RefThunk_AddRelayClearSubtitle.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepHold)
#include "../../generated/splicewrap/RefThunk_AddRelayStepHold.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStepRelease)
#include "../../generated/splicewrap/RefThunk_AddRelayStepRelease.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayRhythmSequence)
#include "../../generated/splicewrap/RefThunk_AddRelayRhythmSequence.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayRhythmCue)
#include "../../generated/splicewrap/RefThunk_AddRelayRhythmCue.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayPlayAsegDefault)
#include "../../generated/splicewrap/RefThunk_AddRelayPlayAsegDefault.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStartMusic)
#include "../../generated/splicewrap/RefThunk_AddRelayStartMusic.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayStopMusic)
#include "../../generated/splicewrap/RefThunk_AddRelayStopMusic.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWaitTimer)
#include "../../generated/splicewrap/RefThunk_AddRelayWaitTimer.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayMurrayApplySma)
#include "../../generated/splicewrap/RefThunk_AddRelayMurrayApplySma.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWaitBentleyGadget)
#include "../../generated/splicewrap/RefThunk_AddRelayWaitBentleyGadget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWaitBentleyGadgetDone)
#include "../../generated/splicewrap/RefThunk_AddRelayWaitBentleyGadgetDone.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayWaitLine)
#include "../../generated/splicewrap/RefThunk_AddRelayWaitLine.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayHintBox)
#include "../../generated/splicewrap/RefThunk_AddRelayHintBox.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayClearHintBox)
#include "../../generated/splicewrap/RefThunk_AddRelayClearHintBox.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelaySetSsgGoal)
#include "../../generated/splicewrap/RefThunk_AddRelaySetSsgGoal.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayResetStepGoal)
#include "../../generated/splicewrap/RefThunk_AddRelayResetStepGoal.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayMurrayThrowAim)
#include "../../generated/splicewrap/RefThunk_AddRelayMurrayThrowAim.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayBentleyJumpOnMurray)
#include "../../generated/splicewrap/RefThunk_AddRelayBentleyJumpOnMurray.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayMurrayMoveTo)
#include "../../generated/splicewrap/RefThunk_AddRelayMurrayMoveTo.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRelayMurrayMoveToAndControl)
#include "../../generated/splicewrap/RefThunk_AddRelayMurrayMoveToAndControl.inc"
SLY2_SPLICEWRAP_END

extern "C" void LaunchStepplayer(void *p, qword, float, float);
// Splice binding for option 720 on STEPPLAYER (g_aeopid[730], thunkFn): unpacks the script's 3 arguments (Vector,
// Float, Float) with RefSetArgsFromSplice, calls LaunchStepplayer and returns nothing to the script.
extern "C" CRef RefThunk_LaunchStepplayer(void *p, int c, void *aref)
{
    int ak[3] = {0x2, 0x1, 0x1};
    union { qword q; } v0;
    float v1;
    float v2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    LaunchStepplayer(p, v0.q, v1, v2);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepplayerGrabTarget)
#include "../../generated/splicewrap/RefThunk_SetStepplayerGrabTarget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_IsStepplayerGrabReady)
#include "../../generated/splicewrap/RefThunk_IsStepplayerGrabReady.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_StartStepplayerFollow (was src/game/app/splice_thunks/RefThunk_StartStepplayerFollow.cpp) ----
namespace T_RefThunk_StartStepplayerFollow {
extern "C" { void StartStepplayerFollow(void *p, int n, qword q); CRef RefThunk_StartStepplayerFollow(void *p, int c, void *aref); }
// Splice binding for option 446 on STEPPLAYER (g_aeopid[733], thunkFn): unpacks the script's 2 arguments (0x2002,
// Vector) with RefSetArgsFromSplice, calls StartStepplayerFollow and returns nothing to the script.
CRef RefThunk_StartStepplayerFollow(void *object, int argCount, void *args)
{
    int argTypes[2] = {0x2002, 2};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, args, argTypes, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    StartStepplayerFollow(object, n, u.q);
    return ref;
}
} // namespace T_RefThunk_StartStepplayerFollow

SLY2_SPLICEWRAP_BEGIN(RefThunk_StartStepplayerThrowAt)
#include "../../generated/splicewrap/RefThunk_StartStepplayerThrowAt.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_IsStepplayerNotThrowing)
#include "../../generated/splicewrap/RefThunk_IsStepplayerNotThrowing.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepplayerGrabState9)
#include "../../generated/splicewrap/RefThunk_SetStepplayerGrabState9.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_IsStepplayerNotInGrabState9)
#include "../../generated/splicewrap/RefThunk_IsStepplayerNotInGrabState9.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepplayerGrabTargetEx)
#include "../../generated/splicewrap/RefThunk_SetStepplayerGrabTargetEx.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepplayerGrabTargetAlt)
#include "../../generated/splicewrap/RefThunk_SetStepplayerGrabTargetAlt.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StartStepplayerGrabStateC)
#include "../../generated/splicewrap/RefThunk_StartStepplayerGrabStateC.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_IsStepplayerNotInGrabStateC)
#include "../../generated/splicewrap/RefThunk_IsStepplayerNotInGrabStateC.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetStepplayerIdleParams(void *p, void *, void *, void *);
// Splice binding for option 249 on STEPPLAYER (g_aeopid[743], thunkFn): unpacks the script's 3 arguments (Lm, Lm,
// Lm) with RefSetArgsFromSplice, calls SetStepplayerIdleParams and returns nothing to the script.
extern "C" CRef RefThunk_SetStepplayerIdleParams(void *p, int c, void *aref)
{
    int ak[3] = {0xa, 0xa, 0xa};
    union { qword q; } v0;
    union { qword q; } v1;
    union { qword q; } v2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    SetStepplayerIdleParams(p, &v0, &v1, &v2);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_ForceStepplayerLeanTarget)
#include "../../generated/splicewrap/RefThunk_ForceStepplayerLeanTarget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_UnlockStepplayerLeanTarget)
#include "../../generated/splicewrap/RefThunk_UnlockStepplayerLeanTarget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepplayerPlayerState)
#include "../../generated/splicewrap/RefThunk_SetStepplayerPlayerState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SpawnStepplayerBootyMarker)
#include "../../generated/splicewrap/RefThunk_SpawnStepplayerBootyMarker.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_DropStepplayerBooty)
#include "../../generated/splicewrap/RefThunk_DropStepplayerBooty.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_DeliverStepplayerBooty)
#include "../../generated/splicewrap/RefThunk_DeliverStepplayerBooty.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetStepplayerThrowParams(void *p, float, float, void *, float, float, qword, qword);
// Splice binding for option 743 on STEPPLAYER (g_aeopid[760], thunkFn/thunkUser): unpacks the script's 7 arguments
// (Float, Float, Lm, Float, Float, Vector, Vector) with RefSetArgsFromSplice, calls SetStepplayerThrowParams and
// returns nothing to the script.
extern "C" CRef RefThunk_SetStepplayerThrowParams(void *p, int c, void *aref)
{
    int ak[7] = {0x1, 0x1, 0xa, 0x1, 0x1, 0x2, 0x2};
    union { qword q; } v2;
    union { qword q; } v5;
    union { qword q; } v6;
    float v0;
    float v1;
    float v3;
    float v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(7, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5, &v6);
    ref.Decref();
    ref.InitTag(0x24);
    SetStepplayerThrowParams(p, v0, v1, &v2, v3, v4, v5.q, v6.q);
    return ref;
}

extern "C" void SetJtPipeVectors(void *p, qword a, qword b);
// Splice binding for option 749 on STEPPLAYER (g_aeopid[766], thunkFn/thunkUser): unpacks the script's 2 arguments
// (Vector, Vector) with RefSetArgsFromSplice, calls SetJtPipeVectors and returns nothing to the script.
extern "C" CRef RefThunk_SetJtPipeVectors(void *p, int c, void *aref)
{
    int ak[2] = {2, 2};
    union { qword q; } u;
    union { qword q; } v;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &u, &v);
    ref.Decref();
    ref.InitTag(0x24);
    SetJtPipeVectors((char *)p + 0xF50, u.q, v.q);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetStepplayerGadget)
#include "../../generated/splicewrap/RefThunk_SetStepplayerGadget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_GetGadgetSlotFromId)
#include "../../generated/splicewrap/RefThunk_GetGadgetSlotFromId.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetJtGrabObject)
#include "../../generated/splicewrap/RefThunk_SetJtGrabObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetJtUnknown1D30)
#include "../../generated/splicewrap/RefThunk_SetJtUnknown1D30.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetJtMeshProbePoints(void *p, qword, qword, qword);
// Splice binding for option 754 on JT (g_aeopid[771], thunkFn/thunkUser): unpacks the script's 3 arguments (Vector,
// Vector, Vector) with RefSetArgsFromSplice, calls SetJtMeshProbePoints and returns nothing to the script.
extern "C" CRef RefThunk_SetJtMeshProbePoints(void *p, int c, void *aref)
{
    int ak[3] = {0x2, 0x2, 0x2};
    union { qword q; } v0;
    union { qword q; } v1;
    union { qword q; } v2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    SetJtMeshProbePoints(p, v0.q, v1.q, v2.q);
    return ref;
}

extern "C" void SetJtUnknown1FA0(void *p, qword q);
// Splice binding for option 755 on JT (g_aeopid[772], thunkFn/thunkUser): unpacks the script's 1 argument (Vector)
// with RefSetArgsFromSplice, calls SetJtUnknown1FA0 and returns nothing to the script.
extern "C" CRef RefThunk_SetJtUnknown1FA0(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    ref.Decref();
    ref.InitTag(0x24);
    SetJtUnknown1FA0(p, u.q);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_PutHeroAtWarp)
#include "../../generated/splicewrap/RefThunk_PutHeroAtWarp.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkExitTRIGGER)
#include "../../generated/splicewrap/RefThunkExitTRIGGER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddExitTaskState)
#include "../../generated/splicewrap/RefThunk_AddExitTaskState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddWarpAliasStart)
#include "../../generated/splicewrap/RefThunk_AddWarpAliasStart.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddWarpAliasEnd)
#include "../../generated/splicewrap/RefThunk_AddWarpAliasEnd.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddWarpOidAlias)
#include "../../generated/splicewrap/RefThunk_AddWarpOidAlias.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkWarpSET_SM_GOAL)
#include "../../generated/splicewrap/RefThunkWarpSET_SM_GOAL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddWarpTaskState)
#include "../../generated/splicewrap/RefThunk_AddWarpTaskState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddWarpOidPair)
#include "../../generated/splicewrap/RefThunk_AddWarpOidPair.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_FInvulnerablePo)
#include "../../generated/splicewrap/RefThunk_FInvulnerablePo.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetDprizeParams)
#include "../../generated/splicewrap/RefThunk_SetDprizeParams.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddLootEntry)
#include "../../generated/splicewrap/RefThunk_AddLootEntry.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearCurrentLootEntry)
#include "../../generated/splicewrap/RefThunk_ClearCurrentLootEntry.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddLootEntryObject)
#include "../../generated/splicewrap/RefThunk_AddLootEntryObject.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_FindClosestSwPo (was src/game/app/splice_thunks/RefThunk_FindClosestSwPo.cpp) ----
namespace T_RefThunk_FindClosestSwPo {
extern "C" {
extern "C++" { CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *an) __asm__("RefSetArgsFromSplice__FiP4CRefP4OTYPe"); }
void *FindClosestSwPo(void *p, qword q);
CRef RefThunk_FindClosestSwPo(void *p, int c, void *aref);
}
// Splice binding for option 847 on SW (g_aeopid[929], thunkFn): unpacks the script's 1 argument (Vector) with
// RefSetArgsFromSplice, calls FindClosestSwPo and returns a value of type 0x2032 to the script.
CRef RefThunk_FindClosestSwPo(void *object, int argCount, void *args)
{
    union { qword q; } u;
    int argType = 2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, args, &argType, &u);
    ref.SetBasic((BASIC *)FindClosestSwPo(object, u.q));
    return ref;
}
} // namespace T_RefThunk_FindClosestSwPo

// ---- splice thunk RefThunk_EmitSwNoise (was src/game/app/splice_thunks/RefThunk_EmitSwNoise.cpp) ----
namespace T_RefThunk_EmitSwNoise {
void *EmitSwNoise(void *p, int a, qword q, int b) __asm__("EmitSwNoise"); CRef RefThunk_EmitSwNoise(void *p, int c, void *aref) __asm__("RefThunk_EmitSwNoise");
// Splice binding for option 850 on SW (g_aeopid[932], thunkFn): unpacks the script's 3 arguments (0x1046, Vector,
// 0x2002) with RefSetArgsFromSplice, calls EmitSwNoise and returns a value of type 0x2093 to the script.
CRef RefThunk_EmitSwNoise(void *object, int argCount, void *args)
{
    int argTypes[3] = {0x1046, 2, 0x2002};
    union { qword q; } u;
    int a, b;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, args, argTypes, &a, &u, &b);
    ref.SetBasic((BASIC *)EmitSwNoise(object, a, u.q, b));
    return ref;
}
} // namespace T_RefThunk_EmitSwNoise

SLY2_SPLICEWRAP_BEGIN(RefThunk_CallSlot24OnSwObjects)
#include "../../generated/splicewrap/RefThunk_CallSlot24OnSwObjects.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetAmbienceGroupMix)
#include "../../generated/splicewrap/RefThunk_SetAmbienceGroupMix.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSwHANDS_OFF)
#include "../../generated/splicewrap/RefThunkSwHANDS_OFF.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkSwHANDS_ON)
#include "../../generated/splicewrap/RefThunkSwHANDS_ON.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetGrabMarkerProxy)
#include "../../generated/splicewrap/RefThunk_SetGrabMarkerProxy.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkWrADD_CIRCLE_WARP)
#include "../../generated/splicewrap/RefThunkWrADD_CIRCLE_WARP.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkWrADD_BEND_WARP)
#include "../../generated/splicewrap/RefThunkWrADD_BEND_WARP.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkWrADD_BEND_NOISE)
#include "../../generated/splicewrap/RefThunkWrADD_BEND_NOISE.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkWrADD_SWIVEL_NOISE)
#include "../../generated/splicewrap/RefThunkWrADD_SWIVEL_NOISE.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetCritterLink)
#include "../../generated/splicewrap/RefThunk_SetCritterLink.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StartScreenFade)
#include "../../generated/splicewrap/RefThunk_StartScreenFade.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_POCTL_ConsumeAnyPressed)
#include "../../generated/splicewrap/RefThunk_POCTL_ConsumeAnyPressed.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_POCTL_IsButtonDown)
#include "../../generated/splicewrap/RefThunk_POCTL_IsButtonDown.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableTriggerClass)
#include "../../generated/splicewrap/RefThunk_AddBreakableTriggerClass.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableStage)
#include "../../generated/splicewrap/RefThunk_AddBreakableStage.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_InitBreakablePreStage)
#include "../../generated/splicewrap/RefThunk_InitBreakablePreStage.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetBreakableStageParam)
#include "../../generated/splicewrap/RefThunk_SetBreakableStageParam.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableJoltCamera)
#include "../../generated/splicewrap/RefThunk_AddBreakableJoltCamera.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableRumble)
#include "../../generated/splicewrap/RefThunk_AddBreakableRumble.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableSound)
#include "../../generated/splicewrap/RefThunk_AddBreakableSound.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableShowGlobs)
#include "../../generated/splicewrap/RefThunk_AddBreakableShowGlobs.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ShowSoGlobsById)
#include "../../generated/splicewrap/RefThunk_ShowSoGlobsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableShowParts)
#include "../../generated/splicewrap/RefThunk_AddBreakableShowParts.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ShowSoPartsById)
#include "../../generated/splicewrap/RefThunk_ShowSoPartsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableRemoveObject)
#include "../../generated/splicewrap/RefThunk_AddBreakableRemoveObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableHideGlobs)
#include "../../generated/splicewrap/RefThunk_AddBreakableHideGlobs.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_HideSoGlobsById)
#include "../../generated/splicewrap/RefThunk_HideSoGlobsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableHideParts)
#include "../../generated/splicewrap/RefThunk_AddBreakableHideParts.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_HideSoPartsById)
#include "../../generated/splicewrap/RefThunk_HideSoPartsById.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBreakableTaskStates)
#include "../../generated/splicewrap/RefThunk_AddBreakableTaskStates.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ScheduleBreakableReaction)
#include "../../generated/splicewrap/RefThunk_ScheduleBreakableReaction.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetBreakableStage)
#include "../../generated/splicewrap/RefThunk_SetBreakableStage.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StartMurrayLiftGrabspot)
#include "../../generated/splicewrap/RefThunk_StartMurrayLiftGrabspot.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ThrowMurrayCarriedAt)
#include "../../generated/splicewrap/RefThunk_ThrowMurrayCarriedAt.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_EnterMurrayThrowAim)
#include "../../generated/splicewrap/RefThunk_EnterMurrayThrowAim.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearMurrayBehaviors)
#include "../../generated/splicewrap/RefThunk_ClearMurrayBehaviors.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StartMurrayFlop)
#include "../../generated/splicewrap/RefThunk_StartMurrayFlop.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AcquireRenderTargetNoResult)
#include "../../generated/splicewrap/RefThunk_AcquireRenderTargetNoResult.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ReleaseRenderTarget)
#include "../../generated/splicewrap/RefThunk_ReleaseRenderTarget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_FireTriggerGrabspot)
#include "../../generated/splicewrap/RefThunk_FireTriggerGrabspot.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_CanGrabspotAcceptPlayer)
#include "../../generated/splicewrap/RefThunk_CanGrabspotAcceptPlayer.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddTriggerGrabspotRsmg)
#include "../../generated/splicewrap/RefThunk_AddTriggerGrabspotRsmg.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddTriggerGrabspotPair)
#include "../../generated/splicewrap/RefThunk_AddTriggerGrabspotPair.inc"
SLY2_SPLICEWRAP_END

extern "C" void * NewNoise(void *p, qword, int);
// Splice binding for option 214 on CID117 (g_aeopid[1095], thunkFn): unpacks the script's 2 arguments (Vector,
// 0x2002) with RefSetArgsFromSplice, calls NewNoise and returns a value of type 0x2093 to the script.
extern "C" CRef RefThunk_NewNoise(void *p, int c, void *aref)
{
    int ak[2] = {0x2, 0x2002};
    union { qword q; } v0;
    int v1;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &v0, &v1);
    ref.SetBasic((BASIC *)NewNoise(p, v0.q, v1));
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_RemoveNoise)
#include "../../generated/splicewrap/RefThunk_RemoveNoise.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StickTossable)
#include "../../generated/splicewrap/RefThunk_StickTossable.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_DropTossable)
#include "../../generated/splicewrap/RefThunk_DropTossable.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SendRootBhvMode15)
#include "../../generated/splicewrap/RefThunk_SendRootBhvMode15.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ShowWidget)
#include "../../generated/splicewrap/RefThunk_ShowWidget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_HideWidget)
#include "../../generated/splicewrap/RefThunk_HideWidget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_PlayWidgetAnim)
#include "../../generated/splicewrap/RefThunk_PlayWidgetAnim.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StopWidgetAnim)
#include "../../generated/splicewrap/RefThunk_StopWidgetAnim.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetWidgetAnimPhase)
#include "../../generated/splicewrap/RefThunk_SetWidgetAnimPhase.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetWidgetAnimTime)
#include "../../generated/splicewrap/RefThunk_SetWidgetAnimTime.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_ShowWidgetArgsFromSplice (was src/game/app/splice_thunks/RefThunk_ShowWidgetArgsFromSplice.cpp) ----
namespace T_RefThunk_ShowWidgetArgsFromSplice {
extern "C" { void ShowWidgetArgsFromSplice(void *p, CRef *pref); CRef RefThunk_ShowWidgetArgsFromSplice(void *p, int c, void *aref); }
// Splice binding for option 984 on WIDGET (g_aeopid[1132], thunkFn): unpacks the script's 1 argument (0x8010) with
// RefSetArgsFromSplice, calls ShowWidgetArgsFromSplice and returns nothing to the script.
CRef RefThunk_ShowWidgetArgsFromSplice(void *object, int argCount, void *args)
{
    int argType = 0x8010;
    CRef r;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, args, &argType, &r);
    ref.Decref();
    ref.InitTag(0x24);
    ShowWidgetArgsFromSplice(object, &r);
    return ref;
}
} // namespace T_RefThunk_ShowWidgetArgsFromSplice

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddItemCounterOidA)
#include "../../generated/splicewrap/RefThunk_AddItemCounterOidA.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddItemCounterOidB)
#include "../../generated/splicewrap/RefThunk_AddItemCounterOidB.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBootyEntry)
#include "../../generated/splicewrap/RefThunk_AddBootyEntry.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ClearCurrentBootyEntry)
#include "../../generated/splicewrap/RefThunk_ClearCurrentBootyEntry.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddBootyEntryObject)
#include "../../generated/splicewrap/RefThunk_AddBootyEntryObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetBootyContainerKind)
#include "../../generated/splicewrap/RefThunk_SetBootyContainerKind.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_CanAcquireProxy)
#include "../../generated/splicewrap/RefThunk_CanAcquireProxy.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AcquireProxyObject)
#include "../../generated/splicewrap/RefThunk_AcquireProxyObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_EnsureProxyCopy)
#include "../../generated/splicewrap/RefThunk_EnsureProxyCopy.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_LockProxyObject)
#include "../../generated/splicewrap/RefThunk_LockProxyObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_UnlockProxyObject)
#include "../../generated/splicewrap/RefThunk_UnlockProxyObject.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_StartFrameCapture)
#include "../../generated/splicewrap/RefThunk_StartFrameCapture.inc"
SLY2_SPLICEWRAP_END

extern "C" qword GetShapeClosestPoint(void *p, qword q);
// Splice binding for option 1057 on SHAPE (g_aeopid[1223], thunkFn): unpacks the script's 1 argument (Vector) with
// RefSetArgsFromSplice, calls GetShapeClosestPoint and returns a vector to the script.
extern "C" CRef RefThunk_GetShapeClosestPoint(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    CRef_SetVector_VU_VECTOR(&ref, GetShapeClosestPoint(p, u.q));
    return ref;
}

extern "C" void SetXfHandleActuator1World(void *p, int, int, qword);
// Splice binding for option 1060 on CID149 (g_aeopid[1228], thunkFn): unpacks the script's 3 arguments (0x100D, Int,
// Vector) with RefSetArgsFromSplice, calls SetXfHandleActuator1World and returns nothing to the script.
// Also known as: RefXfhMovePos (upstream readable / Sly 1 name; RefXfhMovePos__FP5BASICiP4CRef).
extern "C" CRef RefThunk_SetXfHandleActuator1World(void *p, int c, void *aref)
{
    int ak[3] = {0x100d, 0x5, 0x2};
    union { qword q; } v2;
    int v0;
    int v1;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    SetXfHandleActuator1World(p, v0, v1, v2.q);
    return ref;
}

extern "C" void SetXfHandleActuator1Mat(void *p, int a, int b, void *ps); 
struct BIG { qword aq[4]; };
// Splice binding for option 1061 on CID149 (g_aeopid[1229], thunkFn): unpacks the script's 3 arguments (0x100D, Int,
// 0x4) with RefSetArgsFromSplice, calls SetXfHandleActuator1Mat and returns nothing to the script.
// Also known as: RefXfhMoveRot (upstream readable / Sly 1 name; RefXfhMoveRot__FP5BASICiP4CRef).
extern "C" CRef RefThunk_SetXfHandleActuator1Mat(void *p, int c, void *aref)
{
    int ak[3] = {0x100D, 5, 4};
    BIG s;
    int a, b;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &a, &b, &s);
    ref.Decref();
    ref.InitTag(0x24);
    SetXfHandleActuator1Mat(p, a, b, &s);
    return ref;
}

// Also known as: RefXfhLook (upstream readable / Sly 1 name; RefXfhLook__FP5BASICiP4CRef).
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetXfHandleActuator4)
#include "../../generated/splicewrap/RefThunk_SetXfHandleActuator4.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetXfHandleKind16Params(void *p, int, float, float, float, float, qword);
// Splice binding for option 1063 on CID149 (g_aeopid[1231], thunkFn): unpacks the script's 6 arguments (Int, Float,
// Float, Float, Float, Vector) with RefSetArgsFromSplice, calls SetXfHandleKind16Params and returns nothing to the
// script.
extern "C" CRef RefThunk_SetXfHandleKind16Params(void *p, int c, void *aref)
{
    int ak[6] = {0x5, 0x1, 0x1, 0x1, 0x1, 0x2};
    union { qword q; } v5;
    int v0;
    float v1;
    float v2;
    float v3;
    float v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(6, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5);
    ref.Decref();
    ref.InitTag(0x24);
    SetXfHandleKind16Params(p, v0, v1, v2, v3, v4, v5.q);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddJobCompleteEvent)
#include "../../generated/splicewrap/RefThunk_AddJobCompleteEvent.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddJobFailEvent)
#include "../../generated/splicewrap/RefThunk_AddJobFailEvent.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SignalJobComplete)
#include "../../generated/splicewrap/RefThunk_SignalJobComplete.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RestartJob)
#include "../../generated/splicewrap/RefThunk_RestartJob.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_FailJob)
#include "../../generated/splicewrap/RefThunk_FailJob.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ResetJobTasks)
#include "../../generated/splicewrap/RefThunk_ResetJobTasks.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddJobStateVisibility)
#include "../../generated/splicewrap/RefThunk_AddJobStateVisibility.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunkJobSET_SM_GOAL (was src/game/app/splice_thunks/RefThunkJobSET_SM_GOAL.cpp) ----
namespace T_RefThunkJobSET_SM_GOAL {
extern "C" { void AddJobStateRsmg(void *p, int, int, int, int, int); CRef RefThunkJobSET_SM_GOAL(void *p, int c, void *aref); }
// Splice binding for option 526 on JOB (g_aeopid[1256], thunkUser): unpacks the script's 5 arguments (0x1050, Oid,
// Oid, Oid, 0xF) with RefSetArgsFromSplice, calls AddJobStateRsmg and returns nothing to the script.
CRef RefThunkJobSET_SM_GOAL(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x1050, 0x6, 0x6, 0x6, 0xf};
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddJobStateRsmg(object, v0, v1, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunkJobSET_SM_GOAL

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetJobCompletionWidget)
#include "../../generated/splicewrap/RefThunk_SetJobCompletionWidget.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_RequestTaskState)
#include "../../generated/splicewrap/RefThunk_RequestTaskState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ForceTaskState)
#include "../../generated/splicewrap/RefThunk_ForceTaskState.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SendTaskTimeout)
#include "../../generated/splicewrap/RefThunk_SendTaskTimeout.inc"
SLY2_SPLICEWRAP_END

extern "C" void AddTaskStateVisibility(void *p, int, void *, int, int);
// Splice binding for option 1108 on TASK (g_aeopid[1287], thunkUser): unpacks the script's 4 arguments (Oid, 0x9050,
// Oid, 0xF) with RefSetArgsFromSplice, calls AddTaskStateVisibility and returns nothing to the script.
extern "C" CRef RefThunk_AddTaskStateVisibility(void *p, int c, void *aref)
{
    int ak[4] = {0x6, 0x9050, 0x6, 0xf};
    CRef v1;
    int v0;
    int v2;
    int v3;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(4, aref, ak, &v0, &v1, &v2, &v3);
    ref.Decref();
    ref.InitTag(0x24);
    AddTaskStateVisibility(p, v0, &v1, v2, v3);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunkTaskSET_SM_GOAL)
#include "../../generated/splicewrap/RefThunkTaskSET_SM_GOAL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ForceTaskState0)
#include "../../generated/splicewrap/RefThunk_ForceTaskState0.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ForceTaskState1)
#include "../../generated/splicewrap/RefThunk_ForceTaskState1.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ForceTaskState2)
#include "../../generated/splicewrap/RefThunk_ForceTaskState2.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ForceTaskState3)
#include "../../generated/splicewrap/RefThunk_ForceTaskState3.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ActivateTask)
#include "../../generated/splicewrap/RefThunk_ActivateTask.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_PostNotice)
#include "../../generated/splicewrap/RefThunk_PostNotice.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_CancelNotice)
#include "../../generated/splicewrap/RefThunk_CancelNotice.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddSafehouseEntry)
#include "../../generated/splicewrap/RefThunk_AddSafehouseEntry.inc"
SLY2_SPLICEWRAP_END

// ---- splice thunk RefThunk_SetPoChoices (was src/game/app/splice_thunks/RefThunk_SetPoChoices.cpp) ----
namespace T_RefThunk_SetPoChoices {
extern "C" { void SetPoChoices(void *p, CRef *pref); CRef RefThunk_SetPoChoices(void *p, int c, void *aref); }
// Splice binding for option 1154 on CID70 (g_aeopid[1339], thunkFn/thunkUser): unpacks the script's 1 argument
// (0x8006) with RefSetArgsFromSplice, calls SetPoChoices and returns nothing to the script.
CRef RefThunk_SetPoChoices(void *object, int argCount, void *args)
{
    int argType = 0x8006;
    CRef r;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, args, &argType, &r);
    ref.Decref();
    ref.InitTag(0x24);
    SetPoChoices(object, &r);
    return ref;
}
} // namespace T_RefThunk_SetPoChoices

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddDimitriRoute)
#include "../../generated/splicewrap/RefThunk_AddDimitriRoute.inc"
SLY2_SPLICEWRAP_END

extern "C" qword ChooseEscapePosition(void *p, qword q);
// Splice binding for option 1156 on CID59 (g_aeopid[1341], thunkFn): unpacks the script's 1 argument (Vector) with
// RefSetArgsFromSplice, calls ChooseEscapePosition and returns a vector to the script.
extern "C" CRef RefThunk_ChooseEscapePosition(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    CRef_SetVector_VU_VECTOR(&ref, ChooseEscapePosition(p, u.q));
    return ref;
}

// ---- splice thunk RefThunk_AddDancerMove (was src/game/app/splice_thunks/RefThunk_AddDancerMove.cpp) ----
namespace T_RefThunk_AddDancerMove {
void AddDancerMove(void *p, int, int, int, int, int) __asm__("AddDancerMove"); CRef RefThunk_AddDancerMove(void *p, int c, void *aref) __asm__("RefThunk_AddDancerMove");
// Splice binding for option 1158 on CID68 (g_aeopid[1343], thunkUser): unpacks the script's 5 arguments (0x103C,
// Oid, 0x100B, Oid, Oid) with RefSetArgsFromSplice, calls AddDancerMove and returns nothing to the script.
CRef RefThunk_AddDancerMove(void *object, int argCount, void *args)
{
    int argTypes[5] = {0x103c, 0x6, 0x100b, 0x6, 0x6};
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, args, argTypes, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddDancerMove(object, v0, v1, v2, v3, v4);
    return ref;
}
} // namespace T_RefThunk_AddDancerMove

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRhythmManagerGrade)
#include "../../generated/splicewrap/RefThunk_AddRhythmManagerGrade.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ResetRhythmScore)
#include "../../generated/splicewrap/RefThunk_ResetRhythmScore.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_JumpBhvScriptToLabel)
#include "../../generated/splicewrap/RefThunk_JumpBhvScriptToLabel.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SendCarmelitaRootBhvValue)
#include "../../generated/splicewrap/RefThunk_SendCarmelitaRootBhvValue.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRoutePoint)
#include "../../generated/splicewrap/RefThunk_AddRoutePoint.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_KillHack)
#include "../../generated/splicewrap/RefThunk_KillHack.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ResetHack)
#include "../../generated/splicewrap/RefThunk_ResetHack.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddHackshipOidPair)
#include "../../generated/splicewrap/RefThunk_AddHackshipOidPair.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetHackshipEntry)
#include "../../generated/splicewrap/RefThunk_SetHackshipEntry.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetHackenemyEntry)
#include "../../generated/splicewrap/RefThunk_SetHackenemyEntry.inc"
SLY2_SPLICEWRAP_END

extern "C" void SetHackenemyEntryVectors(void *p, int, void *, void *);
// Splice binding for option 1182 on CID73 (g_aeopid[1380], thunkFn/thunkUser): unpacks the script's 3 arguments
// (0x105F, Lm, Lm) with RefSetArgsFromSplice, calls SetHackenemyEntryVectors and returns nothing to the script.
extern "C" CRef RefThunk_SetHackenemyEntryVectors(void *p, int c, void *aref)
{
    int ak[3] = {0x105f, 0xa, 0xa};
    union { qword q; } v1;
    union { qword q; } v2;
    int v0;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    SetHackenemyEntryVectors(p, v0, &v1, &v2);
    return ref;
}

extern "C" void SetHackenemyEntrySecondVectors(void *p, int, void *, void *);
// Splice binding for option 1183 on CID73 (g_aeopid[1381], thunkFn/thunkUser): unpacks the script's 3 arguments
// (0x105F, Lm, Lm) with RefSetArgsFromSplice, calls SetHackenemyEntrySecondVectors and returns nothing to the script.
extern "C" CRef RefThunk_SetHackenemyEntrySecondVectors(void *p, int c, void *aref)
{
    int ak[3] = {0x105f, 0xa, 0xa};
    union { qword q; } v1;
    union { qword q; } v2;
    int v0;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    SetHackenemyEntrySecondVectors(p, v0, &v1, &v2);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_SetHackbulletMode)
#include "../../generated/splicewrap/RefThunk_SetHackbulletMode.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetHacklockEntry)
#include "../../generated/splicewrap/RefThunk_SetHacklockEntry.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkHacklockSET_SM_GOAL)
#include "../../generated/splicewrap/RefThunkHacklockSET_SM_GOAL.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_ActivateHackshipFromHacklock)
#include "../../generated/splicewrap/RefThunk_ActivateHackshipFromHacklock.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkHacklockTRIGGER)
#include "../../generated/splicewrap/RefThunkHacklockTRIGGER.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_AddHackwallSegment)
#include "../../generated/splicewrap/RefThunk_AddHackwallSegment.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_UseRobottankWeaponAmmo)
#include "../../generated/splicewrap/RefThunk_UseRobottankWeaponAmmo.inc"
SLY2_SPLICEWRAP_END

extern "C" void AddRobottankWheel(void *p, int n, void *pu, float g);
// Splice binding for option 1215 on CID77 (g_aeopid[1419], thunkUser): unpacks the script's 3 arguments (Oid, Float,
// Lm) with RefSetArgsFromSplice, calls AddRobottankWheel and returns nothing to the script.
extern "C" CRef RefThunk_AddRobottankWheel(void *p, int c, void *aref)
{
    int ak[3] = {6, 1, 0xa};
    union { qword q; } u;
    int n;
    float g;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &n, &g, &u);
    ref.Decref();
    ref.InitTag(0x24);
    AddRobottankWheel(p, n, &u, g);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_AddRobottankWeapon)
#include "../../generated/splicewrap/RefThunk_AddRobottankWeapon.inc"
SLY2_SPLICEWRAP_END

extern "C" void AddRobottankAttachment(void *p, int n, void *pu);
// Splice binding for option 1178 on CID77 (g_aeopid[1421], thunkUser): unpacks the script's 2 arguments (Oid, Lm)
// with RefSetArgsFromSplice, calls AddRobottankAttachment and returns nothing to the script.
extern "C" CRef RefThunk_AddRobottankAttachment(void *p, int c, void *aref)
{
    int ak[2] = {6, 0xa};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    AddRobottankAttachment(p, n, &u);
    return ref;
}

SLY2_SPLICEWRAP_BEGIN(RefThunk_FindRobottankWeapon)
#include "../../generated/splicewrap/RefThunk_FindRobottankWeapon.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_GetRobottankSlotValue)
#include "../../generated/splicewrap/RefThunk_GetRobottankSlotValue.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetRobottankSlotValue)
#include "../../generated/splicewrap/RefThunk_SetRobottankSlotValue.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetRobottankAmmo)
#include "../../generated/splicewrap/RefThunk_SetRobottankAmmo.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunk_SetRajanCameraFocus)
#include "../../generated/splicewrap/RefThunk_SetRajanCameraFocus.inc"
SLY2_SPLICEWRAP_END
SLY2_SPLICEWRAP_BEGIN(RefThunkVaultTRIGGER)
#include "../../generated/splicewrap/RefThunkVaultTRIGGER.inc"
SLY2_SPLICEWRAP_END

// ---- Classes ----
// The classes are at file scope under their real names (sly2/classes.h and the headers it includes), with their
// real virtual layouts, so the factories' inline constructors store the real vtables (_vt$<len><NAME>, named in
// config/symbol_addrs.txt). Construction as the game does it: BASIC clears its two counted pointers in member
// constructors, then each class stores its vptr; members with constructors (SO's GLOBSET, PO's +0x174, BLIP,
// PATHLINKEND, BHVGUARDREC) and the out-of-line constructors (SW_ctor, JT_ctor, ConstructLeap, ...) as declared there.
// SW comes from sly2/sw.h (types/views-3): its constructor is SW_ctor, out of line in world/scene/sw.cpp.


// ---- Factories (pfnNew, CID order) ----
// The factories and getters were checked against the descriptor table D_002BAAE8 (pfnNew and size for all 185
// constructible classes, slot 6 for all 199). The guard behaviour classes were confirmed on live guards at runtime;
// runtime observation also identified BHV168 = BHVWALKGOAL, BHV173 = BHVSHOOT and BHV194 = BHVFOLLOW.

extern "C" SW *NewSw() { return new SW; }

extern "C" LO *NewLo() { return new LO; }

extern "C" XFM *NewXfm() { return new XFM; }

extern "C" LIGHT *NewLight() { return new LIGHT; }

extern "C" CID5 *NewCid5() { return new CID5; }

extern "C" PROXY *NewProxy() { return new PROXY; }

extern "C" CLUEPROXY *NewClueproxy() { return new CLUEPROXY; }

extern "C" CID8 *NewCid8() { return new CID8; }

extern "C" CID9 *NewCid9() { return new CID9; }

extern "C" SHAPE *NewShape() { return new SHAPE; }

extern "C" PIPE *NewPipe() { return new PIPE; }

extern "C" CID12 *NewCid12() { return new CID12; }

extern "C" RAIL *NewRail() { return new RAIL; }

extern "C" CID14 *NewCid14() { return new CID14; }

extern "C" TN *NewTn() { return new TN; }

extern "C" DETECTOR *NewDetector() { return new DETECTOR; }

extern "C" PATHZONE *NewPathzone() { return new PATHZONE; }

extern "C" TARGET *NewTarget() { return new TARGET; }

extern "C" HND *NewHnd() { return new HND; }

extern "C" CID20 *NewCid20() { return new CID20; }

extern "C" CID21 *NewCid21() { return new CID21; }

extern "C" VOL *NewVol() { return new VOL; }

extern "C" CID23 *NewCid23() { return new CID23; }

extern "C" VOLBTN *NewVolbtn() { return new VOLBTN; }

extern "C" CID25 *NewCid25() { return new CID25; }

extern "C" EXIT *NewExit() { return new EXIT; }

extern "C" CID27 *NewCid27() { return new CID27; }

extern "C" BLIPG *NewBlipg() { return new BLIPG; }

extern "C" EMITTER *NewEmitter() { return new EMITTER; }

extern "C" EXPLG *NewExplg() { return new EXPLG; }

extern "C" EXPLO *NewExplo() { return new EXPLO; }

extern "C" EXPLS *NewExpls() { return new EXPLS; }

extern "C" CID34 *NewCid34() { return new CID34; }

extern "C" CID35 *NewCid35() { return new CID35; }

extern "C" CID36 *NewCid36() { return new CID36; }

extern "C" CID37 *NewCid37() { return new CID37; }

extern "C" CID38 *NewCid38() { return new CID38; }

extern "C" CID39 *NewCid39() { return new CID39; }

extern "C" CID40 *NewCid40() { return new CID40; }

extern "C" CID41 *NewCid41() { return new CID41; }

extern "C" WARP *NewWarp() { return new WARP; }

extern "C" CID43 *NewCid43() { return new CID43; }

extern "C" CID44 *NewCid44() { return new CID44; }

extern "C" CID45 *NewCid45() { return new CID45; }

extern "C" CID46 *NewCid46() { return new CID46; }

extern "C" CID47 *NewCid47() { return new CID47; }

extern "C" SO *NewSo() { return new SO; }

extern "C" MS *NewMs() { return new MS; }

extern "C" PO *NewPo() { return new PO; }

extern "C" STEP *NewStep() { return new STEP; }

extern "C" JT *NewJt() { return new JT; }

extern "C" MURRAY *NewMurray() { return new MURRAY; }

extern "C" BENTLEY *NewBentley() { return new BENTLEY; }

extern "C" STEPGUARD *NewStepguard() { return new STEPGUARD; }

extern "C" GOMER *NewGomer() { return new GOMER; }

extern "C" GUARD *NewGuard() { return new GUARD; }

extern "C" DIMITRI *NewDimitri() { return new DIMITRI; }

extern "C" NEELA *NewNeela() { return new NEELA; }

extern "C" RAJAN *NewRajan() { return new RAJAN; }

extern "C" JEANBISON *NewJeanbison() { return new JEANBISON; }

extern "C" CARMELITA *NewCarmelita() { return new CARMELITA; }

extern "C" CONTESSA *NewContessa() { return new CONTESSA; }

extern "C" ARPEGGIO *NewArpeggio() { return new ARPEGGIO; }

extern "C" BEAR *NewBear() { return new BEAR; }

extern "C" BEARCUB *NewBearcub() { return new BEARCUB; }

extern "C" DANCER *NewDancer() { return new DANCER; }

extern "C" TURRET *NewTurret() { return new TURRET; }

extern "C" SAFEHOUSE *NewSafehouse() { return new SAFEHOUSE; }

extern "C" HACK *NewHack() { return new HACK; }

extern "C" HACKSHIP *NewHackship() { return new HACKSHIP; }

extern "C" HACKENEMY *NewHackenemy() { return new HACKENEMY; }

extern "C" HACKBULLET *NewHackbullet() { return new HACKBULLET; }

extern "C" HACKLOCK *NewHacklock() { return new HACKLOCK; }

extern "C" HACKWALL *NewHackwall() { return new HACKWALL; }

extern "C" ROBOTTANK *NewRobottank() { return new ROBOTTANK; }

extern "C" VAULT *NewVault() { return new VAULT; }

extern "C" BOMB *NewBomb() { return new BOMB; }

extern "C" MISSILE *NewMissile() { return new MISSILE; }

extern "C" ACCMISS *NewAccmiss() { return new ACCMISS; }

extern "C" CID82 *NewCid82() { return new CID82; }

extern "C" BUTTON *NewButton() { return new BUTTON; }

extern "C" LASEN *NewLasen() { return new LASEN; }

extern "C" BARRIER *NewBarrier() { return new BARRIER; }

extern "C" RIPG *NewRipg() { return new RIPG; }

extern "C" BARREL *NewBarrel() { return new BARREL; }

extern "C" WATER *NewWater() { return new WATER; }

extern "C" COIN *NewCoin() { return new COIN; }

extern "C" KEY *NewKey() { return new KEY; }

extern "C" MEDICINE *NewMedicine() { return new MEDICINE; }

extern "C" LOOT *NewLoot() { return new LOOT; }

extern "C" CLUE *NewClue() { return new CLUE; }

extern "C" CID96 *NewCid96() { return new CID96; }

extern "C" SKY *NewSky() { return new SKY; }

extern "C" CID98 *NewCid98() { return new CID98; }

extern "C" CID99 *NewCid99() { return new CID99; }

extern "C" CID100 *NewCid100() { return new CID100; }

extern "C" CID101 *NewCid101() { return new CID101; }

extern "C" CID102 *NewCid102() { return new CID102; }

extern "C" BOOTY *NewBooty() { return new BOOTY; }

extern "C" CID104 *NewCid104() { return new CID104; }

extern "C" CID105 *NewCid105() { return new CID105; }

extern "C" CID106 *NewCid106() { return new CID106; }

extern "C" WAYPT *NewWaypt() { return new WAYPT; }

extern "C" CID108 *NewCid108() { return new CID108; }

extern "C" ASEG *NewAseg() { return new ASEG; }

extern "C" ASEGBL *NewAsegbl() { return new ASEGBL; }

extern "C" SM *NewSm() { return new SM; }

extern "C" CID112 *NewCid112() { return new CID112; }

extern "C" CID113 *NewCid113() { return new CID113; }

extern "C" RELAY *NewRelay() { return new RELAY; }

extern "C" WR *NewWr() { return new WR; }

extern "C" CID116 *NewCid116() { return new CID116; }

extern "C" CID117 *NewCid117() { return new CID117; }

extern "C" CID118 *NewCid118() { return new CID118; }

extern "C" CID119 *NewCid119() { return new CID119; }

extern "C" CID120 *NewCid120() { return new CID120; }

extern "C" CID121 *NewCid121() { return new CID121; }

extern "C" CID122 *NewCid122() { return new CID122; }

extern "C" CID123 *NewCid123() { return new CID123; }

extern "C" PATHLINK *NewPathlink() { return new PATHLINK; }

extern "C" CID125 *NewCid125() { return new CID125; }

extern "C" WIDGET *NewWidget() { return new WIDGET; }

extern "C" WIDGET127 *NewWidget127() { return new WIDGET127; }

extern "C" WIDGET128 *NewWidget128() { return new WIDGET128; }

extern "C" WIDGET129 *NewWidget129() { return new WIDGET129; }

extern "C" WIDGET130 *NewWidget130() { return new WIDGET130; }

extern "C" WIDGET131 *NewWidget131() { return new WIDGET131; }

extern "C" JOB *NewJob() { return new JOB; }

extern "C" TASK *NewTask() { return new TASK; }

extern "C" CM *NewCm() { return new CM; }

extern "C" SMA *NewSma() { return new SMA; }

extern "C" LOOP *NewLoop() { return new LOOP; }

extern "C" PINGPONG *NewPingpong() { return new PINGPONG; }

extern "C" SHUFFLE *NewShuffle() { return new SHUFFLE; }

extern "C" EYES *NewEyes() { return new EYES; }

extern "C" SCROLLER *NewScroller() { return new SCROLLER; }

extern "C" CIRCLER *NewCircler() { return new CIRCLER; }

extern "C" LEAP *NewLeap() { return new LEAP; }

extern "C" CID146 *NewCid146() { return new CID146; }

extern "C" JSG *NewJsg() { return new JSG; }

extern "C" CID154 *NewCid154() { return new CID154; }

extern "C" BHV *NewBhv() { return new BHV; }

extern "C" BHVGUARD *NewBhvguard() { return new BHVGUARD; }

extern "C" BHVANIM *NewBhvanim() { return new BHVANIM; }

extern "C" BHVIDLE *NewBhvidle() { return new BHVIDLE; }

extern "C" BHVSTRIKE *NewBhvstrike() { return new BHVSTRIKE; }

extern "C" BHVHIT *NewBhvhit() { return new BHVHIT; }

extern "C" BHV161 *NewBhv161() { return new BHV161; }

extern "C" BHV162 *NewBhv162() { return new BHV162; }

extern "C" BHVKO *NewBhvko() { return new BHVKO; }

extern "C" BHVATTACK *NewBhvattack() { return new BHVATTACK; }

extern "C" BHVCOMBAT *NewBhvcombat() { return new BHVCOMBAT; }

extern "C" BHVPATH *NewBhvpath() { return new BHVPATH; }

extern "C" BHVWALKPATH *NewBhvwalkpath() { return new BHVWALKPATH; }

extern "C" BHVWALKGOAL *NewBhvwalkgoal() { return new BHVWALKGOAL; }

extern "C" BHV169 *NewBhv169() { return new BHV169; }

extern "C" BHVPATROL *NewBhvpatrol() { return new BHVPATROL; }

extern "C" BHV171 *NewBhv171() { return new BHV171; }

extern "C" BHVLEAP *NewBhvleap() { return new BHVLEAP; }

extern "C" BHVSHOOT *NewBhvshoot() { return new BHVSHOOT; }

extern "C" BHV174 *NewBhv174() { return new BHV174; }

extern "C" BHV175 *NewBhv175() { return new BHV175; }

extern "C" BHV176 *NewBhv176() { return new BHV176; }

extern "C" BHVJUGGLED *NewBhvjuggled() { return new BHVJUGGLED; }

extern "C" BHV178 *NewBhv178() { return new BHV178; }

extern "C" BHV179 *NewBhv179() { return new BHV179; }

extern "C" BHV180 *NewBhv180() { return new BHV180; }

extern "C" BHV181 *NewBhv181() { return new BHV181; }

extern "C" BHV182 *NewBhv182() { return new BHV182; }

extern "C" BHV183 *NewBhv183() { return new BHV183; }

extern "C" BHVRAIL *NewBhvrail() { return new BHVRAIL; }

extern "C" BHVPIPE *NewBhvpipe() { return new BHVPIPE; }

extern "C" BHVGOTO *NewBhvgoto() { return new BHVGOTO; }

extern "C" BHV187 *NewBhv187() { return new BHV187; }

extern "C" BHV188 *NewBhv188() { return new BHV188; }

extern "C" BHV189 *NewBhv189() { return new BHV189; }

extern "C" BHVPATHLINK *NewBhvpathlink() { return new BHVPATHLINK; }

extern "C" BHV191 *NewBhv191() { return new BHV191; }

extern "C" BHVPATROLPATH *NewBhvpatrolpath() { return new BHVPATROLPATH; }

extern "C" BHV193 *NewBhv193() { return new BHV193; }

extern "C" BHVFOLLOW *NewBhvfollow() { return new BHVFOLLOW; }

extern "C" BHV195 *NewBhv195() { return new BHV195; }

extern "C" BHV196 *NewBhv196() { return new BHV196; }

extern "C" BHV197 *NewBhv197() { return new BHV197; }

extern "C" BHV198 *NewBhv198() { return new BHV198; }


// ---- Descriptor getters (0x113A10 + 0x10 * cid) ----
/* D_002BAAE8: the class descriptor table, one sly2/basic.h CLASSDESC (CLASSINFO and the factory) per CID
   (object-model.md section 1). Defined at the end of the file, after g_aeopid (crt0_splicewrap's .data). */
extern CLASSDESC D_002BAAE8[199];

extern "C" CLASSINFO *GetBasicClassDesc() { return &D_002BAAE8[0]; }

extern "C" CLASSINFO *GetSwClassDesc() { return &D_002BAAE8[1]; }

extern "C" CLASSINFO *GetLoClassDesc() { return &D_002BAAE8[2]; }

extern "C" CLASSINFO *GetXfmClassDesc() { return &D_002BAAE8[3]; }

extern "C" CLASSINFO *GetLightClassDesc() { return &D_002BAAE8[4]; }

extern "C" CLASSINFO *GetCid5ClassDesc() { return &D_002BAAE8[5]; }

extern "C" CLASSINFO *GetProxyClassDesc() { return &D_002BAAE8[6]; }

extern "C" CLASSINFO *GetClueproxyClassDesc() { return &D_002BAAE8[7]; }

extern "C" CLASSINFO *GetCid8ClassDesc() { return &D_002BAAE8[8]; }

extern "C" CLASSINFO *GetCid9ClassDesc() { return &D_002BAAE8[9]; }

extern "C" CLASSINFO *GetShapeClassDesc() { return &D_002BAAE8[10]; }

extern "C" CLASSINFO *GetPipeClassDesc() { return &D_002BAAE8[11]; }

extern "C" CLASSINFO *GetCid12ClassDesc() { return &D_002BAAE8[12]; }

extern "C" CLASSINFO *GetRailClassDesc() { return &D_002BAAE8[13]; }

extern "C" CLASSINFO *GetCid14ClassDesc() { return &D_002BAAE8[14]; }

extern "C" CLASSINFO *GetTnClassDesc() { return &D_002BAAE8[15]; }

extern "C" CLASSINFO *GetDetectorClassDesc() { return &D_002BAAE8[16]; }

extern "C" CLASSINFO *GetPathzoneClassDesc() { return &D_002BAAE8[17]; }

extern "C" CLASSINFO *GetTargetClassDesc() { return &D_002BAAE8[18]; }

extern "C" CLASSINFO *GetHndClassDesc() { return &D_002BAAE8[19]; }

CLASSINFO *CID20::GetClassInfo() { return &D_002BAAE8[20]; }

extern "C" CLASSINFO *GetCid21ClassDesc() { return &D_002BAAE8[21]; }

extern "C" CLASSINFO *GetVolClassDesc() { return &D_002BAAE8[22]; }

extern "C" CLASSINFO *GetCid23ClassDesc() { return &D_002BAAE8[23]; }

extern "C" CLASSINFO *GetVolbtnClassDesc() { return &D_002BAAE8[24]; }

CLASSINFO *CID25::GetClassInfo() { return &D_002BAAE8[25]; }

extern "C" CLASSINFO *GetExitClassDesc() { return &D_002BAAE8[26]; }

extern "C" CLASSINFO *GetCid27ClassDesc() { return &D_002BAAE8[27]; }

extern "C" CLASSINFO *GetBlipgClassDesc() { return &D_002BAAE8[28]; }

extern "C" CLASSINFO *GetEmitterClassDesc() { return &D_002BAAE8[29]; }

extern "C" CLASSINFO *GetExplClassDesc() { return &D_002BAAE8[30]; }

extern "C" CLASSINFO *GetExplgClassDesc() { return &D_002BAAE8[31]; }

CLASSINFO *EXPLO::GetClassInfo() { return &D_002BAAE8[32]; }

extern "C" CLASSINFO *GetExplsClassDesc() { return &D_002BAAE8[33]; }

extern "C" CLASSINFO *GetCid34ClassDesc() { return &D_002BAAE8[34]; }

extern "C" CLASSINFO *GetCid35ClassDesc() { return &D_002BAAE8[35]; }

extern "C" CLASSINFO *GetCid36ClassDesc() { return &D_002BAAE8[36]; }

extern "C" CLASSINFO *GetCid37ClassDesc() { return &D_002BAAE8[37]; }

extern "C" CLASSINFO *GetCid38ClassDesc() { return &D_002BAAE8[38]; }

extern "C" CLASSINFO *GetCid39ClassDesc() { return &D_002BAAE8[39]; }

extern "C" CLASSINFO *GetCid40ClassDesc() { return &D_002BAAE8[40]; }

extern "C" CLASSINFO *GetCid41ClassDesc() { return &D_002BAAE8[41]; }

extern "C" CLASSINFO *GetWarpClassDesc() { return &D_002BAAE8[42]; }

extern "C" CLASSINFO *GetCid43ClassDesc() { return &D_002BAAE8[43]; }

extern "C" CLASSINFO *GetCid44ClassDesc() { return &D_002BAAE8[44]; }

extern "C" CLASSINFO *GetCid45ClassDesc() { return &D_002BAAE8[45]; }

extern "C" CLASSINFO *GetCid46ClassDesc() { return &D_002BAAE8[46]; }

extern "C" CLASSINFO *GetCid47ClassDesc() { return &D_002BAAE8[47]; }

extern "C" CLASSINFO *GetSoClassDesc() { return &D_002BAAE8[48]; }

extern "C" CLASSINFO *GetMsClassDesc() { return &D_002BAAE8[49]; }

extern "C" CLASSINFO *GetPoClassDesc() { return &D_002BAAE8[50]; }

extern "C" CLASSINFO *GetStepClassDesc() { return &D_002BAAE8[51]; }

extern "C" CLASSINFO *GetStepplayerClassDesc() { return &D_002BAAE8[52]; }

extern "C" CLASSINFO *GetJtClassDesc() { return &D_002BAAE8[53]; }

extern "C" CLASSINFO *GetMurrayClassDesc() { return &D_002BAAE8[54]; }

extern "C" CLASSINFO *GetBentleyClassDesc() { return &D_002BAAE8[55]; }

extern "C" CLASSINFO *GetStepguardClassDesc() { return &D_002BAAE8[56]; }

extern "C" CLASSINFO *GetGomerClassDesc() { return &D_002BAAE8[57]; }

CLASSINFO *GUARD::GetClassInfo() { return &D_002BAAE8[58]; }

extern "C" CLASSINFO *GetDimitriClassDesc() { return &D_002BAAE8[59]; }

extern "C" CLASSINFO *GetNeelaClassDesc() { return &D_002BAAE8[60]; }

extern "C" CLASSINFO *GetRajanClassDesc() { return &D_002BAAE8[61]; }

CLASSINFO *JEANBISON::GetClassInfo() { return &D_002BAAE8[62]; }

extern "C" CLASSINFO *GetCarmelitaClassDesc() { return &D_002BAAE8[63]; }

extern "C" CLASSINFO *GetContessaClassDesc() { return &D_002BAAE8[64]; }

CLASSINFO *ARPEGGIO::GetClassInfo() { return &D_002BAAE8[65]; }

CLASSINFO *BEAR::GetClassInfo() { return &D_002BAAE8[66]; }

CLASSINFO *BEARCUB::GetClassInfo() { return &D_002BAAE8[67]; }

extern "C" CLASSINFO *GetDancerClassDesc() { return &D_002BAAE8[68]; }

extern "C" CLASSINFO *GetTurretClassDesc() { return &D_002BAAE8[69]; }

extern "C" CLASSINFO *GetSafehouseClassDesc() { return &D_002BAAE8[70]; }

extern "C" CLASSINFO *GetHackClassDesc() { return &D_002BAAE8[71]; }

extern "C" CLASSINFO *GetHackshipClassDesc() { return &D_002BAAE8[72]; }

extern "C" CLASSINFO *GetHackenemyClassDesc() { return &D_002BAAE8[73]; }

extern "C" CLASSINFO *GetHackbulletClassDesc() { return &D_002BAAE8[74]; }

extern "C" CLASSINFO *GetHacklockClassDesc() { return &D_002BAAE8[75]; }

extern "C" CLASSINFO *GetHackwallClassDesc() { return &D_002BAAE8[76]; }

extern "C" CLASSINFO *GetRobottankClassDesc() { return &D_002BAAE8[77]; }

extern "C" CLASSINFO *GetVaultClassDesc() { return &D_002BAAE8[78]; }

extern "C" CLASSINFO *GetBombClassDesc() { return &D_002BAAE8[79]; }

extern "C" CLASSINFO *GetMissileClassDesc() { return &D_002BAAE8[80]; }

extern "C" CLASSINFO *GetAccmissClassDesc() { return &D_002BAAE8[81]; }

extern "C" CLASSINFO *GetCid82ClassDesc() { return &D_002BAAE8[82]; }

extern "C" CLASSINFO *GetButtonClassDesc() { return &D_002BAAE8[83]; }

extern "C" CLASSINFO *GetSensorClassDesc() { return &D_002BAAE8[84]; }

extern "C" CLASSINFO *GetLasenClassDesc() { return &D_002BAAE8[85]; }

extern "C" CLASSINFO *GetBarrierClassDesc() { return &D_002BAAE8[86]; }

extern "C" CLASSINFO *GetRipgClassDesc() { return &D_002BAAE8[87]; }

extern "C" CLASSINFO *GetBarrelClassDesc() { return &D_002BAAE8[88]; }

extern "C" CLASSINFO *GetWaterClassDesc() { return &D_002BAAE8[89]; }

extern "C" CLASSINFO *GetDprizeClassDesc() { return &D_002BAAE8[90]; }

extern "C" CLASSINFO *GetCoinClassDesc() { return &D_002BAAE8[91]; }

extern "C" CLASSINFO *GetKeyClassDesc() { return &D_002BAAE8[92]; }

extern "C" CLASSINFO *GetMedicineClassDesc() { return &D_002BAAE8[93]; }

extern "C" CLASSINFO *GetLootClassDesc() { return &D_002BAAE8[94]; }

extern "C" CLASSINFO *GetClueClassDesc() { return &D_002BAAE8[95]; }

extern "C" CLASSINFO *GetCid96ClassDesc() { return &D_002BAAE8[96]; }

extern "C" CLASSINFO *GetSkyClassDesc() { return &D_002BAAE8[97]; }

extern "C" CLASSINFO *GetCid98ClassDesc() { return &D_002BAAE8[98]; }

extern "C" CLASSINFO *GetCid99ClassDesc() { return &D_002BAAE8[99]; }

extern "C" CLASSINFO *GetCid100ClassDesc() { return &D_002BAAE8[100]; }

CLASSINFO *CID101::GetClassInfo() { return &D_002BAAE8[101]; }

extern "C" CLASSINFO *GetCid102ClassDesc() { return &D_002BAAE8[102]; }

extern "C" CLASSINFO *GetBootyClassDesc() { return &D_002BAAE8[103]; }

extern "C" CLASSINFO *GetCid104ClassDesc() { return &D_002BAAE8[104]; }

extern "C" CLASSINFO *GetCid105ClassDesc() { return &D_002BAAE8[105]; }

extern "C" CLASSINFO *GetCid106ClassDesc() { return &D_002BAAE8[106]; }

extern "C" CLASSINFO *GetWayptClassDesc() { return &D_002BAAE8[107]; }

extern "C" CLASSINFO *GetCid108ClassDesc() { return &D_002BAAE8[108]; }

extern "C" CLASSINFO *GetAsegClassDesc() { return &D_002BAAE8[109]; }

CLASSINFO *ASEGBL::GetClassInfo() { return &D_002BAAE8[110]; }

extern "C" CLASSINFO *GetSmClassDesc() { return &D_002BAAE8[111]; }

extern "C" CLASSINFO *GetCid112ClassDesc() { return &D_002BAAE8[112]; }

extern "C" CLASSINFO *GetCid113ClassDesc() { return &D_002BAAE8[113]; }

extern "C" CLASSINFO *GetRelayClassDesc() { return &D_002BAAE8[114]; }

extern "C" CLASSINFO *GetWrClassDesc() { return &D_002BAAE8[115]; }

extern "C" CLASSINFO *GetCid116ClassDesc() { return &D_002BAAE8[116]; }

extern "C" CLASSINFO *GetCid117ClassDesc() { return &D_002BAAE8[117]; }

CLASSINFO *CID118::GetClassInfo() { return &D_002BAAE8[118]; }

extern "C" CLASSINFO *GetCid119ClassDesc() { return &D_002BAAE8[119]; }

extern "C" CLASSINFO *GetCid120ClassDesc() { return &D_002BAAE8[120]; }

extern "C" CLASSINFO *GetCid121ClassDesc() { return &D_002BAAE8[121]; }

extern "C" CLASSINFO *GetCid122ClassDesc() { return &D_002BAAE8[122]; }

extern "C" CLASSINFO *GetCid123ClassDesc() { return &D_002BAAE8[123]; }

extern "C" CLASSINFO *GetPathlinkClassDesc() { return &D_002BAAE8[124]; }

extern "C" CLASSINFO *GetCid125ClassDesc() { return &D_002BAAE8[125]; }

extern "C" CLASSINFO *GetWidgetClassDesc() { return &D_002BAAE8[126]; }

extern "C" CLASSINFO *GetWidget127ClassDesc() { return &D_002BAAE8[127]; }

extern "C" CLASSINFO *GetWidget128ClassDesc() { return &D_002BAAE8[128]; }

extern "C" CLASSINFO *GetWidget129ClassDesc() { return &D_002BAAE8[129]; }

CLASSINFO *WIDGET130::GetClassInfo() { return &D_002BAAE8[130]; }

extern "C" CLASSINFO *GetWidget131ClassDesc() { return &D_002BAAE8[131]; }

extern "C" CLASSINFO *GetJobClassDesc() { return &D_002BAAE8[132]; }

extern "C" CLASSINFO *GetTaskClassDesc() { return &D_002BAAE8[133]; }

extern "C" CLASSINFO *GetAsegaClassDesc() { return &D_002BAAE8[134]; }

CLASSINFO *CM::GetClassInfo() { return &D_002BAAE8[135]; }

extern "C" CLASSINFO *GetSmaClassDesc() { return &D_002BAAE8[136]; }

CLASSINFO *POCTL::GetClassInfo() { return &D_002BAAE8[137]; }

extern "C" CLASSINFO *GetSaaClassDesc() { return &D_002BAAE8[138]; }

extern "C" CLASSINFO *GetLoopClassDesc() { return &D_002BAAE8[139]; }

extern "C" CLASSINFO *GetPingpongClassDesc() { return &D_002BAAE8[140]; }

extern "C" CLASSINFO *GetShuffleClassDesc() { return &D_002BAAE8[141]; }

extern "C" CLASSINFO *GetEyesClassDesc() { return &D_002BAAE8[142]; }

extern "C" CLASSINFO *GetScrollerClassDesc() { return &D_002BAAE8[143]; }

extern "C" CLASSINFO *GetCirclerClassDesc() { return &D_002BAAE8[144]; }

extern "C" CLASSINFO *GetLeapClassDesc() { return &D_002BAAE8[145]; }

extern "C" CLASSINFO *GetCid146ClassDesc() { return &D_002BAAE8[146]; }

extern "C" CLASSINFO *GetCid147ClassDesc() { return &D_002BAAE8[147]; }

extern "C" CLASSINFO *GetCid148ClassDesc() { return &D_002BAAE8[148]; }

CLASSINFO *CID149::GetClassInfo() { return &D_002BAAE8[149]; }

CLASSINFO *CID150::GetClassInfo() { return &D_002BAAE8[150]; }

extern "C" CLASSINFO *GetCid151ClassDesc() { return &D_002BAAE8[151]; }

CLASSINFO *CID152::GetClassInfo() { return &D_002BAAE8[152]; }

CLASSINFO *JSG::GetClassInfo() { return &D_002BAAE8[153]; }

CLASSINFO *CID154::GetClassInfo() { return &D_002BAAE8[154]; }

extern "C" CLASSINFO *GetBhvClassDesc() { return &D_002BAAE8[155]; }

extern "C" CLASSINFO *GetBhvguardClassDesc() { return &D_002BAAE8[156]; }

extern "C" CLASSINFO *GetBhvanimClassDesc() { return &D_002BAAE8[157]; }

extern "C" CLASSINFO *GetBhvidleClassDesc() { return &D_002BAAE8[158]; }

extern "C" CLASSINFO *GetBhvstrikeClassDesc() { return &D_002BAAE8[159]; }

extern "C" CLASSINFO *GetBhvhitClassDesc() { return &D_002BAAE8[160]; }

extern "C" CLASSINFO *GetBhv161ClassDesc() { return &D_002BAAE8[161]; }

extern "C" CLASSINFO *GetBhv162ClassDesc() { return &D_002BAAE8[162]; }

extern "C" CLASSINFO *GetBhvkoClassDesc() { return &D_002BAAE8[163]; }

extern "C" CLASSINFO *GetBhvattackClassDesc() { return &D_002BAAE8[164]; }

extern "C" CLASSINFO *GetBhvcombatClassDesc() { return &D_002BAAE8[165]; }

extern "C" CLASSINFO *GetBhvpathClassDesc() { return &D_002BAAE8[166]; }

extern "C" CLASSINFO *GetBhvwalkpathClassDesc() { return &D_002BAAE8[167]; }

extern "C" CLASSINFO *GetBhvwalkgoalClassDesc() { return &D_002BAAE8[168]; }

extern "C" CLASSINFO *GetBhv169ClassDesc() { return &D_002BAAE8[169]; }

extern "C" CLASSINFO *GetBhvpatrolClassDesc() { return &D_002BAAE8[170]; }

extern "C" CLASSINFO *GetBhv171ClassDesc() { return &D_002BAAE8[171]; }

extern "C" CLASSINFO *GetBhvleapClassDesc() { return &D_002BAAE8[172]; }

extern "C" CLASSINFO *GetBhvshootClassDesc() { return &D_002BAAE8[173]; }

extern "C" CLASSINFO *GetBhv174ClassDesc() { return &D_002BAAE8[174]; }

extern "C" CLASSINFO *GetBhv175ClassDesc() { return &D_002BAAE8[175]; }

extern "C" CLASSINFO *GetBhv176ClassDesc() { return &D_002BAAE8[176]; }

extern "C" CLASSINFO *GetBhvjuggledClassDesc() { return &D_002BAAE8[177]; }

extern "C" CLASSINFO *GetBhv178ClassDesc() { return &D_002BAAE8[178]; }

extern "C" CLASSINFO *GetBhv179ClassDesc() { return &D_002BAAE8[179]; }

extern "C" CLASSINFO *GetBhv180ClassDesc() { return &D_002BAAE8[180]; }

extern "C" CLASSINFO *GetBhv181ClassDesc() { return &D_002BAAE8[181]; }

extern "C" CLASSINFO *GetBhv182ClassDesc() { return &D_002BAAE8[182]; }

extern "C" CLASSINFO *GetBhv183ClassDesc() { return &D_002BAAE8[183]; }

extern "C" CLASSINFO *GetBhvrailClassDesc() { return &D_002BAAE8[184]; }

extern "C" CLASSINFO *GetBhvpipeClassDesc() { return &D_002BAAE8[185]; }

extern "C" CLASSINFO *GetBhvgotoClassDesc() { return &D_002BAAE8[186]; }

extern "C" CLASSINFO *GetBhv187ClassDesc() { return &D_002BAAE8[187]; }

extern "C" CLASSINFO *GetBhv188ClassDesc() { return &D_002BAAE8[188]; }

extern "C" CLASSINFO *GetBhv189ClassDesc() { return &D_002BAAE8[189]; }

extern "C" CLASSINFO *GetBhvpathlinkClassDesc() { return &D_002BAAE8[190]; }

extern "C" CLASSINFO *GetBhv191ClassDesc() { return &D_002BAAE8[191]; }

extern "C" CLASSINFO *GetBhvpatrolpathClassDesc() { return &D_002BAAE8[192]; }

extern "C" CLASSINFO *GetBhv193ClassDesc() { return &D_002BAAE8[193]; }

extern "C" CLASSINFO *GetBhvfollowClassDesc() { return &D_002BAAE8[194]; }

extern "C" CLASSINFO *GetBhv195ClassDesc() { return &D_002BAAE8[195]; }

extern "C" CLASSINFO *GetBhv196ClassDesc() { return &D_002BAAE8[196]; }

extern "C" CLASSINFO *GetBhv197ClassDesc() { return &D_002BAAE8[197]; }

extern "C" CLASSINFO *GetBhv198ClassDesc() { return &D_002BAAE8[198]; }

// ---- Out-of-line constructors from the linkonce area (0x256360..0x256498) ----
// The constructors of STEPGUARD, JT and MURRAY: inline in the game (their copies sit in the linkonce area) but
// called, not inlined, by the factories (HYPOTHESIS: too big for g++'s inliner). Defined here as the constructors
// they are, under their current symbols (the asm labels in the views above). The real form, inline in the class, is
// not emitted here: g++ 2.95 emits a polymorphic class's inline members only in the TU that emits its vtable (the
// key TU: stepguard.cpp for STEPGUARD since data/vtables). Defined inline in sly2/classes.h, the key TU does emit
// STEPGUARD_ctor (MATCH, weak in .gnu.linkonce.t.), but every factory here then inlines it (NewStepguard and the ten
// guard factories differ), so the HYPOTHESIS above is wrong for this compiler: these factories must not see an inline
// definition. Left as is (integrate/round9): the definitions' attributes give them what an out-of-line inline copy
// gets, a weak symbol in .gnu.linkonce.t.<symbol>, emitted by this object only.

// STEPGUARD's constructor (out-of-line copy of the inline one): base constructors, then the STEPGUARD vtable
// and its LEAP sub-object.
__attribute__((weak, section(".gnu.linkonce.t.STEPGUARD_ctor")))
STEPGUARD::STEPGUARD() {}

// JT's (Sly's player class) constructor (out-of-line inline copy): clears the gadget sound reference and +0x2048.
__attribute__((weak, section(".gnu.linkonce.t.JT_ctor")))
JT::JT() : psndGadget(refptrNull) { unknown2048 = 0; }

// MURRAY's constructor (out-of-line inline copy).
__attribute__((weak, section(".gnu.linkonce.t.MURRAY_ctor")))
MURRAY::MURRAY() {}

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
#include "class_registry_options.inc"   /* the Splice option tables: g_aeopid, the class option arrays */
/* The class descriptor table D_002BAAE8 (declared above as CLASSDESC[199]). CLASSDESC derives from CLASSINFO and so
   has no aggregate initializer: the table is defined under C linkage with a plain record of the same layout
   {parent, cid, cidEnd, cb, pfnNew}; the getters keep returning &D_002BAAE8[cid]. cb is sizeof(class) (every row
   equals the ELF's size; the headers assert the EE sizes), so another layout gets its own instance sizes: CloneSaa
   (shdanim.cpp) copies instanceSize bytes, and the factories allocate by class. */
namespace M_classdesc {
struct CLASSROW { CLASSROW *pcdParent; int cid; int cidEnd; int cb; void *(*pfnNew)(); };
typedef void *(*PFNNEW)();
CLASSROW D_002BAAE8[199] __asm__("D_002BAAE8") = {
    {0, 0, 199, sizeof(BASIC), 0}, /* Basic */
    {&D_002BAAE8[0], 1, 2, sizeof(SW), (PFNNEW)NewSw}, /* Sw */
    {&D_002BAAE8[0], 2, 134, sizeof(LO), (PFNNEW)NewLo}, /* Lo */
    {&D_002BAAE8[2], 3, 48, sizeof(XFM), (PFNNEW)NewXfm}, /* Xfm */
    {&D_002BAAE8[3], 4, 5, sizeof(LIGHT), (PFNNEW)NewLight}, /* Light */
    {&D_002BAAE8[3], 5, 6, sizeof(CID5), (PFNNEW)NewCid5}, /* Cid5 */
    {&D_002BAAE8[3], 6, 8, sizeof(PROXY), (PFNNEW)NewProxy}, /* Proxy */
    {&D_002BAAE8[6], 7, 8, sizeof(CLUEPROXY), (PFNNEW)NewClueproxy}, /* Clueproxy */
    {&D_002BAAE8[3], 8, 10, sizeof(CID8), (PFNNEW)NewCid8}, /* Cid8 */
    {&D_002BAAE8[8], 9, 10, sizeof(CID9), (PFNNEW)NewCid9}, /* Cid9 */
    {&D_002BAAE8[3], 10, 15, sizeof(SHAPE), (PFNNEW)NewShape}, /* Shape */
    {&D_002BAAE8[10], 11, 12, sizeof(PIPE), (PFNNEW)NewPipe}, /* Pipe */
    {&D_002BAAE8[10], 12, 13, sizeof(CID12), (PFNNEW)NewCid12}, /* Cid12 */
    {&D_002BAAE8[10], 13, 14, sizeof(RAIL), (PFNNEW)NewRail}, /* Rail */
    {&D_002BAAE8[10], 14, 15, sizeof(CID14), (PFNNEW)NewCid14}, /* Cid14 */
    {&D_002BAAE8[3], 15, 16, sizeof(TN), (PFNNEW)NewTn}, /* Tn */
    {&D_002BAAE8[3], 16, 17, sizeof(DETECTOR), (PFNNEW)NewDetector}, /* Detector */
    {&D_002BAAE8[3], 17, 18, sizeof(PATHZONE), (PFNNEW)NewPathzone}, /* Pathzone */
    {&D_002BAAE8[3], 18, 20, sizeof(TARGET), (PFNNEW)NewTarget}, /* Target */
    {&D_002BAAE8[18], 19, 20, sizeof(HND), (PFNNEW)NewHnd}, /* Hnd */
    {&D_002BAAE8[3], 20, 21, sizeof(CID20), (PFNNEW)NewCid20}, /* Cid20 */
    {&D_002BAAE8[3], 21, 22, sizeof(CID21), (PFNNEW)NewCid21}, /* Cid21 */
    {&D_002BAAE8[3], 22, 28, sizeof(VOL), (PFNNEW)NewVol}, /* Vol */
    {&D_002BAAE8[22], 23, 24, sizeof(CID23), (PFNNEW)NewCid23}, /* Cid23 */
    {&D_002BAAE8[22], 24, 26, sizeof(VOLBTN), (PFNNEW)NewVolbtn}, /* Volbtn */
    {&D_002BAAE8[24], 25, 26, sizeof(CID25), (PFNNEW)NewCid25}, /* Cid25 */
    {&D_002BAAE8[22], 26, 27, sizeof(EXIT), (PFNNEW)NewExit}, /* Exit */
    {&D_002BAAE8[22], 27, 28, sizeof(CID27), (PFNNEW)NewCid27}, /* Cid27 */
    {&D_002BAAE8[3], 28, 29, sizeof(BLIPG), (PFNNEW)NewBlipg}, /* Blipg */
    {&D_002BAAE8[3], 29, 30, sizeof(EMITTER), (PFNNEW)NewEmitter}, /* Emitter */
    {&D_002BAAE8[3], 30, 34, sizeof(EXPL), 0}, /* Expl */
    {&D_002BAAE8[30], 31, 32, sizeof(EXPLG), (PFNNEW)NewExplg}, /* Explg */
    {&D_002BAAE8[30], 32, 34, sizeof(EXPLO), (PFNNEW)NewExplo}, /* Explo */
    {&D_002BAAE8[32], 33, 34, sizeof(EXPLS), (PFNNEW)NewExpls}, /* Expls */
    {&D_002BAAE8[3], 34, 35, sizeof(CID34), (PFNNEW)NewCid34}, /* Cid34 */
    {&D_002BAAE8[3], 35, 38, sizeof(CID35), (PFNNEW)NewCid35}, /* Cid35 */
    {&D_002BAAE8[35], 36, 37, sizeof(CID36), (PFNNEW)NewCid36}, /* Cid36 */
    {&D_002BAAE8[35], 37, 38, sizeof(CID37), (PFNNEW)NewCid37}, /* Cid37 */
    {&D_002BAAE8[3], 38, 39, sizeof(CID38), (PFNNEW)NewCid38}, /* Cid38 */
    {&D_002BAAE8[3], 39, 40, sizeof(CID39), (PFNNEW)NewCid39}, /* Cid39 */
    {&D_002BAAE8[3], 40, 41, sizeof(CID40), (PFNNEW)NewCid40}, /* Cid40 */
    {&D_002BAAE8[3], 41, 42, sizeof(CID41), (PFNNEW)NewCid41}, /* Cid41 */
    {&D_002BAAE8[3], 42, 43, sizeof(WARP), (PFNNEW)NewWarp}, /* Warp */
    {&D_002BAAE8[3], 43, 44, sizeof(CID43), (PFNNEW)NewCid43}, /* Cid43 */
    {&D_002BAAE8[3], 44, 45, sizeof(CID44), (PFNNEW)NewCid44}, /* Cid44 */
    {&D_002BAAE8[3], 45, 46, sizeof(CID45), (PFNNEW)NewCid45}, /* Cid45 */
    {&D_002BAAE8[3], 46, 47, sizeof(CID46), (PFNNEW)NewCid46}, /* Cid46 */
    {&D_002BAAE8[3], 47, 48, sizeof(CID47), (PFNNEW)NewCid47}, /* Cid47 */
    {&D_002BAAE8[2], 48, 109, sizeof(SO), (PFNNEW)NewSo}, /* So */
    {&D_002BAAE8[48], 49, 50, sizeof(MS), (PFNNEW)NewMs}, /* Ms */
    {&D_002BAAE8[48], 50, 79, sizeof(PO), (PFNNEW)NewPo}, /* Po */
    {&D_002BAAE8[50], 51, 69, sizeof(STEP), (PFNNEW)NewStep}, /* Step */
    {&D_002BAAE8[51], 52, 56, sizeof(STEPPLAYER), 0}, /* Stepplayer */
    {&D_002BAAE8[52], 53, 54, sizeof(JT), (PFNNEW)NewJt}, /* Jt */
    {&D_002BAAE8[52], 54, 55, sizeof(MURRAY), (PFNNEW)NewMurray}, /* Murray */
    {&D_002BAAE8[52], 55, 56, sizeof(BENTLEY), (PFNNEW)NewBentley}, /* Bentley */
    {&D_002BAAE8[51], 56, 68, sizeof(STEPGUARD), (PFNNEW)NewStepguard}, /* Stepguard */
    {&D_002BAAE8[56], 57, 58, sizeof(GOMER), (PFNNEW)NewGomer}, /* Gomer */
    {&D_002BAAE8[56], 58, 60, sizeof(GUARD), (PFNNEW)NewGuard}, /* Guard */
    {&D_002BAAE8[58], 59, 60, sizeof(DIMITRI), (PFNNEW)NewDimitri}, /* Dimitri */
    {&D_002BAAE8[56], 60, 61, sizeof(NEELA), (PFNNEW)NewNeela}, /* Neela */
    {&D_002BAAE8[56], 61, 62, sizeof(RAJAN), (PFNNEW)NewRajan}, /* Rajan */
    {&D_002BAAE8[56], 62, 63, sizeof(JEANBISON), (PFNNEW)NewJeanbison}, /* Jeanbison */
    {&D_002BAAE8[56], 63, 64, sizeof(CARMELITA), (PFNNEW)NewCarmelita}, /* Carmelita */
    {&D_002BAAE8[56], 64, 65, sizeof(CONTESSA), (PFNNEW)NewContessa}, /* Contessa */
    {&D_002BAAE8[56], 65, 66, sizeof(ARPEGGIO), (PFNNEW)NewArpeggio}, /* Arpeggio */
    {&D_002BAAE8[56], 66, 67, sizeof(BEAR), (PFNNEW)NewBear}, /* Bear */
    {&D_002BAAE8[56], 67, 68, sizeof(BEARCUB), (PFNNEW)NewBearcub}, /* Bearcub */
    {&D_002BAAE8[51], 68, 69, sizeof(DANCER), (PFNNEW)NewDancer}, /* Dancer */
    {&D_002BAAE8[50], 69, 70, sizeof(TURRET), (PFNNEW)NewTurret}, /* Turret */
    {&D_002BAAE8[50], 70, 71, sizeof(SAFEHOUSE), (PFNNEW)NewSafehouse}, /* Safehouse */
    {&D_002BAAE8[50], 71, 77, sizeof(HACK), (PFNNEW)NewHack}, /* Hack */
    {&D_002BAAE8[71], 72, 73, sizeof(HACKSHIP), (PFNNEW)NewHackship}, /* Hackship */
    {&D_002BAAE8[71], 73, 74, sizeof(HACKENEMY), (PFNNEW)NewHackenemy}, /* Hackenemy */
    {&D_002BAAE8[71], 74, 75, sizeof(HACKBULLET), (PFNNEW)NewHackbullet}, /* Hackbullet */
    {&D_002BAAE8[71], 75, 76, sizeof(HACKLOCK), (PFNNEW)NewHacklock}, /* Hacklock */
    {&D_002BAAE8[71], 76, 77, sizeof(HACKWALL), (PFNNEW)NewHackwall}, /* Hackwall */
    {&D_002BAAE8[50], 77, 78, sizeof(ROBOTTANK), (PFNNEW)NewRobottank}, /* Robottank */
    {&D_002BAAE8[50], 78, 79, sizeof(VAULT), (PFNNEW)NewVault}, /* Vault */
    {&D_002BAAE8[48], 79, 83, sizeof(BOMB), (PFNNEW)NewBomb}, /* Bomb */
    {&D_002BAAE8[79], 80, 83, sizeof(MISSILE), (PFNNEW)NewMissile}, /* Missile */
    {&D_002BAAE8[80], 81, 82, sizeof(ACCMISS), (PFNNEW)NewAccmiss}, /* Accmiss */
    {&D_002BAAE8[80], 82, 83, sizeof(CID82), (PFNNEW)NewCid82}, /* Cid82 */
    {&D_002BAAE8[48], 83, 84, sizeof(BUTTON), (PFNNEW)NewButton}, /* Button */
    {&D_002BAAE8[48], 84, 86, sizeof(SENSOR), 0}, /* Sensor */
    {&D_002BAAE8[84], 85, 86, sizeof(LASEN), (PFNNEW)NewLasen}, /* Lasen */
    {&D_002BAAE8[48], 86, 87, sizeof(BARRIER), (PFNNEW)NewBarrier}, /* Barrier */
    {&D_002BAAE8[48], 87, 88, sizeof(RIPG), (PFNNEW)NewRipg}, /* Ripg */
    {&D_002BAAE8[48], 88, 89, sizeof(BARREL), (PFNNEW)NewBarrel}, /* Barrel */
    {&D_002BAAE8[48], 89, 90, sizeof(WATER), (PFNNEW)NewWater}, /* Water */
    {&D_002BAAE8[48], 90, 96, sizeof(DPRIZE), 0}, /* Dprize */
    {&D_002BAAE8[90], 91, 92, sizeof(COIN), (PFNNEW)NewCoin}, /* Coin */
    {&D_002BAAE8[90], 92, 93, sizeof(KEY), (PFNNEW)NewKey}, /* Key */
    {&D_002BAAE8[90], 93, 94, sizeof(MEDICINE), (PFNNEW)NewMedicine}, /* Medicine */
    {&D_002BAAE8[90], 94, 95, sizeof(LOOT), (PFNNEW)NewLoot}, /* Loot */
    {&D_002BAAE8[90], 95, 96, sizeof(CLUE), (PFNNEW)NewClue}, /* Clue */
    {&D_002BAAE8[48], 96, 97, sizeof(CID96), (PFNNEW)NewCid96}, /* Cid96 */
    {&D_002BAAE8[48], 97, 98, sizeof(SKY), (PFNNEW)NewSky}, /* Sky */
    {&D_002BAAE8[48], 98, 99, sizeof(CID98), (PFNNEW)NewCid98}, /* Cid98 */
    {&D_002BAAE8[48], 99, 100, sizeof(CID99), (PFNNEW)NewCid99}, /* Cid99 */
    {&D_002BAAE8[48], 100, 103, sizeof(CID100), (PFNNEW)NewCid100}, /* Cid100 */
    {&D_002BAAE8[100], 101, 102, sizeof(CID101), (PFNNEW)NewCid101}, /* Cid101 */
    {&D_002BAAE8[100], 102, 103, sizeof(CID102), (PFNNEW)NewCid102}, /* Cid102 */
    {&D_002BAAE8[48], 103, 104, sizeof(BOOTY), (PFNNEW)NewBooty}, /* Booty */
    {&D_002BAAE8[48], 104, 105, sizeof(CID104), (PFNNEW)NewCid104}, /* Cid104 */
    {&D_002BAAE8[48], 105, 106, sizeof(CID105), (PFNNEW)NewCid105}, /* Cid105 */
    {&D_002BAAE8[48], 106, 107, sizeof(CID106), (PFNNEW)NewCid106}, /* Cid106 */
    {&D_002BAAE8[48], 107, 108, sizeof(WAYPT), (PFNNEW)NewWaypt}, /* Waypt */
    {&D_002BAAE8[48], 108, 109, sizeof(CID108), (PFNNEW)NewCid108}, /* Cid108 */
    {&D_002BAAE8[2], 109, 111, sizeof(ASEG), (PFNNEW)NewAseg}, /* Aseg */
    {&D_002BAAE8[109], 110, 111, sizeof(ASEGBL), (PFNNEW)NewAsegbl}, /* Asegbl */
    {&D_002BAAE8[2], 111, 112, sizeof(SM), (PFNNEW)NewSm}, /* Sm */
    {&D_002BAAE8[2], 112, 113, sizeof(CID112), (PFNNEW)NewCid112}, /* Cid112 */
    {&D_002BAAE8[2], 113, 114, sizeof(CID113), (PFNNEW)NewCid113}, /* Cid113 */
    {&D_002BAAE8[2], 114, 115, sizeof(RELAY), (PFNNEW)NewRelay}, /* Relay */
    {&D_002BAAE8[2], 115, 116, sizeof(WR), (PFNNEW)NewWr}, /* Wr */
    {&D_002BAAE8[2], 116, 117, sizeof(CID116), (PFNNEW)NewCid116}, /* Cid116 */
    {&D_002BAAE8[2], 117, 118, sizeof(CID117), (PFNNEW)NewCid117}, /* Cid117 */
    {&D_002BAAE8[2], 118, 119, sizeof(CID118), (PFNNEW)NewCid118}, /* Cid118 */
    {&D_002BAAE8[2], 119, 120, sizeof(CID119), (PFNNEW)NewCid119}, /* Cid119 */
    {&D_002BAAE8[2], 120, 121, sizeof(CID120), (PFNNEW)NewCid120}, /* Cid120 */
    {&D_002BAAE8[2], 121, 122, sizeof(CID121), (PFNNEW)NewCid121}, /* Cid121 */
    {&D_002BAAE8[2], 122, 123, sizeof(CID122), (PFNNEW)NewCid122}, /* Cid122 */
    {&D_002BAAE8[2], 123, 124, sizeof(CID123), (PFNNEW)NewCid123}, /* Cid123 */
    {&D_002BAAE8[2], 124, 125, sizeof(PATHLINK), (PFNNEW)NewPathlink}, /* Pathlink */
    {&D_002BAAE8[2], 125, 126, sizeof(CID125), (PFNNEW)NewCid125}, /* Cid125 */
    {&D_002BAAE8[2], 126, 132, sizeof(WIDGET), (PFNNEW)NewWidget}, /* Widget */
    {&D_002BAAE8[126], 127, 128, sizeof(WIDGET127), (PFNNEW)NewWidget127}, /* Widget127 */
    {&D_002BAAE8[126], 128, 129, sizeof(WIDGET128), (PFNNEW)NewWidget128}, /* Widget128 */
    {&D_002BAAE8[126], 129, 130, sizeof(WIDGET129), (PFNNEW)NewWidget129}, /* Widget129 */
    {&D_002BAAE8[126], 130, 131, sizeof(WIDGET130), (PFNNEW)NewWidget130}, /* Widget130 */
    {&D_002BAAE8[126], 131, 132, sizeof(WIDGET131), (PFNNEW)NewWidget131}, /* Widget131 */
    {&D_002BAAE8[2], 132, 133, sizeof(JOB), (PFNNEW)NewJob}, /* Job */
    {&D_002BAAE8[2], 133, 134, sizeof(TASK), (PFNNEW)NewTask}, /* Task */
    {&D_002BAAE8[0], 134, 135, sizeof(ASEGA), 0}, /* Asega */
    {&D_002BAAE8[0], 135, 136, sizeof(CM), (PFNNEW)NewCm}, /* Cm */
    {&D_002BAAE8[0], 136, 137, sizeof(SMA), (PFNNEW)NewSma}, /* Sma */
    {&D_002BAAE8[0], 137, 138, sizeof(POCTL), 0}, /* Poctl */
    {&D_002BAAE8[0], 138, 145, sizeof(SAA), 0}, /* Saa */
    {&D_002BAAE8[138], 139, 140, sizeof(LOOP), (PFNNEW)NewLoop}, /* Loop */
    {&D_002BAAE8[138], 140, 141, sizeof(PINGPONG), (PFNNEW)NewPingpong}, /* Pingpong */
    {&D_002BAAE8[138], 141, 142, sizeof(SHUFFLE), (PFNNEW)NewShuffle}, /* Shuffle */
    {&D_002BAAE8[138], 142, 143, sizeof(EYES), (PFNNEW)NewEyes}, /* Eyes */
    {&D_002BAAE8[138], 143, 144, sizeof(SCROLLER), (PFNNEW)NewScroller}, /* Scroller */
    {&D_002BAAE8[138], 144, 145, sizeof(CIRCLER), (PFNNEW)NewCircler}, /* Circler */
    {&D_002BAAE8[0], 145, 146, sizeof(LEAP), (PFNNEW)NewLeap}, /* Leap */
    {&D_002BAAE8[0], 146, 147, sizeof(CID146), (PFNNEW)NewCid146}, /* Cid146 */
    {&D_002BAAE8[0], 147, 148, sizeof(CID147), 0}, /* Cid147 */
    {&D_002BAAE8[0], 148, 151, sizeof(CID148), 0}, /* Cid148 */
    {&D_002BAAE8[148], 149, 150, sizeof(CID149), 0}, /* Cid149 */
    {&D_002BAAE8[148], 150, 151, sizeof(CID150), 0}, /* Cid150 */
    {&D_002BAAE8[0], 151, 152, sizeof(CID151), 0}, /* Cid151 */
    {&D_002BAAE8[0], 152, 153, sizeof(CID152), 0}, /* Cid152 */
    {&D_002BAAE8[0], 153, 154, sizeof(JSG), (PFNNEW)NewJsg}, /* Jsg */
    {&D_002BAAE8[0], 154, 155, sizeof(CID154), (PFNNEW)NewCid154}, /* Cid154 */
    {&D_002BAAE8[0], 155, 199, sizeof(BHV), (PFNNEW)NewBhv}, /* Bhv */
    {&D_002BAAE8[155], 156, 157, sizeof(BHVGUARD), (PFNNEW)NewBhvguard}, /* Bhvguard */
    {&D_002BAAE8[155], 157, 164, sizeof(BHVANIM), (PFNNEW)NewBhvanim}, /* Bhvanim */
    {&D_002BAAE8[157], 158, 159, sizeof(BHVIDLE), (PFNNEW)NewBhvidle}, /* Bhvidle */
    {&D_002BAAE8[157], 159, 160, sizeof(BHVSTRIKE), (PFNNEW)NewBhvstrike}, /* Bhvstrike */
    {&D_002BAAE8[157], 160, 164, sizeof(BHVHIT), (PFNNEW)NewBhvhit}, /* Bhvhit */
    {&D_002BAAE8[160], 161, 162, sizeof(BHV161), (PFNNEW)NewBhv161}, /* Bhv161 */
    {&D_002BAAE8[160], 162, 163, sizeof(BHV162), (PFNNEW)NewBhv162}, /* Bhv162 */
    {&D_002BAAE8[160], 163, 164, sizeof(BHVKO), (PFNNEW)NewBhvko}, /* Bhvko */
    {&D_002BAAE8[155], 164, 165, sizeof(BHVATTACK), (PFNNEW)NewBhvattack}, /* Bhvattack */
    {&D_002BAAE8[155], 165, 166, sizeof(BHVCOMBAT), (PFNNEW)NewBhvcombat}, /* Bhvcombat */
    {&D_002BAAE8[155], 166, 169, sizeof(BHVPATH), (PFNNEW)NewBhvpath}, /* Bhvpath */
    {&D_002BAAE8[166], 167, 168, sizeof(BHVWALKPATH), (PFNNEW)NewBhvwalkpath}, /* Bhvwalkpath */
    {&D_002BAAE8[166], 168, 169, sizeof(BHVWALKGOAL), (PFNNEW)NewBhvwalkgoal}, /* Bhvwalkgoal */
    {&D_002BAAE8[155], 169, 170, sizeof(BHV169), (PFNNEW)NewBhv169}, /* Bhv169 */
    {&D_002BAAE8[155], 170, 171, sizeof(BHVPATROL), (PFNNEW)NewBhvpatrol}, /* Bhvpatrol */
    {&D_002BAAE8[155], 171, 172, sizeof(BHV171), (PFNNEW)NewBhv171}, /* Bhv171 */
    {&D_002BAAE8[155], 172, 173, sizeof(BHVLEAP), (PFNNEW)NewBhvleap}, /* Bhvleap */
    {&D_002BAAE8[155], 173, 174, sizeof(BHVSHOOT), (PFNNEW)NewBhvshoot}, /* Bhvshoot */
    {&D_002BAAE8[155], 174, 175, sizeof(BHV174), (PFNNEW)NewBhv174}, /* Bhv174 */
    {&D_002BAAE8[155], 175, 176, sizeof(BHV175), (PFNNEW)NewBhv175}, /* Bhv175 */
    {&D_002BAAE8[155], 176, 177, sizeof(BHV176), (PFNNEW)NewBhv176}, /* Bhv176 */
    {&D_002BAAE8[155], 177, 178, sizeof(BHVJUGGLED), (PFNNEW)NewBhvjuggled}, /* Bhvjuggled */
    {&D_002BAAE8[155], 178, 179, sizeof(BHV178), (PFNNEW)NewBhv178}, /* Bhv178 */
    {&D_002BAAE8[155], 179, 180, sizeof(BHV179), (PFNNEW)NewBhv179}, /* Bhv179 */
    {&D_002BAAE8[155], 180, 181, sizeof(BHV180), (PFNNEW)NewBhv180}, /* Bhv180 */
    {&D_002BAAE8[155], 181, 182, sizeof(BHV181), (PFNNEW)NewBhv181}, /* Bhv181 */
    {&D_002BAAE8[155], 182, 183, sizeof(BHV182), (PFNNEW)NewBhv182}, /* Bhv182 */
    {&D_002BAAE8[155], 183, 184, sizeof(BHV183), (PFNNEW)NewBhv183}, /* Bhv183 */
    {&D_002BAAE8[155], 184, 185, sizeof(BHVRAIL), (PFNNEW)NewBhvrail}, /* Bhvrail */
    {&D_002BAAE8[155], 185, 186, sizeof(BHVPIPE), (PFNNEW)NewBhvpipe}, /* Bhvpipe */
    {&D_002BAAE8[155], 186, 187, sizeof(BHVGOTO), (PFNNEW)NewBhvgoto}, /* Bhvgoto */
    {&D_002BAAE8[155], 187, 188, sizeof(BHV187), (PFNNEW)NewBhv187}, /* Bhv187 */
    {&D_002BAAE8[155], 188, 189, sizeof(BHV188), (PFNNEW)NewBhv188}, /* Bhv188 */
    {&D_002BAAE8[155], 189, 190, sizeof(BHV189), (PFNNEW)NewBhv189}, /* Bhv189 */
    {&D_002BAAE8[155], 190, 191, sizeof(BHVPATHLINK), (PFNNEW)NewBhvpathlink}, /* Bhvpathlink */
    {&D_002BAAE8[155], 191, 192, sizeof(BHV191), (PFNNEW)NewBhv191}, /* Bhv191 */
    {&D_002BAAE8[155], 192, 193, sizeof(BHVPATROLPATH), (PFNNEW)NewBhvpatrolpath}, /* Bhvpatrolpath */
    {&D_002BAAE8[155], 193, 194, sizeof(BHV193), (PFNNEW)NewBhv193}, /* Bhv193 */
    {&D_002BAAE8[155], 194, 195, sizeof(BHVFOLLOW), (PFNNEW)NewBhvfollow}, /* Bhvfollow */
    {&D_002BAAE8[155], 195, 196, sizeof(BHV195), (PFNNEW)NewBhv195}, /* Bhv195 */
    {&D_002BAAE8[155], 196, 197, sizeof(BHV196), (PFNNEW)NewBhv196}, /* Bhv196 */
    {&D_002BAAE8[155], 197, 198, sizeof(BHV197), (PFNNEW)NewBhv197}, /* Bhv197 */
    {&D_002BAAE8[155], 198, 199, sizeof(BHV198), (PFNNEW)NewBhv198}, /* Bhv198 */
};
}
