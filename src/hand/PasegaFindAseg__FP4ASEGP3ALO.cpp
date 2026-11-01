#include "sly2/aseg.h"
// Walks paseg->dlAsega (ASEG+0x50) through ASEGA::dleAseg (+0x2C) looking for the ASEGA bound to palo (ASEGA::paloRoot, +0x18).
struct DLI { void *m_pdl; void **m_ppv; int m_ibDle; DLI *m_pdliNext; };
ASEGA *PasegaFindAseg(ASEG *paseg, ALO *palo)
{
    DLI dli;
    ASEGA *p;
    for (p = (ASEGA *)paseg->dlAsega.head; p; p = (ASEGA *)p->dleAseg.next)
        if (p->paloRoot == palo) return p;
    return 0;
}
