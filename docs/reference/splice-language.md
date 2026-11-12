# Splice language reference

A reference for people writing or reading Splice scripts: the values, the special forms, every builtin, how objects
expose options, and the events the engine sends. Everything here comes from the decompiled code (`src/game/splice/`,
`src/game/app/bif.cpp`, the option tables in `src/game/app/class_registry_options.inc`, `src/game/object/basic.cpp`).
Where the code and a source comment disagree, this page follows the code and says so.

Related pages: [systems/splice-scripting.md](../systems/splice-scripting.md) (how the VM works inside),
[formats/splice-scripts.md](../formats/splice-scripts.md) (the serialized form in world files),
[splice-options.md](splice-options.md) (generated per-class option table),
[splice-dump.md](splice-dump.md) (dumping the scripts of your own disc and compiling them back),
[guides/port-plan.md](../guides/port-plan.md#15-modding-and-extensions-with-splice) section 15 (modding plans).

**Scripts have no names on disc.** The game ships compiled scripts: lists of tagged values in which symbols are
numbers, builtins are table indices and options are integer ids. The spellings below are therefore labels: builtin
spellings come from source comments that carry the Sly 1 names where the slot is shared, and everything else is
written as a number or a code-derived label. Until a compiler exists (section 6), "writing" a script means writing
the S-expression that a future compiler would turn into those numbers.

## 1. Values and syntax

### 1.1 Value types

A value (`CRef`, `include/sly2/cref.h`) is a tag plus one 32-bit word. The tag numbers are those of the serialized
form.

| Tag | Type | Payload | Written as / made by |
|---|---|---|---|
| 0x15 | integer | signed 32-bit | `42`, `-1` |
| 0x16 | float | 32-bit float | `1.5` |
| 0x17 | boolean | word 0 or 1 | `#t`, `#f` |
| 0x18 | symbol | symbol id | `foo` (evaluated: variable lookup); `'foo` (the symbol itself) |
| 0x19 | builtin | builtin index | the builtin's name in operator position, or as a value (`(map abs xs)`) |
| 0x1A | vector | boxed {x, y, z} floats | `(vector x y z)` |
| 0x1B | matrix | boxed 4x4 floats | `(matrix ...)` |
| 0x1C | CLQ | boxed quadratic coefficients | `(clq a b c)` |
| 0x1D | LM | boxed {min, max} range | `(lm min max)` |
| 0x1E | SMP | boxed 3 smoothing floats | `(smp a b c)` |
| 0x1F | pair | a list cell (`CPair`) | `(a b c)`, `(list ...)`, `(cons a b)` |
| 0x20 | procedure | a closure (`CProc`) | `(lambda (params) body...)` |
| 0x21 | object | a `BASIC *` game object | only from builtins (`find-object`, `geto`, ...) |
| 0x22 | method | an object + native thunk (`CMethod`) | `(geto obj OPTION)` on a method option |
| 0x24 | void | none | the result of `set!`, `define`, `while`, `print`, ... |
| -1 | nil (untagged) | none | an unbound symbol, `(import ...)`, a failed lookup, the stub builtins |
| 0 | null / empty list | none | `'()`; the serialized "None" cell |

Notes:

- **Integers and floats.** Arithmetic keeps integers when every operand is an integer (but `+`, `-`, `*`, `/`
  accumulate in float, so integers above 2^24 lose precision). Most builtins coerce: a float argument accepts an
  integer and vice versa (truncating).
- **Booleans.** Only `#f` (tag 0x17, word 0) is false for `and`, `or`, `cond`, `not`, `while`. `if` is different:
  see section 2.
- **Boxed values** (vector, matrix, CLQ, LM, SMP) live in reference-counted per-type heaps. Values are copied by
  reference; no builtin mutates a box in place, so this is not observable from scripts.
- **Nil and null are different.** Tag -1 (an empty `CRef`) is what an unbound symbol evaluates to; tag 0 is the
  empty list (the code's `SetNil` makes tag 0). `null?` is true only for tag 0, so it does not detect an unbound
  variable; no type predicate tests for tag -1.
- **Lists** are proper lists of pairs; the empty list is the null value (tag 0). `list?` is true for both a pair and
  null; `null?` only for null. There is no dotted-pair syntax in compiled scripts (Sly 1's dot tag is rejected by the
  Sly 2 loader), so `cons` with a non-list second argument does not make an improper list (`cons` attaches the
  second argument only when it is a list).
- **Objects** have no literal form. The serialized object tag exists but every instance on disc is the null object.
  Objects are found at run time (section 4.4).
- **Symbols** are global small integers, the same in every level. Ids 0-28 are the event symbols (section 5); 30-989
  are named by the save-schema table in every level file (`murray`, `flags`, ...); the rest have no names on disc.
  1002, 990 and 1038 are the conventional first parameter of handlers ("self").

### 1.2 Vectors, matrices and the curve types

| Constructor | Arguments | Result |
|---|---|---|
| `(vector)` | none | (0, 0, 0) |
| `(vector s)` | one number | (s, s, s) |
| `(vector x y z)` | three numbers | (x, y, z). Any other count leaves the components uninitialised. |
| `(matrix)` | none | identity |
| `(matrix s)` | one number | uniform scale s (w stays 1) |
| `(matrix r0 r1 r2)` / `(matrix r0 r1 r2 r3)` | three or four **vectors** | rows copied (the rest of the identity kept) |
| `(matrix a b ... )` | 9 or 16 numbers | row-major 3x3 (rest identity) or full 4x4 |
| `(clq a b c)` | 3 numbers | the quadratic a + b·t + c·t² (`clq-evaluate`) |
| `(lm min max)` | 2 numbers | a closed range |
| `(smp a b c)` | 3 numbers | smoothing parameters (as the native SMP: used by options that smooth a value) |

`(get-element v i)` reads component i of a vector, CLQ, LM or SMP; `(get-element m i)` reads row i of a matrix as a
vector and `(get-element m i j)` one element.

Literal vectors and matrices exist in the serialized format (tags 26-30 with inline floats) but no retail script uses
them; scripts always call the constructor.

### 1.3 Syntax

Splice is written as Scheme-style S-expressions: `(operator operand ...)`, `'datum` for `(quote datum)`, `;` comments
(source-level only: nothing survives compilation). A file of level script is one `(begin ...)` "module" of top-level
`define`s (section 6).

## 2. Special forms

`EvalExpression` (`splice/eval.cpp`) evaluates atoms to themselves, looks symbols up through the frame chain (nil when
unbound) and dispatches a list on its head. These keywords are the whole set; anything else in head position is a
call.

| Tag | Form | Semantics |
|---|---|---|
| 1 | `(set! sym expr)` | Evaluates expr and stores it in the **existing** binding of sym, searching this frame and its parents. Silently does nothing when sym is unbound. Returns void. |
| 2 | `(define sym expr)` | Evaluates expr and adds a binding at the head of the **current** frame. Never checks for an existing binding: redefining shadows (the old binding stays in the list). Returns void. There is no `(define (f x) ...)` shorthand; write `(define f (lambda (x) ...))`. |
| 5 | `(assert expr msg)` | Evaluates expr; when it is false (word 0) prints "Splice assert: " and then msg through `print`, which the release build compiled out. Returns void. |
| 6 | (keyword unknown) | New in Sly 2: evaluates its first operand and returns it, or void with no operand. Never used by retail scripts. |
| 7 | `(if test then [else])` | Takes `then` only when the test's **word equals 1**, whatever the tag: `#t` and the integer 1 go to `then`; `#f`, 0, 2, a list or an object go to `else`. Returns void when the else branch is missing and taken. |
| 8 | `(or e ...)` | Returns the first value that is not `#f`; `#f` when all are (or there are none). |
| 9 | `(and e ...)` | Returns the first `#f`, else the last value (`#t` when empty). |
| 10 | `(cond (test body...) ... (else body...))` | Runs the body of the first clause whose test is not `#f` and returns its last value; a clause without body returns the test value. `else` is its own tag (11). Void when nothing matches. |
| 12 | `(case key ((datum ...) body...) ... (else body...))` | Evaluates key once and runs the first clause holding a datum `eqv?` to it. Data are literals, not evaluated. Void when nothing matches. |
| 13 | `(let ((sym val) ...) body...)` | Evaluates each val in the **outer** frame (there is no `let*`: a later val cannot see an earlier sym), binds them in a new child frame and evaluates the body there. With an empty binding list (`(let () ...)`) the body runs in the current frame, so a `define` inside leaks out. |
| 14 | `(while test body)` | Re-evaluates **one** body expression until test is `#f`; returns void. Wrap several expressions in `begin`. |
| 15 | `(lambda (p ... [. rest]) body...)` | A closure over the current frame. A dotted parameter collects the extra arguments as a list. |
| 16 | `(begin e ...)` | Evaluates in order in the current frame; returns the last value (void when empty). |
| 17 | `(import ...)` | Returns nil; nothing is imported. Unused by retail scripts. |
| 18 | `(quote datum)` | Returns the datum unevaluated. |

### 2.1 Calls

`(f a b ...)` evaluates the operator, then calls by its type:

- **builtin**: evaluates exactly *min* arguments (the builtin's minimum count) into an array; when the builtin takes
  rest arguments, the remaining operands are evaluated and passed as **one list** in the slot after the fixed ones
  (the empty list when there are none). So `(vector 1 2 3)` passes one list argument; the builtin sees the count.
- **method** (from `geto`): evaluates as many arguments as the method takes and calls the native thunk with the
  object; extra operands are ignored.
- **procedure**: a new frame whose single parent is the closure's captured frame; parameters are bound in order and
  the body runs there, returning its last value.

Argument counts are **not checked**. Missing arguments make the evaluator walk off the end of the operand list (a
crash in the original); extra arguments to a builtin without rest arguments, a method or a lambda without a dotted
parameter are silently ignored. At most 32 fixed arguments.

### 2.2 Environments

- Each script table entry is evaluated once into its own frame under the world's root frame; objects whose records
  name that entry all share that frame (systems guide section 3). A top-level `define` is therefore shared by every
  object using the script, and so is a run-time `define` in a handler.
- Frames have up to four parents, searched depth first; proxies add their source frames as parents.
- Per-object state goes into options or the object's sidebag (section 4.2), not into frame variables.
- Nothing survives a level change: frames, closures and lists are discarded with the world. Use save variables for
  persistent state.

### 2.3 Memory

Lists, closures, frames and bindings are reference counted, with a mark-and-sweep collector for cycles (a closure
capturing its own frame, the usual case for handlers). Scripts never free anything. The pools are fixed size per
level; very large lists built at run time can exhaust them (the original then grows a pool or fails).

## 3. Builtins

The table `g_mpbifkbif` in `src/game/app/bif.cpp` has 169 entries `{function, min, rest}`. **min** is the number of
fixed arguments; **rest** = 1 means any number of further arguments are accepted (passed as a list). Spellings:

- plain: from the comment on the builtin in `bif.cpp` (the Sly 1 name of a shared slot);
- `name~`: a label from the research listing (not an original name);
- `?`: no spelling known. Write `(builtin N)` style placeholders, or the C++ name, until one is chosen.

Types: *num* integer or float, *v* vector, *m* matrix, *obj* object, *oid* integer object id, *proc* lambda, *any*
any value; `[x]` optional (a rest argument).

### 3.1 Core (0-46)

| # | Spelling | min, rest | Arguments -> result | C++ function / notes |
|---|---|---|---|---|
| 0 | `+` | 1, 1 | num/v/m ... -> same | `BuiltinAdd`; vector and matrix addition too |
| 1 | `-` | 1, 1 | num/v/m ... -> same | `BuiltinSubtract` |
| 2 | `*` | 1, 1 | num, v·s, m·m ... -> product | `BuiltinMultiply` |
| 3 | `/` | 1, 1 | num ... -> quotient | `BuiltinDivide` |
| 4 | `print` | 1, 0 | any -> void | `BuiltinPrint`; body compiled out of release |
| 5 | `print-frame` | 0, 1 | -> void | `BuiltinPrintFrame`; compiled out |
| 6 | `print-sidebag` | 1, 0 | obj -> void | `BuiltinPrintSidebag`; compiled out |
| 7 | `=` | 2, 0 | any any -> bool | `BuiltinIntEqual`: same tag and same word (1 and 1.0 differ; boxed values compare by pointer) |
| 8 | `<` | 2, 0 | num num -> bool | `BuiltinLess` |
| 9 | `<=` | 2, 0 | num num -> bool | `BuiltinLessOrEqual` |
| 10 | `>` | 2, 0 | num num -> bool | `BuiltinGreater` |
| 11 | `>=` | 2, 0 | num num -> bool | `BuiltinGreaterOrEqual` |
| 12 | `eqv?` | 2, 0 | any any -> bool | `BuiltinEqv`: same tag and payload; vectors by value |
| 13 | `equal?` | 2, 0 | any any -> bool | `BuiltinEqual`: structural (recursive) |
| 14 | `boolean?` | 1, 0 | any -> bool | `BuiltinIsBoolean` |
| 15 | `number?` | 1, 0 | any -> bool | `BuiltinIsNum` |
| 16 | `integer?` | 1, 0 | any -> bool | `BuiltinIsInteger` |
| 17 | `float?` | 1, 0 | any -> bool | `BuiltinIsFloat` |
| 18 | `symbol?` | 1, 0 | any -> bool | `BuiltinIsSymbol` |
| 19 | `vector?` | 1, 0 | any -> bool | `BuiltinIsVector` |
| 20 | `matrix?` | 1, 0 | any -> bool | `BuiltinIsMatrix` |
| 21 | `clq?` | 1, 0 | any -> bool | `BuiltinIsClq` |
| 22 | `lm?` | 1, 0 | any -> bool | `BuiltinIsLm` |
| 23 | `smp?` | 1, 0 | any -> bool | `BuiltinIsSmp` |
| 24 | `list?` | 1, 0 | any -> bool | `BuiltinIsList`: pair or null |
| 25 | `null?` | 1, 0 | any -> bool | `BuiltinIsNull` |
| 26 | `object?` | 1, 0 | any -> bool | `BuiltinIsObject` (true for the null object too) |
| 27 | `null-obj?` | 1, 0 | any -> bool | `BuiltinIsNullObj`: an object value holding null |
| 28 | `method?` | 1, 0 | any -> bool | `BuiltinIsMethod` |
| 29 | `procedure?` | 1, 0 | any -> bool | `BuiltinIsProcedure`: lambda or builtin |
| 30 | `near?` | 3, 0 | num num eps -> bool | `BuiltinAreNear`: \|a - b\| within eps |
| 31 | `not` | 1, 0 | any -> bool | `BuiltinNot`: true only for `#f` |
| 32 | `cons` | 2, 0 | any list -> list | `BuiltinCons` |
| 33 | `car` | 1, 0 | list -> any | `BuiltinCar` (no check: null crashes) |
| 34 | `cdr` | 1, 0 | list -> list | `BuiltinCdr` (nil at the end) |
| 35 | `set-car!` | 2, 0 | list any -> void | `BuiltinSetCar` |
| 36 | `set-cdr!` | 2, 0 | list list -> void | `BuiltinSetCdr` |
| 37 | `length` | 1, 0 | list -> int | `BuiltinLength` |
| 38 | `nth` | 2, 0 | list int -> any | `BuiltinNth`, zero-based |
| 39 | `member?` | 2, 0 | any list -> bool | `BuiltinIsMember` (by `equal?`) |
| 40 | `list` | 0, 1 | any ... -> list | `BuiltinList` |
| 41 | `?` | 1, 0 | list -> list | `BuiltinShuffle`: random permutation that always moves the first element; used to reshuffle line lists |
| 42 | `append` | 0, 1 | list ... -> list | `BuiltinAppend` (copies all but the last) |
| 43 | `map` | 2, 0 | proc list -> list | `BuiltinMap` |
| 44 | `filter` | 2, 0 | proc list -> list | `BuiltinFilter` (keeps elements whose result is not `#f`) |
| 45 | `for-each` | 2, 0 | proc list -> void | `BuiltinForEach` |
| 46 | `eval` | 1, 0 | expr -> any | `BuiltinEval`: evaluates a quoted expression in the caller's frame |

### 3.2 Constructors and math (47-96)

| # | Spelling | min, rest | Arguments -> result | C++ function / notes |
|---|---|---|---|---|
| 47 | `vector` | 0, 1 | [num ...] -> v | `BuiltinVector`; section 1.2 |
| 48 | `matrix` | 0, 1 | [...] -> m | `BuiltinMatrix`; section 1.2 |
| 49 | `clq` | 3, 0 | num num num -> clq | `BuiltinClq` |
| 50 | `lm` | 2, 0 | num num -> lm | `BuiltinLm` |
| 51 | `smp` | 3, 0 | num num num -> smp | `BuiltinSmp` |
| 52 | `get-element` | 1, 1 | v/m/clq/lm/smp i [j] -> float or v | `BuiltinGetElement` |
| 53 | `random` | 2, 0 | lo hi -> num | `BuiltinRandom`: integer range when both are integers, else float |
| 54 | `sqrt` | 1, 0 | num -> float | `BuiltinSqrt` |
| 55 | `sin` | 1, 0 | rad -> float | `BuiltinSin` |
| 56 | `cos` | 1, 0 | rad -> float | `BuiltinCos` |
| 57 | `tan` | 1, 0 | rad -> float | `BuiltinTan` |
| 58 | `asin` | 1, 0 | num -> rad | `BuiltinAsin` |
| 59 | `acos` | 1, 0 | num -> rad | `BuiltinAcos` |
| 60 | `atan` | 1, 0 | num -> rad | `BuiltinAtan` |
| 61 | `rad-normalize` | 1, 0 | rad -> rad | `BuiltinRadNormalize`: normalizes an angle (`NormalizeAngle`) |
| 62 | `atan2` | 2, 0 | y x -> rad | `BuiltinAtan2` |
| 63 | `vector-dot-product` | 2, 0 | v v -> float | `BuiltinVectorDotProduct` |
| 64 | `vector-cross-product` | 2, 0 | v v -> v | `BuiltinVectorCrossProduct` |
| 65 | `vector-lth` | 1, 0 | v -> float | `BuiltinVectorLength` |
| 66 | `vector-distance` | 1, 1 | v [v] -> float | `BuiltinVectorDistance` (to the origin without a second vector) |
| 67 | `vector-distance-squared` | 1, 1 | v [v] -> float | `BuiltinVectorDistanceSquared` |
| 68 | `vector-normalize` | 1, 1 | v [default] -> v | `BuiltinVectorNormalize` (default used for a near-zero vector) |
| 69 | `vector-project-normal` | 2, 0 | v n -> v | `BuiltinVectorProjectNormal`: (v·n)n |
| 70 | `vector-project-tangent` | 2, 0 | v n -> v | `BuiltinVectorProjectTangent`: v - (v·n)n |
| 71 | `vector-ballistic-velocity` | 3, 1 | from to speed [obj] -> v | `BuiltinVectorBallisticVelocity`: launch velocity under gravity (0, 0, -1750) |
| 72 | `vector-radian-normal` | 2, 0 | v v -> (rad v) | `BuiltinVectorRadianNormal`: angle and rotation axis between two vectors |
| 73 | `matrix-transpose` | 1, 0 | m -> m | `BuiltinMatrixTranspose` |
| 74 | `matrix-invert` | 1, 0 | m -> m | `BuiltinMatrixInvert` |
| 75 | `matrix-calculate-dmat` | 2, 0 | m m -> m | `BuiltinMatrixCalculateDmat`: the relative rotation between two matrices |
| 76 | `matrix-interpolate-rotate` | 3, 0 | m m t -> m | `BuiltinMatrixInterpolateRotate` |
| 77 | `matrix-decompose-to-translate` | 1, 0 | m -> v | `BuiltinMatrixDecomposeToTranslate` |
| 78 | `matrix-decompose-to-rotate` | 1, 0 | m -> m | `BuiltinMatrixDecomposeToRotate` |
| 79 | `matrix-decompose-to-euler` | 1, 0 | m -> v | `BuiltinMatrixDecomposeToEuler` |
| 80 | `matrix-decompose-to-radian-normal` | 1, 0 | m -> (rad v) | `BuiltinMatrixDecomposeToRadianNormal` |
| 81 | `matrix-look-at` | 2, 1 | from to [up] -> m | `BuiltinMatrixLookAt` (up defaults to +Z) |
| 82 | `matrix-tilt-upright` | 1, 0 | m -> m | `BuiltinMatrixTiltUpright` |
| 83 | `clq-evaluate` | 2, 0 | clq t -> float | `BuiltinClqEvaluate` |
| 84 | `clq-evaluate-lm` | 3, 0 | clq lm t -> float | `BuiltinClqEvaluateLm`: result clamped to the range |
| 85 | `clq-fit` | 4, 0 | num num num num -> clq | `BuiltinClqFit` |
| 86 | `lm-limit` | 2, 0 | lm num -> float | `BuiltinLmLimit`: clamp |
| 87 | `lm-check?` | 2, 0 | lm num -> bool | `BuiltinLmCheck`: inside the range |
| 88 | `floor` | 1, 0 | num -> int | `BuiltinFloor` (truncates: wrong for negatives) |
| 89 | `ceiling` | 1, 0 | num -> int | `BuiltinCeiling` ((int)(x + 1): wrong for whole numbers) |
| 90 | `round` | 1, 0 | num -> int | `BuiltinRound` ((int)(x + 0.5)) |
| 91 | `truncate` | 1, 0 | num -> int | `BuiltinTruncate` |
| 92 | `abs` | 1, 0 | num -> num | `BuiltinAbs` |
| 93 | `max` | 1, 1 | num ... -> num | `BuiltinMaximum` |
| 94 | `min` | 1, 1 | num ... -> num | `BuiltinMinimum` |
| 95 | `modulo` | 2, 0 | int int -> int | `BuiltinModulo` (C `%`) |
| 96 | `current-time` | 0, 0 | -> float | `BuiltinCurrentTime`: game clock seconds |

### 3.3 World queries, scheduling, options (97-111)

| # | Spelling | min, rest | Arguments -> result | C++ function / notes |
|---|---|---|---|---|
| 97 | `load-filter~` | 0, 0 | -> int | `BuiltinCurrentLevel`: the current level index |
| 98 | `?` | 0, 0 | -> int | `BuiltinCurrentEpisode`: episode of the level, or -1 |
| 99 | `?` | 0, 0 | -> int | `BuiltinCurrentWorld`: world of the level, or -1 |
| 100 | `?` | 0, 0 | -> obj | `BuiltinFindCurrentJob` (null object in free roam) |
| 101 | `?` | 0, 0 | -> obj | `BuiltinFindCurrentTask` |
| 102 | `?` | 0, 0 | -> obj | `BuiltinFindPendingTask` |
| 103 | `?` | 0, 0 | -> float | `BuiltinGetDifficulty`: the dynamic difficulty scale |
| 104 | `?` | 0, 0 | -> int | `BuiltinGetDevFlag`: 0 in the retail build (name inferred) |
| 105 | `schedule-callback` | 2, 1 | delay proc [arg ...] -> int | `BuiltinScheduleCallback`; section 5.3 |
| 106 | `cancel-callback` (assumed) | 1, 0 | handle -> void | `BuiltinCancelCallback` |
| 107 | `defer-object-update` | 2, 0 | obj delay -> void | `BuiltinDeferObjectUpdate`; section 5.3 |
| 108 | `addo` | 3, 0 | obj id value -> void | `BuiltinAddOption`; section 4.2 |
| 109 | `ensureo` | 3, 0 | obj id value -> void | `BuiltinEnsureOption` |
| 110 | `seto!` | 3, 0 | obj id value -> void | `BuiltinSetOption` |
| 111 | `geto` | 2, 0 | obj id -> any | `BuiltinGetOption` |

### 3.4 Objects, sound, saves (112-139)

| # | Spelling | min, rest | Arguments -> result | C++ function / notes |
|---|---|---|---|---|
| 112 | `find-object` | 2, 1 | oid scope [ctx] -> obj | `BuiltinFindObject`; section 4.4 |
| 113 | `find-objects` | 2, 1 | oid scope [ctx] -> list | `BuiltinFindObjects` (up to 64) |
| 114 | `?` | 2, 1 | oid-list scope [ctx] -> list | `BuiltinFindObjectsFromOids`: `find-object` over a list |
| 115 | `find-nearest-object` | 2, 1 | oid scope [ctx] -> obj | `BuiltinFindNearestObject` |
| 116 | `find-nearest-objects` | 2, 1 | oid scope [ctx] -> list | `BuiltinFindNearestObjects` |
| 117 | `find-world-object` | 0, 0 | -> obj | `BuiltinFindWorldObject`: the world (`SW`) |
| 118 | `find-sound-object~` | 0, 0 | -> obj | `BuiltinFindSoundObject`: the sound manager (dialogue methods) |
| 119 | `find-camera-object` | 0, 0 | -> obj | `BuiltinFindCameraObject`: the camera (`CM`) |
| 120 | `find-class-objects` | 2, 0 | cid scope -> list | `BuiltinFindClassObjects` (up to 384) |
| 121 | `find-objects-in-bounding-box` | 2, 0 | v-min v-max -> list | `BuiltinFindObjectsInBoundingBox` |
| 122 | `find-objects-in-bounding-sphere` | 2, 0 | v-center radius -> list | `BuiltinFindObjectsInBoundingSphere` |
| 123 | `hit-test-objects` | 3, 0 | obj-list from to -> list of (obj pos normal) | `BuiltinHitTestObjects`: segment test |
| 124 | `hit-test-objects-first` | 3, 0 | obj-list from to -> (obj pos normal) | `BuiltinHitTestObjectsFirst` |
| 125 | `convert-object-position` | 3, 0 | obj-from obj-to v -> v | `BuiltinConvertObjectPosition`: a position from one object's space to another's |
| 126 | `convert-object-vector` | 3, 0 | obj-from obj-to v -> v | `BuiltinConvertObjectVector` |
| 127 | `convert-object-matrix` | 3, 0 | obj-from obj-to m -> m | `BuiltinConvertObjectMatrix` |
| 128 | `near-clip-center` | 0, 0 | -> v | `BuiltinNearClipCenter`: centre of the camera's near plane |
| 129 | `start-sound` | 2, 0 | sfx-id int -> obj | `BuiltinStartSound`: returns the sound object |
| 130 | `start-rumble` | 2, 0 | rumble-kind seconds -> void | `BuiltinStartRumble` |
| 131 | `?` | 1, 0 | path -> any | `BuiltinGetSaveVar`; section 5.4 |
| 132 | `set-save-var!~` | 2, 0 | path value -> void | `BuiltinSetSaveVar`; unused by retail scripts |
| 133 | `find-binding-or-noop~` | 1, 0 | 'sym -> proc | `BuiltinFindBindingOrNoop`: sym looked up from the **parent** of the nearest enclosing script frame (the binding the caller's script shadows), else builtin 134 |
| 134 | `?` | 0, 1 | any ... -> void | `BuiltinNoop` |
| 135 | `?` | 0, 1 | int ... -> int | `BuiltinBitAnd` (empty: -1) |
| 136 | `?` | 0, 1 | int ... -> int | `BuiltinBitOr` (empty: 0) |
| 137 | `find-hintbox-object~` | 0, 0 | -> obj | `BuiltinFindHintboxObject`: the hint box widget (listed as `find-ticker-object~` in older research listings) |
| 138 | `?` | 1, 0 | int -> bool | `BuiltinIsPowerUpAcquired` |
| 139 | `?` | 2, 0 | int bool -> void | `BuiltinSetPowerUpAcquired` (also sends a game event) |

### 3.5 Stubs (140-168)

`BuiltinStub140` .. `BuiltinStub168` (`src/game/app/bif2.cpp`) had their bodies compiled out of the release build and
return nil; their names and purposes are unknown. Only the arities survive. No retail script calls them. A port may
reuse these slots for new builtins without renumbering anything.

| # | min, rest | # | min, rest | # | min, rest |
|---|---|---|---|---|---|
| 140-150 | 0, 0 | 151-153 | 1, 0 | 154, 155 | 0, 0 |
| 156 | 3, 0 | 157 | 1, 0 | 158 | 0, 0 |
| 159 | 1, 0 | 160, 161 | 0, 0 | 162 | 3, 0 |
| 163 | 2, 0 | 164 | 1, 0 | 165 | 0, 0 |
| 166 | 2, 0 | 167, 168 | 0, 0 | | |

## 4. Objects and options

### 4.1 What an option is

Every class publishes numbered **options**: properties and methods, merged with those of its base classes and sorted
by id. Option ids are per class and polymorphic (the same id can mean different things on different classes), so
a script must know the class of the object it addresses. Ids 1-13 are on every object (`BASIC`); `LO` adds ids 14-67
(with gaps) for every placed object; the full list is generated in [splice-options.md](splice-options.md).

Each binding has a type (bool, int, float, vector, matrix, clq, lm, smp, object reference, or method) and some of:
get (`geto` reads it), set (`seto!` writes it), method (`geto` returns a callable method), file (the level loader sets
it from the object's option list in the world file).

### 4.2 The four option builtins

| Form | Ids up to 0x4D0 (class options) | Ids above 0x4D0 (sidebag variables) |
|---|---|---|
| `(geto obj id)` | the record's value converted to a Splice value, or a method value | the sidebag binding, nil when unbound |
| `(seto! obj id value)` | writes through the record (value converted to the record's type) | assigns an existing sidebag binding (nothing when unbound) |
| `(addo obj id value)` | **always the sidebag**: adds a new binding | the same |
| `(ensureo obj id value)` | **always the sidebag**: assigns when bound, else adds | the same |

Notes from the code (`src/game/object/spliceobj.cpp`):

- `ensureo` always ends with the value set; the comment on `EnsureBasicOption` says "only when it has no value yet",
  which is not what the code does. The native `EnsureLoSidebag*` helpers use it for typed defaults.
- `geto` / `seto!` on an id the class does not have, or with an access the record lacks (`seto!` on a get-only
  option), reach a null record or a null function: a crash in the original. Check the class table first.
- Sidebag variables (ids 1255-1875 in retail scripts) are per object, unlike frame variables; the custom-property
  methods (BASIC options 4-10, thunks `RefThunkBasicCUSTOM_INT_PROPERTY` and siblings) let level files declare them with a type and default.
- `seto!` returns void; `geto` of an object option returns an object value (possibly the null object).

### 4.3 Calling methods

A method option is fetched with `geto` and applied: `((geto obj id) arg ...)`. The method value carries the object, the
native thunk and its argument count; arguments are converted to the native types by `RefSetArgsFromSplice`. Methods
return whatever the thunk returns, usually void. A method value can be stored and called later (it holds a reference
to the object).

The thunks are `RefThunk<Class><SCRIPT_NAME>` where the script-side name is known (for example `RefThunkBombDETONATE`)
and `RefThunk_<Native>` after the native function otherwise, in `src/generated/splicewrap/` (one `.inc` per thunk)
and `src/game/app/class_registry.cpp`. Reading a thunk's `.inc` tells you its argument types and what it calls.

### 4.4 Finding objects

- **By oid.** `(find-object oid scope [ctx])`: oid is the global object id of the placed object (the same ids across
  levels for shared actors such as the gang); scope is an object to search under, or the null object to search the
  whole world; the optional ctx object selects whose proxy scope is searched (defaults to scope). `find-objects`
  returns every match, up to 64 (oids are not unique when objects are proxied or cloned); the `nearest` variants
  use another search kind that picks the closest match(es).
- **Singletons.** `find-world-object`, `find-camera-object`, builtin 118 (sound manager), builtin 137 (hint box),
  and builtins 100-102 for the current job and tasks.
- **By class or space.** `find-class-objects` (class id and scope), `find-objects-in-bounding-box`,
  `find-objects-in-bounding-sphere`, and the `hit-test-objects` segment tests.
- **Self.** Every handler receives its object as the first argument; BASIC option 2 also returns the object itself.

### 4.5 The generated option table

[splice-options.md](splice-options.md) lists, for each of the 199 classes, the option ids the class adds or rebinds,
with a label, the type and the access. Regenerate it after changing the option tables, thunks or accessors:

```
python3 tools/splice_options.py            # writes docs/reference/splice-options.md
python3 tools/splice_options.py --tsv      # every (class, id) binding, for scripts and diffs
```

## 5. Events

### 5.1 Handlers

An event is a symbol id 0-28. An object handles it when its frame binds that symbol to a procedure:

```scheme
(define <event-symbol> (lambda (self arg ...) body...))
```

`HandleBasicSpliceEvent` (`src/game/object/basic.cpp`, vtable slot 3) builds the call `(event self args...)` and
either evaluates it immediately in the object's frame or, when the sender posts it, schedules it for the current time
(it runs in the next callback pass of the world update). The argument values are converted from native values by the
event's parameter-type list (`s_aotypEvtParm` in `class_registry_options.inc`); the sender decides how many of them it
passes. Unhandled events cost nothing. Handlers commonly chain to the binding they shadow first:
`((find-binding-or-noop~ '<event>) self arg ...)`. Builtin 133 walks up from the calling frame to the object's script
frame and looks the symbol up in that frame's parents (a proxy's or base script's frame), so it finds the inherited
handler, not the handler itself, and returns the no-op builtin when there is none.

### 5.2 Event ids

Meanings come from the code that sends them; "inferred" marks readings from script usage. Parameter types (after
`self`) are from the parameter table; senders may pass fewer.

| Id | Meaning | Sent by | Parameter types |
|---|---|---|---|
| 0 | post-load | `PostLoLoad` when the object finishes loading | object (LO) |
| 1 | update | `UpdateLo` when the object's next Splice update time has come (section 5.3) | object |
| 2, 3 | unknown | | object |
| 4 | label | animation segment and relay label callbacks | int, object (LO) |
| 5 | trigger | `SendTriggerEvent` (BASIC option 12): buttons, exits, warps, volumes | object (LO) |
| 6 | untrigger | `SendUntriggerEvent` (BASIC option 13) | object (STEPGUARD) |
| 7-13 | unknown | | see the generated table below |
| 14 | hit (inferred) | | object (LO), object (LO) |
| 15, 16 | unknown | | |
| 17 | game event (jobs) | the game event bus; argument 1 is the event kind (16 = hero died) | int, object (LO), object (LO) |
| 18 | unknown | | int, object (LO) |
| 19 | activate (tasks, jobs; inferred) | task and job state changes | object (LO) |
| 20 | deactivate (inferred) | | object (TASK) |
| 21-28 | unknown | | see below |

Full parameter-type list from the table, for reference: 7 int, int, object (SMA); 8 int, int, object (VOLBTN);
9 object (SO); 10 object (SO), int, object (SO); 11 object (SO), int, object (LO); 12 int, int, object (PO);
13 object (LO); 15 object (LO), object (BREAKABLE); 16 object (LO); 21, 22 object (TASK); 23 bool, object (STEPGUARD);
24 int, object (SPAWNER); 25 object (STEPGUARD), object (STEPPLAYER); 26 object (BOMB); 27 object (DETECTOR);
28 object (LO), int, int, bool.

### 5.3 Scheduling

- **Per-frame updates.** An object whose frame binds event 1 at post-load gets update events. `(defer-object-update
  obj seconds)` sets the object's next update time to now + seconds, so a handler throttles itself by calling it at
  the end of each update.
- **Callbacks.** `(schedule-callback seconds proc arg ...)` schedules `(proc arg ...)` at now + seconds and returns
  an integer handle for `(cancel-callback handle)`. The arguments are evaluated when scheduling and passed quoted (not
  re-evaluated). The call runs in a fresh, empty frame, so proc must be a **lambda** (a closure keeps its own
  environment; a builtin passed here is mis-tagged as a procedure and crashes) and must not rely on dynamic scope.
  Callbacks are sorted by time and run once from `ProcessSwCallbacks` during the world update. Posted events use the
  same queue but cannot be cancelled.

### 5.4 Save variables

Persistent state lives in save variables, addressed by a **path**: a list of save-schema symbols and integers, for
example `'(<character> <field>)` or `'(<array> 3)`, using the symbol ids 30-989 whose names are in the level's
save-schema table. Builtin 131 (`BuiltinGetSaveVar`) returns the value at a path; builtin 132 (`set-save-var!~`) writes one. The schema,
the trie that stores the values and the native `GetSaveVarByName` / `SetSaveVarByName` are in
[formats/save-variables.md](../formats/save-variables.md) and
[systems/progression-and-saves.md](../systems/progression-and-saves.md). Power-up flags have their own builtins
(138, 139).

## 6. Writing a script

### 6.1 Shape of a script entry

A world entry's script table holds one compiled list per entry; each is a single `(begin ...)` of top-level
`define`s, evaluated once at level load into the entry's frame. Objects that name the entry share that frame and
receive events through the handlers it defines.

### 6.2 An invented example

A pressure plate that, when triggered, raises a counter on itself, lights up a lamp after three presses, and resets
after ten seconds. Everything below is invented for illustration: the option ids, oids and handler symbols are
placeholders (`sidebag-presses`, `opt-lamp-on`, `lamp-oid`, `post-load`, `trigger`), and builtins without a known spelling would be written by
number.

```scheme
(begin
  ;; placeholders a compiler would resolve: trigger = event 5, post-load = event 0
  (define press-limit 3)

  (define post-load
    (lambda (self)
      (ensureo self sidebag-presses 0)))            ; per-object counter in the sidebag (id > 0x4D0)

  (define reset-plate
    (lambda (self)
      (seto! self sidebag-presses 0)))

  (define trigger
    (lambda (self other)
      ((find-binding-or-noop~ 'trigger) self other) ; chain to any earlier handler
      (seto! self sidebag-presses (+ (geto self sidebag-presses) 1))
      (if (= (geto self sidebag-presses) press-limit)
          (begin
            ((geto (find-object lamp-oid (geto self 2)) opt-lamp-on))  ; call a method option
            (schedule-callback 10.0 reset-plate self))))))
```

Points the example illustrates:

- The counter lives in the sidebag, so two plates sharing the script do not share the count; `press-limit` lives in
  the shared frame, which is fine because it never changes.
- `if` compares with `=`, which returns a boolean, so the "word equals 1" rule is safe. Avoid `(if (geto ...) ...)`
  on integer options.
- `schedule-callback` gets the lambda value `reset-plate`, not a symbol, and its argument `self` is evaluated now.
- `find-object` takes a scope object; the null object would search the whole world.
- Before using an option id, look the object's class up in [splice-options.md](splice-options.md) and check the
  access (`get` / `set` / `method(n)`).

### 6.3 Toolchain status

- **Dump and compile**: `tools/splice/dump.py` reads every level's scripts from your own disc image and prints them
  in the notation of this page (plus a JSON AST); `tools/splice/encode.py` compiles a listing back to the serialized
  form, byte-identical for an unedited listing. Usage, the output notation and the format:
  [splice-dump.md](splice-dump.md). Level scripts are game content and are never committed.
- **Writing scripts into a level** also needs the world file re-laid out (later sections shift, per-entry alignment,
  LZSS re-compression); not done yet.
- **Matching build**: the builtin table, option tables and thunks are data of the original executable, so new
  builtins or options exist only in a non-matching (port or mod) build. The stub slots 140-168 are the natural place
  for new builtins; [cookbook/splice-command.md](../cookbook/splice-command.md) walks through adding builtins,
  options and events.
- **Debugging**: `print`, `print-frame`, `print-sidebag` and the message of `assert` are compiled out of the release
  build. A port can restore them by giving the builtins bodies again.
