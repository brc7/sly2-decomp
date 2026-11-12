# Mangled-name signature check (sigcheck.py)

The decomp often declares a function under its mangled linker name with parameter types of its own choosing:

```cpp
void func_001703F8(void *pzpr, int a, int b) __asm__("InitZpr__FP3ZPR3ZPKP2LO");   // asm label
extern "C" int IsEntryInDl__FP2DLPv(void *, void *);                                // C name = mangled name
```

The compiler checks neither: the label or the C linkage bypasses mangling. That keeps the match, but a native
64-bit port compiles these declarations as written, so an `int` standing in for a pointer truncates it, a missing
parameter shifts the others, and an explicit struct-return slot lands in the wrong register on arm64. sigcheck.py
compares every such declaration with the signature its mangled name encodes.

```sh
python3 tools/cleanup/signatures/sigcheck.py [--root REPO] [--map LINKMAP] [--out DIR] [--top N]
python3 tools/cleanup/signatures/demangle.py 'InitDl__FP2DLi' '_$_1A'   # the demangler on its own
```

About ten seconds. Outputs in DIR (default build/signatures/):

- **sigcheck.csv**: one row per binding declaration that disagrees: worst class, all classes, symbol, demangled
  signature, file, line, binding (`asm-label` or `c-name`), scope, the declaration as parsed, details per parameter;
- **cross_tu.csv**: symbols whose declarations (asm-labelled, C-named and canonical C++ ones) disagree with each other,
  with each variant's sites: the "same symbol, different signature in another TU" list;
- **summary.md**: totals by class, the top symbols ranked by severity with file:line, the cross-declaration conflicts;
- **unparsed.csv**: function asm labels the parser did not read as a declaration, or that name a function no config
  file has.

It is a report, not a gate (exit 0).

## How it works

1. **Symbols**: every name in config/symbol_addrs.txt, config/functions.csv, config/matched.csv and the link map
   (`--map`, default build/link/sly2.map when present) that demangle.py reads as a function.
2. **Demangler**: binutils dropped the old GNU v2 ABI (`c++filt -s gnu-v2` is gone, and the EE toolchain bundle has no
   c++filt), so demangle.py reimplements what g++ 2.95 emits: free and member functions, const members,
   constructors, destructors, operators, nested names (`Q2`), template classes (`t`) and template functions (`H`,
   which also encode the return type), `T`/`N` repeats. Facts checked against the game's compiler: repeat indices
   count every parameter including repeats, and for a member function index 0 is the class; static member functions
   mangle exactly like non-static ones; `mode(TI)` integers (qword) mangle as `long long`.
3. **Declarations**: a small C/C++ parser over src/ and include/ (comments and preprocessor lines stripped, scopes
   tracked: namespaces, classes, extern "C", function bodies). Types are canonicalised (`struct` dropped, builtins
   normalised: `unsigned int` = `uint`; typedefs resolved through the file's include closure, then any typedef;
   arrays decay; top-level const dropped). A declaration *binds* to a symbol through an asm label, or because its own
   name is the mangled symbol. A plain C++ declaration whose name, class and parameter types equal the demangled
   signature is a *canonical* declaration: the compiler mangled it, so it is right by construction, and it supplies
   the return type the mangled name does not encode.
4. **Comparison** on the ABI-level parameter list: a member function gets its class pointer first (`C const*` for a
   const member); a declaration inside a class gets its own class pointer the same way. A free declaration may match
   either the member form or the static form (they mangle the same). Destructors take `(this, int __in_chrg)`.

## Classes (most severe first)

| class | meaning | port impact |
|---|---|---|
| wrong-count | parameter count differs (and it is not one of the forms below) | arguments shift |
| int-vs-pointer | an integer type where the symbol has a pointer or reference (or the reverse), in a parameter or the return type | truncation on 64-bit |
| struct-return | one extra leading pointer parameter, the rest agreeing: the hidden struct-return argument written out | arm64 passes it in x8, not x0 |
| wrong-type | pointer, reference or class to a different class (not related by derivation), or a different pointer depth | wrong layout read |
| return | return type differs from the canonical declaration's (scalar kinds; pointer/int is int-vs-pointer) | value misread |
| other | scalar kind or width (int vs uint, char vs uchar, float vs int), function-pointer arity, unparsed parameter | usually benign, check |
| void-pointer | `T*` declared as `void*` | hides the type, same ABI |
| address-only | declared with no parameters (`void f()`): only usable for its address, never called through this declaration | none while it is only address-taken |
| derived | pointer to a class derived from (or a base of) the symbol's class | same ABI with single inheritance |
| alias | enum vs integer, reference vs pointer | same ABI |
| const | differs only in const | none |

The first four are the ones to fix before a native build; the rest are readability debt (and cross_tu.csv shows where
one declaration in a header would replace many private ones).

## Limits

- The parser is heuristic: a declaration produced by a macro, or a parameter written in a form it does not know,
  is listed in unparsed.csv or carries an `unparsed parameter` detail; check those by hand.
- Typedef resolution follows the include closure of the declaring file; a header that relies on a typedef from an
  earlier include of its TU falls back to the first typedef of that name anywhere.
- Classes named in mangled names can be enums or structs; an `enum` definition (or `typedef enum`) somewhere in the
  tree is what makes `int vs OID` an alias rather than a wrong type.
- Function pointer types are compared by arity only.

# Struct-result convention check (sretcheck.py)

```sh
python3 tools/cleanup/signatures/sretcheck.py [--root REPO] [--out DIR]   # sret.csv, sret.md in DIR (default build/signatures/)
```

g++ 2.95 passes a struct (or class) result's address as a hidden first argument, so `MATRIX3 f(float, qword)` and
`void f(MATRIX3 *out, float, qword)` call the same EE code and the decomp uses both spellings for one symbol. Natively
they differ: x86-64 passes a large result's address in rdi (the mismatch happens to work) but returns small trivially
copyable structs in registers, and arm64 passes the result address in x8 with the arguments still in x0.. (it never
works). sretcheck.py reuses sigcheck.py's parser (`sigcheck.load`) and groups every declaration by the linker symbol
it reaches natively: its asm label, its C name (extern "C", or a later definition of a name an extern "C" declaration
gave), or its mangled name when it is the canonical C++ declaration. It skips lines only the EE compiles (branches on
SLY2_EE_LAYOUT / SLY2_EE_ASM / `__R5900__`), file-local `static` helpers, and the forms that are bound natively
elsewhere (SLY2_SRET_LABEL's native label, SLY2_BUILTIN_C's `sym_c`). A symbol is listed when one declaration
returns a class / struct / qword by value with N parameters and another has N + 1 with a leading pointer
(`value` vs `out`). It also checks:

- function-pointer tables against the type they are called through: bif.cpp g_mpbifkbif (`CRef (*)(int, CRef *,
  CFrame *)`), g_aeopid's PMF getters (otyp & 0x8000: `CRef (BASIC::*)()`; otyp 9 / 10 / 11: an explicit CLQ / LM /
  SMP out-pointer);
- that every SLY2_SRET_LABEL native label names a definition (the adapter);
- the world callback API declarations (PostSwCallback, EnsureSwCallback, ClearSwCallbacks) that pass the
  `void (CObj::*)(int)` PMF as an integer or the argument word as a 32-bit integer (natively a PMF is two
  pointer-sized words, and a pointer argument truncates).

Fix pattern: keep the EE spelling and bind the minority form natively to an adapter beside the definition
(docs/guides/portability.md, "Struct results and arm64").
