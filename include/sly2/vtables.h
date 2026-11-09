/* include/sly2/vtables.h -- the vtable symbols that matched code names by hand, declared once, and the vptr-store
 * macros of the C-named constructors.
 *
 * A constructor the original compiled inline but this decomp writes as a C function (its asm label is the address
 * name, it runs before its class's virtuals are declared, or it is called through a view) cannot let g++ store the
 * vptr: it stores the vtable's address itself. g++ 2.95 names a class's vtable _vt$<length><name> (with -fno-rtti, one
 * table per class; data/vtables emits them in their key TUs, see docs/guides/tu-merging.md "Emitting vtables").
 *
 *   SLY2_VTBL(CLASS)                 the vtable of CLASS, as an array of VTE (the table's first entry is slot 0)
 *   SLY2_SET_VPTR(p, CLASS)          p's vptr at +0xC (BASIC's, so every registry class's) = CLASS's vtable
 *   SLY2_SET_VPTR_AT(p, off, CLASS)  the same for a class whose vptr is elsewhere (ACT +0x18, a RIP's part +0xEC...)
 *
 * The declarations only name symbols: an unused one emits nothing (no undefined symbol, no data), and none of them
 * defines an object, so including this header moves nothing in .bss or .data. A TU that also emits one of these
 * vtables (its class's key TU) refers to its own weak copy through the same name. The stores are plain word stores of
 * the table's address (lui/addiu, sw), byte-identical to the per-file forms they replace (member stores, PVT macros,
 * `*(void **)((char *)p + 0xC) = ...`). The addresses in the comments are the tables' places in the ELF. */
#ifndef SLY2_VTABLES_H
#define SLY2_VTABLES_H
#include "sly2/basic.h"

#define SLY2_VTBL(CLASS) vtbl_##CLASS
#define SLY2_DECLARE_VTBL(LEN, CLASS) extern VTE SLY2_VTBL(CLASS)[] __asm__("_vt$" #LEN #CLASS)
#define SLY2_SET_VPTR_AT(p, off, CLASS) (*(VTE **)((char *)(p) + (off)) = SLY2_VTBL(CLASS))
#define SLY2_SET_VPTR(p, CLASS) SLY2_SET_VPTR_AT(p, 0xC, CLASS)

SLY2_DECLARE_VTBL(4, ACT1);         /* 0x2CA820 */
SLY2_DECLARE_VTBL(4, ACT2);         /* 0x2CA7E0 */
SLY2_DECLARE_VTBL(4, ACT3);         /* 0x2DB338 */
SLY2_DECLARE_VTBL(4, ACT4);         /* 0x2CA7A0 */
SLY2_DECLARE_VTBL(4, ACT9);         /* 0x2CA6A0 */
SLY2_DECLARE_VTBL(6, ACTSEG);       /* 0x2CAF08 */
SLY2_DECLARE_VTBL(5, ASEGA);        /* 0x2CAF48 */
SLY2_DECLARE_VTBL(7, BANKSND);      /* 0x2D9C70 */
SLY2_DECLARE_VTBL(6, BHV161);       /* 0x2D0930 */
SLY2_DECLARE_VTBL(6, BHV162);       /* 0x2D0830 */
SLY2_DECLARE_VTBL(6, BHV169);       /* 0x2CF7F8 */
SLY2_DECLARE_VTBL(6, BHV171);       /* 0x2CFA10 */
SLY2_DECLARE_VTBL(6, BHV174);       /* 0x2D4AF8 */
SLY2_DECLARE_VTBL(6, BHV175);       /* 0x2D02A0 */
SLY2_DECLARE_VTBL(6, BHV176);       /* 0x2D0420 */
SLY2_DECLARE_VTBL(6, BHV178);       /* 0x2D01A0 */
SLY2_DECLARE_VTBL(6, BHV179);       /* 0x2D0120 */
SLY2_DECLARE_VTBL(6, BHV180);       /* 0x2CDDC0 */
SLY2_DECLARE_VTBL(6, BHV181);       /* 0x2CDD40 */
SLY2_DECLARE_VTBL(6, BHV182);       /* 0x2CCE90 */
SLY2_DECLARE_VTBL(6, BHV183);       /* 0x2D00A0 */
SLY2_DECLARE_VTBL(6, BHV187);       /* 0x2CFF20 */
SLY2_DECLARE_VTBL(6, BHV189);       /* 0x2CFD20 */
SLY2_DECLARE_VTBL(6, BHV191);       /* 0x2CFC20 */
SLY2_DECLARE_VTBL(6, BHV193);       /* 0x2CC440 */
SLY2_DECLARE_VTBL(6, BHV195);       /* 0x2CC340 */
SLY2_DECLARE_VTBL(6, BHV196);       /* 0x2CC2C0 */
SLY2_DECLARE_VTBL(6, BHV197);       /* 0x2CC240 */
SLY2_DECLARE_VTBL(6, BHV198);       /* 0x2CC1C0 */
SLY2_DECLARE_VTBL(7, BHVANIM);      /* 0x2D0AB0 */
SLY2_DECLARE_VTBL(9, BHVATTACK);    /* 0x2D0620 */
SLY2_DECLARE_VTBL(9, BHVCOMBAT);    /* 0x2D05A0 */
SLY2_DECLARE_VTBL(9, BHVFOLLOW);    /* 0x2CC3C0 */
SLY2_DECLARE_VTBL(7, BHVGOTO);      /* 0x2CFE20 */
SLY2_DECLARE_VTBL(8, BHVGUARD);     /* 0x2D04A0 */
SLY2_DECLARE_VTBL(7, BHVIDLE);      /* 0x2D0A30 */
SLY2_DECLARE_VTBL(10, BHVJUGGLED);  /* 0x2D03A0 */
SLY2_DECLARE_VTBL(5, BHVKO);        /* 0x2D08B0 */
SLY2_DECLARE_VTBL(7, BHVLEAP);      /* 0x2D0320 */
SLY2_DECLARE_VTBL(11, BHVPATHLINK); /* 0x2CFCA0 */
SLY2_DECLARE_VTBL(9, BHVPATROL);    /* 0x2D0520 */
SLY2_DECLARE_VTBL(13, BHVPATROLPATH); /* 0x2CFDA0 */
SLY2_DECLARE_VTBL(7, BHVPIPE);      /* 0x2CFFA0 */
SLY2_DECLARE_VTBL(7, BHVRAIL);      /* 0x2D0020 */
SLY2_DECLARE_VTBL(8, BHVSHOOT);     /* 0x2D0220 */
SLY2_DECLARE_VTBL(9, BHVSTRIKE);    /* 0x2D07B0 */
SLY2_DECLARE_VTBL(11, BHVWALKGOAL); /* 0x2CC4C0 */
SLY2_DECLARE_VTBL(11, BHVWALKPATH); /* 0x2D06A0 */
SLY2_DECLARE_VTBL(5, BLIPG);        /* 0x2CB5D8 */
SLY2_DECLARE_VTBL(6, CID146);       /* 0x2DA9D0 */
SLY2_DECLARE_VTBL(6, CID147);       /* 0x2CE2B8 */
SLY2_DECLARE_VTBL(6, CID149);       /* 0x2CA2C0 */
SLY2_DECLARE_VTBL(6, CID150);       /* 0x2CA280 */
SLY2_DECLARE_VTBL(6, CID151);       /* 0x2D9D00 */
SLY2_DECLARE_VTBL(6, CID152);       /* 0x2CA510 */
SLY2_DECLARE_VTBL(6, CID154);       /* 0x2CA490 */
SLY2_DECLARE_VTBL(19, CLoadCollectorArray); /* 0x2CBE50 */
SLY2_DECLARE_VTBL(2, CM);           /* 0x2CA4D0 */
SLY2_DECLARE_VTBL(14, CMemcardDialog); /* 0x2D3750 */
SLY2_DECLARE_VTBL(15, CMemInputStream); /* 0x2CA5A0 */
SLY2_DECLARE_VTBL(7, CMPART1);      /* 0x2CD528 */
SLY2_DECLARE_VTBL(7, CMPART2);      /* 0x2DB858 */
SLY2_DECLARE_VTBL(10, CPCAMBLEND);  /* 0x2CD408 */
SLY2_DECLARE_VTBL(8, CPCAMERA);     /* 0x2CAB90 */
SLY2_DECLARE_VTBL(6, CPHANG);       /* 0x2CD450 */
SLY2_DECLARE_VTBL(5, CPLCY);        /* 0x2CD570 */
SLY2_DECLARE_VTBL(6, CPLOOK);       /* 0x2CD4E0 */
SLY2_DECLARE_VTBL(8, CPTURRET);     /* 0x2CD3C0 */
SLY2_DECLARE_VTBL(9, CPTWOSHOT);    /* 0x2CD378 */
SLY2_DECLARE_VTBL(6, CPZOOM);       /* 0x2CD498 */
SLY2_DECLARE_VTBL(15, CQueueOutputIpu); /* 0x2D4220 */
SLY2_DECLARE_VTBL(10, CStreamBis);  /* 0x2D2918 */
SLY2_DECLARE_VTBL(7, CSwHeap);      /* 0x2CA578 */
SLY2_DECLARE_VTBL(6, FLYING);       /* 0x2D7738 */
SLY2_DECLARE_VTBL(10, GADGETMENU);  /* 0x2D8568 */
SLY2_DECLARE_VTBL(5, GLINT);        /* 0x2D75F8 */
SLY2_DECLARE_VTBL(8, HUDMETER);     /* 0x2D8668 */
SLY2_DECLARE_VTBL(7, JOBLIST);      /* 0x2D8468 */
SLY2_DECLARE_VTBL(3, JSG);          /* 0x2C94A0 */
SLY2_DECLARE_VTBL(4, LEAF);         /* 0x2D74B8 */
SLY2_DECLARE_VTBL(4, LEAP);         /* 0x2DAA10 */
SLY2_DECLARE_VTBL(5, MATCH);        /* 0x2D7648 */
SLY2_DECLARE_VTBL(9, PAUSEMENU);    /* 0x2D37D0 */
SLY2_DECLARE_VTBL(5, POCTL);        /* 0x2CA240 */
SLY2_DECLARE_VTBL(8, RELAYJSG);     /* 0x2D8C08 */
SLY2_DECLARE_VTBL(5, REMIT);        /* 0x2D7468 */
SLY2_DECLARE_VTBL(3, RIP);          /* 0x2D7788 */
SLY2_DECLARE_VTBL(4, RIPG);         /* 0x2D77D8 */
SLY2_DECLARE_VTBL(5, RIPT0);        /* 0x2D7508 */
SLY2_DECLARE_VTBL(5, RIPT1);        /* 0x2D7558 */
SLY2_DECLARE_VTBL(6, SCREEN);       /* 0x2D85E8 */
SLY2_DECLARE_VTBL(7, SCRN52D);      /* 0x2D83E8 */
SLY2_DECLARE_VTBL(8, SHRAPNEL);     /* 0x2D7698 */
SLY2_DECLARE_VTBL(3, SMA);          /* 0x2D9768 */
SLY2_DECLARE_VTBL(5, SMACK);        /* 0x2D75A8 */
SLY2_DECLARE_VTBL(2, SO);           /* 0x2D9978 */
SLY2_DECLARE_VTBL(9, STREAMSND);    /* 0x2D9BE0 */
SLY2_DECLARE_VTBL(5, STUCK);        /* 0x2D76E8 */
SLY2_DECLARE_VTBL(2, SW);           /* 0x2DC018 */
SLY2_DECLARE_VTBL(8, THIEFNET);     /* 0x2D84E8 */
SLY2_DECLARE_VTBL(5, TITLE);        /* 0x2D8368 */
SLY2_DECLARE_VTBL(6, WIDGET);       /* 0x2DAF30 */

#endif
