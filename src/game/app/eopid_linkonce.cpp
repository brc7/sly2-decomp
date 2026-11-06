// eopid_linkonce.cpp -- the address references that make g++ emit the out-of-line copies of inline option accessors.
//
// Not an original TU. The option table g_aeopid (D_002AC880: 0x28-byte EOPID records in crt0_splicewrap's .data)
// stores option accessors as pointers to member functions, {0xFFFF0000, pfn} (delta 0, index -1: non-virtual): get at
// +0x8, set at +0x10, setUser at +0x18, ensure at +0x20. Many of them are inline functions defined in the class
// headers; only the table takes their address, and that is what made g++ emit them out of line (in
// .gnu.linkonce.t.<symbol>; the linker keeps one copy and places it in the linkonce area 0x254F00..0x259DE4).
//
// g_aeopid (class_registry_options.inc) names them through asm labels that do not emit them; this file takes the same addresses, one SLY2_EOPID_REF per accessor
// (the variables are data; tools/matchtool.py and tools/progress.py compare every FUNC symbol of the object, the
// weak linkonce ones included). The owner class of an accessor is the root-most class whose option array in
// g_mpcidarysplopt (D_00279AD0) lists the record. The EMITTER and EXPLO accessors are function templates
// (sly2/emitter.h): taking the address of each instance emits it the same way. See docs/guides/tu-merging.md "Inline
// functions and linkonce copies".
#include "sly2/cm.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/xfm.h"
#include "sly2/stepplayer.h"
#include "sly2/guard.h"
#include "sly2/emitter.h"

#define SLY2_EOPID_REF(addr, pfn) __typeof__(pfn) g_eopidref_##addr = pfn;
// The same for a template instance (g++ 2.95 cannot take __typeof__ of a template-id's address): the pointer type is
// spelled out, and the template argument is deduced from it.
#define SLY2_EOPID_TREF(addr, ret, fn, params) ret (*g_eopidref_##addr) params = &fn;

// CM
SLY2_EOPID_REF(00256298, &func_00256298)   // EOPID 0x2AE338 get
SLY2_EOPID_REF(002562A0, &func_002562A0)   // EOPID 0x2AE360 get
// CID150 (the SOPART handle): records 1234, 1233, 1232
SLY2_EOPID_REF(002562B8, &func_002562B8)   // set
SLY2_EOPID_REF(002562E0, &func_002562E0)   // get
SLY2_EOPID_REF(002562F0, &func_002562F0)   // set
SLY2_EOPID_REF(00256318, &func_00256318)   // get
SLY2_EOPID_REF(00256328, &func_00256328)   // set
SLY2_EOPID_REF(00256350, &func_00256350)   // get
// STEPPLAYER
SLY2_EOPID_REF(00258280, &func_00258280)   // get
// SHAPE/XFM/VOL/TARGET-derived classes: ensure (the class's own fields)
SLY2_EOPID_REF(002599F8, &func_002599F8)   // CID12
SLY2_EOPID_REF(00259A08, &func_00259A08)   // CID34
SLY2_EOPID_REF(00259A18, &func_00259A18)   // CID38
SLY2_EOPID_REF(00259A28, &func_00259A28)   // BARREL
SLY2_EOPID_REF(00259A38, &func_00259A38)   // CID35
SLY2_EOPID_REF(00259A48, &func_00259A48)   // CID27
SLY2_EOPID_REF(00259A78, &func_00259A78)   // HND
// LO
SLY2_EOPID_REF(00259AE8, &func_00259AE8)   // get
SLY2_EOPID_REF(00259B00, &func_00259B00)   // PIPE ensure
SLY2_EOPID_REF(00259B10, &func_00259B10)   // RAIL ensure
// SO
SLY2_EOPID_REF(00259B20, &func_00259B20)   // get
SLY2_EOPID_REF(00259C10, &func_00259C10)   // CID39 ensure
// STEPGUARD (its detector's volumes)
SLY2_EOPID_REF(00259D38, &func_00259D38)   // set (and setUser)
SLY2_EOPID_REF(00259D48, &func_00259D48)   // set (and setUser)
SLY2_EOPID_REF(00259D60, &func_00259D60)   // get
SLY2_EOPID_REF(00259D70, &func_00259D70)   // get
// EMITTER and EXPLO: one template per accessor (sly2/emitter.h), instantiated for each class; the EXPLO copy of
// func_00258898<T> is 0x259330, and so on in the same order
// EMITTER (0x258898..0x258EC8)
SLY2_EOPID_TREF(00258898, EMITV *, func_00258898, (EMITTER *, ENSK))     // ensure
SLY2_EOPID_TREF(00258920, EMITV *, func_00258920, (EMITTER *, ENSK))     // ensure
SLY2_EOPID_TREF(002589A8, void, func_002589A8, (EMITTER *, int))         // setUser
SLY2_EOPID_TREF(002589F0, int, func_002589F0, (EMITTER *))               // get
SLY2_EOPID_TREF(00258A00, void, func_00258A00, (EMITTER *, int))         // setUser
SLY2_EOPID_TREF(00258AB8, int, func_00258AB8, (EMITTER *))               // get
SLY2_EOPID_TREF(00258AC8, int *, func_00258AC8, (EMITTER *, ENSK))       // ensure
SLY2_EOPID_TREF(00258B00, int, func_00258B00, (EMITTER *, ENSK))         // ensure
SLY2_EOPID_TREF(00258B38, int *, func_00258B38, (EMITTER *, ENSK))       // ensure
SLY2_EOPID_TREF(00258B70, void, func_00258B70, (EMITTER *, int))         // setUser
SLY2_EOPID_TREF(00258C40, int, func_00258C40, (EMITTER *))               // get
SLY2_EOPID_TREF(00258C60, void *, func_00258C60, (EMITTER *, ENSK))      // ensure
SLY2_EOPID_TREF(00258CC8, void *, func_00258CC8, (EMITTER *, ENSK))      // ensure
SLY2_EOPID_TREF(00258D30, void *, func_00258D30, (EMITTER *, ENSK))      // ensure
SLY2_EOPID_TREF(00258D98, void, func_00258D98, (EMITTER *, int))         // set (and setUser)
SLY2_EOPID_TREF(00258E40, int, func_00258E40, (EMITTER *))               // get
SLY2_EOPID_TREF(00258E60, void *, func_00258E60, (EMITTER *, ENSK))      // ensure
SLY2_EOPID_TREF(00258EC8, void *, func_00258EC8, (EMITTER *, ENSK))      // ensure
// EXPLO (0x259330..0x259960; records owned by EXPLO or EXPLS : EXPLO)
SLY2_EOPID_TREF(00259330, EMITV *, func_00258898, (EXPLO *, ENSK))       // ensure
SLY2_EOPID_TREF(002593B8, EMITV *, func_00258920, (EXPLO *, ENSK))       // ensure
SLY2_EOPID_TREF(00259440, void, func_002589A8, (EXPLO *, int))           // setUser
SLY2_EOPID_TREF(00259488, int, func_002589F0, (EXPLO *))                 // get
SLY2_EOPID_TREF(00259498, void, func_00258A00, (EXPLO *, int))           // setUser
SLY2_EOPID_TREF(00259550, int, func_00258AB8, (EXPLO *))                 // get
SLY2_EOPID_TREF(00259560, int *, func_00258AC8, (EXPLO *, ENSK))         // ensure
SLY2_EOPID_TREF(00259598, int, func_00258B00, (EXPLO *, ENSK))           // ensure
SLY2_EOPID_TREF(002595D0, int *, func_00258B38, (EXPLO *, ENSK))         // ensure
SLY2_EOPID_TREF(00259608, void, func_00258B70, (EXPLO *, int))           // setUser
SLY2_EOPID_TREF(002596D8, int, func_00258C40, (EXPLO *))                 // get
SLY2_EOPID_TREF(002596F8, void *, func_00258C60, (EXPLO *, ENSK))        // ensure
SLY2_EOPID_TREF(00259760, void *, func_00258CC8, (EXPLO *, ENSK))        // ensure
SLY2_EOPID_TREF(002597C8, void *, func_00258D30, (EXPLO *, ENSK))        // ensure
SLY2_EOPID_TREF(00259830, void, func_00258D98, (EXPLO *, int))           // set (and setUser)
SLY2_EOPID_TREF(002598D8, int, func_00258E40, (EXPLO *))                 // get
SLY2_EOPID_TREF(002598F8, void *, func_00258E60, (EXPLO *, ENSK))        // ensure
SLY2_EOPID_TREF(00259960, void *, func_00258EC8, (EXPLO *, ENSK))        // ensure
