// player_input.h -- named bits of the player controller's button word (POCTL grfbtn, PO+0x150 -> +0x30), as the
// player state machines test them (ConsumeButtonsPressedWithin / GetButtonsPressedWithin / ClearPoctlJoyPressed, `flags & ...`).
// Meanings are from runtime observation (docs/systems/player.md section 6); the physical buttons follow the PS2 pad
// bit order (byte-swapped libpad layout) and are a hypothesis for the gadget/look bits.
//
// Local to the player files for now: a later pass may move it into include/sly2/ next to POCTL. Enumerators are plain
// ints, so using them instead of the literals never changes code generation.
#ifndef SLY2_PLAYER_INPUT_H
#define SLY2_PLAYER_INPUT_H

enum PlayerButton {
    BUTTON_GADGET_1 = 0x01,   // gadget slot 1 (L2, inferred)
    BUTTON_GADGET_2 = 0x02,   // gadget slot 2 (R2, inferred)
    BUTTON_GADGET_0 = 0x04,   // gadget slot 0 (L1, inferred)
    BUTTON_RUN = 0x08,        // held = run, released = walk at full stick (R1, inferred)
    BUTTON_ALT_ATTACK = 0x10, // triangle: alternate attack / gadget use
    BUTTON_ACTION = 0x20,     // circle: action, grab, wall press, pick up
    BUTTON_JUMP = 0x40,       // cross: jump, pull up from a ledge
    BUTTON_ATTACK = 0x80,     // square: attack
    BUTTON_GADGET_MENU = 0x100, // opens the gadget menu (UpdateStepplayer; Select, inferred)
    BUTTON_HINT = 0x200,      // acknowledges the pending hint (AcknowledgeStepplayerHint; L3, inferred)
    BUTTON_LOOK = 0x400,      // look mode (binocucom, JTMODE kind 0) (R3, inferred)

    // Combinations the state machines test together.
    BUTTON_FACE = BUTTON_ALT_ATTACK | BUTTON_ACTION | BUTTON_JUMP | BUTTON_ATTACK,   // 0xF0: any face button
};

#endif // SLY2_PLAYER_INPUT_H
