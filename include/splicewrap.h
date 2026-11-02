/* Script ("splice") binding wrappers. Each wrapper has the C++ shape
 *     CRef Wrapper(BASIC *pbasic, int cref, CRef *aref)
 * which g++ 2.95 compiles as  CRef *Wrapper(CRef *ret, BASIC *pbasic, int cref, CRef *aref).
 * Written here in C with the constructor/destructor calls spelled out. */
#ifndef SPLICEWRAP_H
#define SPLICEWRAP_H
#include "m2c_prelude.h"
typedef struct CRef { s32 m_tagk; s32 m_tag; } CRef;
typedef s32 OTYP;                                        /* splice argument type code */
/* CRef RefSetArgsFromSplice(int cref, CRef *aref, OTYP *aotyp, ...): checks the arg count/types and
 * stores each decoded argument through the trailing pointers. */
void RefSetArgsFromSplice__FiP4CRefP4OTYPe(CRef *ret, s32 cref, CRef *aref, OTYP *aotyp, ...);
void Decref__4CRef(CRef *pref);                          /* 0x129EC8 (inlined ~CRef) */
void SetTag__4CRef4TAGK(CRef *pref, s32 tagk);               /* 0x12A0B0 */
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);         /* 0x1291A8 copy ctor */
#define TAGK_Void 0x24
#endif
