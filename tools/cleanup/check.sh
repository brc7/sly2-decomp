#!/usr/bin/env bash
# Per-change verification against a recorded baseline: matchtool + objcmp (nm, sizes, sections, relocations by symbol).
#
#   tools/cleanup/check.sh BASE.json TOUCHED...      (TOUCHED = repo-relative .cpp/.c/.h paths, or @FILE)
#
# 1. expands TOUCHED to every source that includes a touched header (includers.py),
# 2. runs `tools/matchtool.py check` on each (prints only files with a non-MATCH function or a compile failure),
# 3. compares each object with BASE.json (objcmp.py cmp; record it first, in a worktree at the base commit:
#      tools/cleanup/objcmp.py record --root <base worktree> BASE.json $(cat all_sources.txt)).
# Then close a family/integration with the whole-tree gates: rm build/progress_cache.json; tools/progress.py;
# git diff --quiet config/matched.csv; tools/link.py (0 differing bytes); tools/linkcheck.py.
# Env: JOBS (default 2) parallel compiles; matchtool's own variables (SLY2_PYTHONPATH, SLY2_M2C_INC) pass through.
set -uo pipefail
[ $# -ge 2 ] || { sed -n '2,12p' "$0"; exit 2; }
HERE="$(cd "$(dirname "$0")" && pwd)"; ROOT="$(cd "$HERE/../.." && pwd)"
BASE="$(realpath "$1")"; shift
cd "$ROOT"
files=$(python3 "$HERE/includers.py" "$@")
n=$(printf '%s\n' $files | grep -c .)
echo "files to check: $n"
mt() {
  out=$(python3 tools/matchtool.py check "$1" 2>&1)
  ok=$(grep -c ': MATCH' <<<"$out"); bad=$(grep -E '^\S+ @0x' <<<"$out" | grep -vc ': MATCH')
  if [ "$ok" = 0 ]; then echo "$1: COMPILE FAIL"; grep -iE 'error|:[0-9]+:' <<<"$out" | head -8
  elif [ "$bad" != 0 ]; then echo "$1: $ok MATCH, $bad other"; grep -E '^\S+ @0x' <<<"$out" | grep -v ': MATCH' | head -5
  fi
}
export -f mt
printf '%s\n' $files | xargs -r -P"${JOBS:-2}" -I{} bash -c 'mt "$@"' _ {}
echo "matchtool checked $n"
python3 "$HERE/objcmp.py" cmp "$BASE" $files
