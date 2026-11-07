/* include/sly2/eopid.h -- EOPID, one record (0x28 bytes) of the object option table g_aeopid (D_002AC880, in
 * crt0_splicewrap's .data): the option's type, its flags, and how to get, set and load it.
 *
 * Representation (docs/guides/tu-merging.md "Representation decisions"): the getter, setter and file words are
 * genuinely unions: grfeopid says whether a word is a byte offset (bit 2 get / bit 0x10 set: the member is read or
 * written directly), a Splice thunk (bit 1: a method), or a pointer to member function, and otyp picks the PMF's
 * signature. The typed PMF members are the superset of the per-function views (spliceobj's getter and setter twins,
 * brx's loader): calling `(pobj->*peopid->pmfGetM)()` without a cast is what the matched code does (a cast of pmfGet
 * gives the same code except two delay-slot hoists, 99.2%). */
#ifndef SLY2_EOPID_H
#define SLY2_EOPID_H
#include "sly2/basic.h"     /* OTYP, GRFEOPID, PFNTHUNK, BASIC, MATRIX3, CLQ, LM, SMP, qword */
#include "sly2/cref.h"

typedef void (BASIC::*PMF)();

/* SETTER: how an option is written: a byte offset into the object, a pointer to member function or a Splice thunk
   (EOPID::grfeopid says which). */
union SETTER
{
      int ibSet;
      PMF pmfSet;
      PFNTHUNK pfnthunk;
      void (BASIC::*pmfSetN)(int);
};

/* EOPID: one object option: how scripts and the level loader read and write a named property of an object. */
struct EOPID
{
      OTYP otyp;                /* +0x00 0 bool, 1 float, 2 vector, 4 matrix, 9 clq, 10 lm, 11 smp, 5..8/13..17/19/20/23 int;
                                   flag bits 0x1000 int, 0x2000 BASIC *, 0x8000 CRef */
      GRFEOPID grfeopid;        /* +0x04 1 method, 2 read directly, 0x10 written directly, 0x2000 object through pmfObj */
      union                     /* +0x08 get */
      {
            int ib;
            PFNTHUNK pfnthunkDeferred;
            PMF pmfGet;
            long (BASIC::*pmfGetB)();
            float (BASIC::*pmfGetF)();
            qword (BASIC::*pmfGetQ)();
            MATRIX3 *(BASIC::*pmfGetM)();
            int (BASIC::*pmfGetN)();
            void (BASIC::*pmfGetClq)(CLQ *);
            void (BASIC::*pmfGetLm)(LM *);
            void (BASIC::*pmfGetSmp)(SMP *);
            CRef (BASIC::*pmfGetRef)();
            BASIC *(BASIC::*pmfGetPo)();
      };
      union                     /* +0x10 set */
      {
            int ibSet;
            PMF pmfSet;
            PFNTHUNK pfnthunk;
            void (BASIC::*pmfSetB)(int);    /* int, not long: a DImode argument stops the arms' shared tail (SetOptionRecordValue) */
            void (BASIC::*pmfSetF)(float);
            void (BASIC::*pmfSetQ)(qword);
            void (BASIC::*pmfSetN)(int);
            void (BASIC::*pmfSetM)(MATRIX3 *);
            void (BASIC::*pmfSetClq)(CLQ *);
            void (BASIC::*pmfSetLm)(LM *);
            void (BASIC::*pmfSetSmp)(SMP *);
            void (BASIC::*pmfSetPo)(BASIC *);
      };
      union { SETTER setFile; int ibFile; int crefReq; };   /* +0x18 load from the level file; a method's argument count */
      void *(BASIC::*pmfObj)(int);                          /* +0x20 the object the option lives in (grfeopid 0x2000) */
};
typedef char assert_EOPID_size[sizeof(EOPID) == 0x28 ? 1 : -1];

/* The record as GetOptionRecordValue / SetOptionRecordValue take it (the Sly 2 symbols name PVREC: RefSetPeopid__FPvP5PVRECP4CRef). */
struct PVREC : public EOPID
{
};

#endif
