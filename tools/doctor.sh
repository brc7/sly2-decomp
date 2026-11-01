#!/usr/bin/env bash
# Sanity-check the toolchain: does each kind of compiler binary actually run here?
# Useful on macOS where x86 code runs under emulation (Rosetta or QEMU) inside Docker.
cd "$(dirname "$0")/.."
ok=0; bad=0
try() { local what=$1; shift
  if out=$(timeout 60 "$@" 2>&1); then echo "OK    $what: $(echo "$out" | head -1)"; ok=$((ok+1))
  else echo "FAIL  $what (exit $?): $(echo "$out" | head -3 | tr '\n' ' ')"; bad=$((bad+1)); fi; }
echo "host: $(uname -a)"
[ -f disc/SCUS_973.16 ] && echo "disc/SCUS_973.16: $(sha1sum disc/SCUS_973.16 | cut -d' ' -f1) (want de2af92a54de71e33edc8a160c31c0337a95e6f5)" || echo "MISSING disc/SCUS_973.16"
try "python rabbitizer"            python3 -c "import rabbitizer; print('rabbitizer', rabbitizer.__version__)"
try "wibo + ee-gcc 2.95.3-136 (game code)" tools/wibo-i686 tools/compilers/bundle/ee-gcc2.95.3-136/bin/ee-gcc.exe --version
try "wibo + SN ps2eeas (assembler)" tools/wibo-i686 tools/compilers/2.95.3-114/lib/gcc-lib/ee/2.95.3/ps2eeas.exe --version
try "native i386 ee-gcc 2.96 (SDK)" tools/compilers/ee-gcc2.96/bin/ee-gcc --version
try "native i386 GNU as 2.96 (SDK)" tools/compilers/ee-gcc2.96/ee/bin/as --version
try "native i386 ee-gcc 2.9-991111 (SDK)" tools/compilers/extra/ee-gcc2.9-991111/bin/ee-gcc --version
echo "summary: $ok ok, $bad failed"
[ $bad -eq 0 ]
