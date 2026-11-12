/* include/sly2/recon.h -- RECONSEL, Sly's recon selector (gameplay/player/recon.cpp): the photo prompt's candidate
 * targets (CID46 recon targets), their screen projection, the selection state machine and its widgets. It lives in
 * JT::reconSel; the mode HUD points at it (JTMODEHUD::reconSel). */
#ifndef SLY2_RECON_H
#define SLY2_RECON_H
#include "sly2/basic.h"
#include "sly2/port.h"   /* SLY2_EE_LAYOUT */

struct CID46; struct JTMODE; struct WIDGET; struct WIDGET130;

/* The recon selector (0xC4 bytes, 0xD0 with its alignment on the EE). */
struct RECONSEL
{
    int state;              /* +0x00 the selection state (-1 off; 8 / 9 the photo) */
    char pad4[0xC];
    QWROW origin;           /* +0x10 screen point of the prompt (x, y, z) */
    QWROW position;         /* +0x20 world position of the viewer (raised by 20) */
    int count;              /* +0x30 */
    CID46 *items[16];       /* +0x34 the recon targets */
    uint cPending;          /* +0x74 */
    CID46 *apending[5];     /* +0x78 */
    CID46 *actor;           /* +0x8C selected target */
    float proximity;        /* +0x90 */
    float sizeGain;         /* +0x94 */
    int rising;             /* +0x98 */
    float choiceTime;       /* +0x9C */
    uint savedFlags;        /* +0xA0 the player's tagged flag word while the photo is taken */
    void *resource;         /* +0xA4 */
    JTMODE *model;          /* +0xA8 the recon mode (its player owns the selector) */
    WIDGET130 *primary;     /* +0xAC the prompt (oid 0x362) */
    WIDGET *secondary;      /* +0xB0 oid 0x365, follows the target */
    int refreshPending;     /* +0xB4 */
    int visible;            /* +0xB8 */
    int selected;           /* +0xBC */
    int pending;            /* +0xC0 */
};
#if defined(SLY2_EE_LAYOUT)
typedef char assert_RECONSEL_size[sizeof(RECONSEL) == 0xD0 ? 1 : -1];
#endif

#endif /* SLY2_RECON_H */
