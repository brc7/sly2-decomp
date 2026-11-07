# Recipe: change player movement parameters

**Goal.** Make Sly, Bentley or Murray move differently: jump higher, run faster, fall slower, turn quicker, or add a
movement state.

**Systems.** [player.md](../systems/player.md) (read sections 3-5 and 10 first),
[physics-and-collision.md](../systems/physics-and-collision.md) section 4 (characters on the solver),
[animation.md](../systems/animation.md) (clips, root motion, blends). Mod build:
[README.md](README.md#how-a-mod-is-built).

## Where the numbers come from

Movement is split between **intent** (the hero's slot 16 `Update` picks a state, a target heading and a desired
velocity) and **motion** (the slot 70 physics preset turns that into acceleration during `SolveSw`). The parameters
live in four kinds of place, and which one you touch decides how hard the change is:

| Kind of parameter | Where | Example |
|---|---|---|
| **tuning constants** | file-scope `float`, `VECTOR4`, `LM`, `SMP` definitions in each player TU's data part, at the end of the file (still address-named `D_...`; their users' comments say what each is) | jump apex height, double-jump height, horizontal jump speed, launch speeds in `air.cpp`; scale factors in `jt.cpp` |
| **animation root motion** | the clips (ASEG) in the level data | ground speed: in many of Sly's ground states the desired velocity is read from the playing clip's root translation channel (`FindAsegChannel` in `UpdateJtPre`), or produced by the gait and directional blends (`UpdateGaitBlend`, `UpdateDirectionalAcceleration`) |
| **per-object physics state** | `SOPHYS` in `include/sly2/so.h` (gravity vector, velocities), STEP's fields (gravity ratio, spin/turn settings) | gravity, set and scaled through `SetStepGravityRatio` and the SO gravity options |
| **per-state collision part** | the phys part (an `SOPART`) chosen per state by slot 90 (`ApplyJtStateStepParams` -> `SetStepPhys`) | crouched versus standing collision shape |

Note that slot 90's "step parameters" are the collision part, not speeds.

## Recipe A: change a tuning constant (jump height, launch speeds)

1. Find the function that uses the number. For jumps: every launch goes through `SetJtAirJump` (`air.cpp`), which
   gets the vertical launch speed from `PlanJtLaunchVelocity`; that function's comment gives, per jump kind, how the
   speed is derived (most kinds solve for an apex height above the take-off point from the gravity; a few use fixed
   speeds). The heights and speeds are the constants it reads.
2. Find the constant's definition at the end of the same file ("Data owned by this TU") and change its initial value.
   Some numbers are literals inside functions instead (check the function body); changing those changes code.
3. Remember who else reads it: the same constant may be shared by several jump kinds or by the landing planner
   (`ChooseJtLanding`, the safe-place return), which predict where a jump lands. A higher jump with an unchanged planner
   can make auto-targeted jumps (to poles, ledges, rails) miss.

These constants are **data**, in writable memory: in a mod build a changed initial value changes only the data
section, not the code, so the object keeps its size and fits the matching layout (option 1 of
[README.md](README.md#how-a-mod-is-built)). You can also try a value live first: write the constant's address in
PCSX2 over PINE and jump (the next launch reads it).

## Recipe B: change ground speed

1. Identify the state: Sly's movement state (`JTS`) and the per-state switch in `UpdateJtPre` that computes the
   desired velocity for it. Murray and Bentley do the same in `UpdateMurrayStateMachine` / `UpdateBentleyStateMachine`
   and their update functions.
2. If the velocity comes from **root motion**, the clip decides the speed. Options: scale the computed velocity in that
   case of the switch (code change), speed the clip up (`SetAsegaSpeed`, which also speeds up the animation, so feet do
   not slide), or replace the clip in the level data.
3. If it comes from the **gait or directional blend**, the blend weights pick between clips of different speeds from
   the stick deflection; change the clips or scale the result.
4. If it comes from a **constant** (some sub-states multiply the stick amount by a speed constant), change the
   constant as in recipe A.

## Recipe C: gravity and air control

- Gravity is a per-object acceleration in `SOPHYS`; heroes scale it with `SetStepGravityRatio` (the air component
  saves and restores a changed scale, see `JTAIR`'s gravity fields). Jump apex heights are computed **from** gravity
  in `PlanJtLaunchVelocity`, so lowering gravity alone keeps the apex and makes jumps floatier, while changing an apex
  constant keeps the timing feel.
- Air steering is `PresetJtAccelJump` (spring toward the desired position from the stick); its stiffness and limits
  are constants of `air.cpp`.

## Recipe D: a new movement state

Follow player.md section 11 ("Add a new move for Sly"): a `JTS` value, entry and exit actions in `SetJtState`, a phys
part in `ApplyJtStateStepParams`, the transition into it (`ChooseJtGroundState` for grounded moves, a component, or
`HandleJtMessage`), its per-frame work in `UpdateJtPre` and `UpdateJt`, its physics in `PresetJtAccel`, and its gadget
rules. For a move shared by all three heroes, put it in a component and switch states through the generic player state
(slot 88).

## Gotchas

- **The match.** Any change here is a mod. Constant changes are the gentlest (data only, same size); new states change
  JT's layout (`JTS_Max` sizes a per-state array in JT).
- **Set states through the setters.** `SetJtState`, `SetMurrayState`, `SetBentleyState` and the components'
  `Set*State` swap animations and run exit actions; writing the state field skips them.
- **Intent versus motion.** Velocities written in `Update` are consumed by the slot 70 preset in `SolveSw`. Writing the
  SO velocity directly from `Update` fights the preset; teleport with `TranslateToPos` /
  `TranslateStepplayerToPos`.
- **Level design assumes the original numbers.** Gaps, ledge heights, rail spacing and guard sight ranges were built
  around the original jump and speed. Test in several levels.
- **Shared code.** The air, grab, hurt and pipe components run for all three heroes; a constant there changes Murray
  and Bentley too.
- **Frame-rate assumptions.** The clock clamps dt; movement integrates with it, but some smoothing uses per-frame
  constants (inferred). A port that changes the frame rate should re-check feel.

## How to test

- Live first: in PCSX2 over PINE, write the constant (recipe A) or the gravity ratio and try the move; this needs no
  build.
- In a mod build: jump on flat ground and measure the apex (the hero's world position over time), then try the
  auto-targeted moves (pole, ledge, rail landings) that depend on the landing planner, and a double jump.
- Check all three heroes when you touched a shared component.
