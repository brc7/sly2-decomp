#!/usr/bin/env bash
# Run every check a branch has to pass before it is pushed, and print one pass/fail summary.
#
#   tools/verify.sh            all checks
#   tools/verify.sh --no-link  skip the full link (needs ld.lld + llvm-objcopy, see docs/guides/full-link.md)
#
# 1. progress, native    rm build/progress_cache.json, then tools/progress.py (game code: wibo compilers)
# 2. progress, Docker    tools/dev.sh progress (the Linux-compiler SDK files; reuses the native results from 1)
#                        PASS = every source built and 6662/6662 functions match
# 3. matched.csv         unchanged against git (a changed row means a function's result changed)
# 4. doc links           tools/docs/linkcheck.py
# 5. full link           tools/link.py: ld.lld errors must be 0; differing bytes against the ELF are reported
#                        (informational: compare with the Status table in docs/guides/full-link.md)
# Outputs of each step are in build/logs/verify-<step>.log.
set -uo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
LOGS=build/logs; mkdir -p "$LOGS"
LINK=1; [ "${1:-}" = "--no-link" ] && LINK=0
summary=""; failed=0
result() {  # result STEP PASS|FAIL|SKIP DETAIL
  summary="$summary$(printf '  %-20s %-5s %s' "$1" "$2" "$3")
"
  [ "$2" = FAIL ] && failed=1
  printf '== %s: %s %s\n' "$1" "$2" "$3"
}
run() {  # run LOGNAME CMD...  (output to the log and the terminal; returns the command's status)
  local log="$LOGS/verify-$1.log"; shift
  "$@" 2>&1 | tee "$log"; return "${PIPESTATUS[0]}"
}

if [ $LINK -eq 1 ]; then   # fail fast, before the long runs, if the link tools are missing
  python3 tools/link.py --check-tools || { echo "verify.sh: fix the LLVM tools first, or run with --no-link"; exit 1; }
fi

echo "== 1/5 progress (native)"
rm -f build/progress_cache.json
run progress-native python3 tools/progress.py
native=$(grep 'matched functions' "$LOGS/verify-progress-native.log" | tail -1)
result "progress (native)" INFO "${native:-no total printed} (SDK files that need Docker are expected to fail here)"

echo "== 2/5 progress (Docker)"
run progress-docker tools/dev.sh progress; rc=$?
total=$(grep 'matched functions' "$LOGS/verify-progress-docker.log" | tail -1)
got=$(echo "$total" | sed -n 's/.*matched functions: \([0-9]*\)\/\([0-9]*\) .*/\1 \2/p')
if [ $rc -eq 0 ] && [ -n "$got" ] && [ "${got% *}" = "${got#* }" ]; then
  result "progress (all)" PASS "$total"
else
  result "progress (all)" FAIL "${total:-no total} (exit $rc; see $LOGS/verify-progress-docker.log)"
fi

echo "== 3/5 config/matched.csv"
if git diff --quiet -- config/matched.csv; then result "matched.csv" PASS "unchanged"
else result "matched.csv" FAIL "changed: git diff config/matched.csv"; fi

echo "== 4/5 doc links"
run linkcheck python3 tools/docs/linkcheck.py; rc=$?
lc=$(tail -1 "$LOGS/verify-linkcheck.log")
if [ $rc -eq 0 ]; then result "doc links" PASS "$lc"; else result "doc links" FAIL "$lc"; fi

if [ $LINK -eq 1 ]; then
  echo "== 5/5 full link"
  run link python3 tools/link.py; rc=$?
  log="$LOGS/verify-link.log"
  lderr=$(sed -n 's/^ld\.lld: exit [0-9]*, \([0-9]*\) errors.*/\1/p' "$log" | tail -1)
  diff=$(awk '/^differing bytes by class:/{f=1;next} f&&/^$/{f=0} f{s+=$1} END{print s+0}' "$log")
  if [ $rc -eq 0 ] && [ "${lderr:-x}" = 0 ]; then result "full link" PASS "$diff bytes differ from the ELF"
  else result "full link" FAIL "exit $rc, ld.lld errors: ${lderr:-?} (see $log)"; fi
else
  result "full link" SKIP "--no-link"
fi

printf '\nsummary:\n%s' "$summary"
if [ $failed -eq 0 ]; then echo "ALL CHECKS PASSED"; else echo "SOME CHECKS FAILED"; fi
exit $failed
