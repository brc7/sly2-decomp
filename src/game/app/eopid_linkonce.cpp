// eopid_linkonce.cpp -- the address references that make g++ emit the out-of-line copies of inline option accessors.
//
// Not an original TU. The option table g_aeopid (D_002AC880: 0x28-byte EOPID records in crt0_splicewrap's .data)
// stores option accessors as pointers to member functions, {0xFFFF0000, pfn} (delta 0, index -1: non-virtual): get at
// +0x8, set at +0x10, setUser at +0x18, ensure at +0x20. Many of them are inline functions defined in the class
// headers; only the table takes their address, and that is what made g++ emit them out of line (in
// .gnu.linkonce.t.<symbol>; the linker keeps one copy and places it in the linkonce area 0x254F00..0x259DE4).
//
// g_aeopid is data and is not in the source, so this file takes the same addresses, one SLY2_EOPID_REF per accessor
// (the variables are data; tools/matchtool.py and tools/progress.py compare every FUNC symbol of the object, the
// weak linkonce ones included). The owner class of an accessor is the root-most class whose option array in
// g_mpcidarysplopt (D_00279AD0) lists the record. See docs/guides/tu-merging.md "Inline functions and linkonce copies".
#include "sly2/cm.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/xfm.h"
#include "sly2/stepplayer.h"
#include "sly2/guard.h"

#define SLY2_EOPID_REF(addr, pfn) __typeof__(pfn) g_eopidref_##addr = pfn;

// CM
SLY2_EOPID_REF(00256298, &func_00256298)   // EOPID 0x2AE338 get
SLY2_EOPID_REF(002562A0, &func_002562A0)   // EOPID 0x2AE360 get
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
