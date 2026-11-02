#!/bin/bash
# One-command setup for the Sly 2 decomp (Linux or WSL).  Downloads every compiler/tool the build uses into
# tools/ (none of them are stored in the repo), then extracts the game executable from YOUR OWN disc image.
#
#   bash tools/setup_all.sh "/path/to/Sly 2 - Band of Thieves (USA).iso"
#
# Needs: bash, curl, tar, xz, python3 (+ pip).  On Debian/Ubuntu/WSL it also installs the 32-bit C library
# (sudo apt) that the native PS2 compilers need.
#
# Already set up?  Just run it again: it checks every item first, only downloads what is missing (e.g. the new
# ee-gcc 2.9 / 3.2 compilers), never overwrites anything, and prints a summary of what it found vs. added.
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
C=tools/compilers
REL=https://github.com/decompme/compilers/releases/download/compilers
mkdir -p $C/bundle $C/extra
HAVE=(); ADDED=(); MISSING=()
have() { HAVE+=("$1"); echo "  ok (already there): $1"; }
added() { ADDED+=("$1"); }

get() {  # get URL DEST-DIR  (extracts .tar.xz / .tar.gz into DEST-DIR, skips if DEST-DIR is non-empty)
  local url=$1 dest=$2
  if [ -d "$dest" ] && [ -n "$(ls -A "$dest" 2>/dev/null)" ]; then have "$dest"; return; fi
  echo "  downloading $(basename "$url") -> $dest"; added "$dest"
  mkdir -p "$dest"; local t; t=$(mktemp)
  curl -fsSL -o "$t" "$url"
  case "$url" in *.tar.xz) tar -xJf "$t" -C "$dest";; *.tar.gz) tar -xzf "$t" -C "$dest";; esac
  rm -f "$t"
}

echo "== wibo (runs the Windows compilers)"
if [ -x tools/wibo-i686 ]; then have tools/wibo-i686; else
  curl -fsSL -o tools/wibo-i686 https://github.com/decompals/wibo/releases/download/1.0.0/wibo-i686; chmod +x tools/wibo-i686; added tools/wibo-i686; fi

echo "== PS2 compiler bundle (game code: ee-gcc 2.95.3-136, plus 2.95.x and Metrowerks builds)"
if [ -d $C/bundle/ee-gcc2.95.3-136 ]; then have $C/bundle; else
  added $C/bundle; t=$(mktemp -d); curl -fsSL -o $t/b.tar.xz $REL/ps2_compilers.tar.xz; tar -xJf $t/b.tar.xz -C $t
  # the archive's top folder name varies; move every ee-gcc*/mwcps2* directory into bundle/
  for d in $(find $t -maxdepth 3 -mindepth 1 -type d \( -name 'ee-gcc*' -o -name 'mwcps2*' \) -prune); do mv -n "$d" $C/bundle/; done
  rm -rf $t
fi

echo "== ee-gcc 2.95.3-114 (its SN assembler ps2eeas assembles the game code)"
get $REL/ee-gcc2.95.3-114.tar.gz $C/2.95.3-114

echo "== ee-gcc 2.96 (Sony SDK library code, GNU as)"
get $REL/ee-gcc2.96.tar.xz $C/ee-gcc2.96

echo "== ee-gcc 2.9 / 3.2 (Sony SDK libraries: mult1, tail calls)"
for n in ee-gcc2.9-990721 ee-gcc2.9-991111 ee-gcc2.9-991111a ee-gcc2.9-991111-01 ee-gcc3.2-040921; do
  get $REL/$n.tar.xz $C/extra/$n; done
for n in ee-gcc2.9-991111-01-dtls13010 ee-gcc3.2-030926; do
  get $REL/$n.tar.gz $C/extra/$n; done

echo "== 32-bit C library for the native compilers"
if [ -e /lib/ld-linux.so.2 ]; then have "32-bit C library"; else
  added "32-bit C library"
  if command -v apt-get >/dev/null; then
    sudo dpkg --add-architecture i386 && sudo apt-get update -qq && sudo apt-get install -y -qq libc6:i386
  else
    echo "  !! install your distro's 32-bit glibc (libc6:i386 / glibc.i686) so tools/compilers/*/bin/ee-gcc can run"
  fi
fi

echo "== Python packages"
if python3 -c "import rabbitizer, spimdisasm, splat" 2>/dev/null; then have "python packages"; else
{ python3 -m pip install -q --user rabbitizer spimdisasm "splat64[mips]" 2>/dev/null \
  || python3 -m pip install -q --break-system-packages rabbitizer spimdisasm "splat64[mips]"; } && added "python packages" \
  || { echo "  !! pip install failed - install rabbitizer, spimdisasm and splat64[mips] yourself"; MISSING+=("python packages"); }
fi
if [ -d "$HOME/m2c" ]; then have "~/m2c"; else added "~/m2c"
  git clone -q https://github.com/matt-kempster/m2c "$HOME/m2c" || echo "  !! couldn't clone m2c (only needed for tools/show.py drafts)"; fi

echo "== Game executable from your own disc"
if [ -f disc/SCUS_973.16 ]; then
  if [ "$(sha1sum disc/SCUS_973.16 | cut -d' ' -f1)" = de2af92a54de71e33edc8a160c31c0337a95e6f5 ]; then have "disc/SCUS_973.16 (already extracted, checksum OK)"
  else echo "  !! disc/SCUS_973.16 exists but is not the NTSC-U v1.0 executable (wrong SHA-1)"; MISSING+=("correct disc/SCUS_973.16"); fi
elif [ -n "$1" ]; then python3 tools/extract_elf.py "$1" && added disc/SCUS_973.16
else echo "  !! run again with the path to your Sly 2 (USA) ISO to extract disc/SCUS_973.16"; MISSING+=("disc/SCUS_973.16"); fi

echo "== Disassembly (used by tools/show.py for m2c drafts)"
if [ -d asm ]; then have asm/; elif [ -f disc/SCUS_973.16 ]; then python3 -m splat split config/sly2.yaml >/dev/null && added asm/; fi

echo
echo "== Summary"
echo "  already there: ${#HAVE[@]} item(s)"
if [ ${#ADDED[@]} -gt 0 ]; then echo "  added now:"; printf '    %s\n' "${ADDED[@]}"; else echo "  added now: nothing - you were fully set up"; fi
if [ ${#MISSING[@]} -gt 0 ]; then echo "  still missing:"; printf '    %s\n' "${MISSING[@]}"; fi
echo
echo "Done. Check it works:  python3 tools/matchtool.py check src/game/core/binstream/bis/bis.cpp   (every function should say MATCH)"
