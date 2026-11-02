#!/usr/bin/env bash
# Run the Sly 2 decomp toolchain inside a linux/amd64 container (anywhere the `docker` CLI works).
# On Apple Silicon wibo can't run in Docker: use native matchtool/progress for game code and this script for
# the Linux-compiler SDK files — see docs/guides/setup.md.
#
#   tools/dev.sh image                 build the toolchain image (once, or after Dockerfile changes)
#   tools/dev.sh setup                 download compilers into tools/ (tools/setup_all.sh in the container);
#                                      on macOS also the host pieces: tools/wibo-macos, ~/m2c, rabbitizer check
#   tools/dev.sh doctor                check that every compiler actually executes (x86 emulation sanity check)
#   tools/dev.sh check FILE [func..]   match-check one source file (tools/matchtool.py check)
#   tools/dev.sh progress              rebuild + verify everything (tools/progress.py); JOBS=N for parallelism
#   tools/dev.sh shell                 interactive shell in the container
#   tools/dev.sh run CMD...            run any command in the container
#
# Every non-interactive command is also logged to build/logs/<cmd>-<timestamp>.log.
# Put disc/SCUS_973.16 in place first (python3 tools/extract_elf.py <your ISO>, runs natively on macOS).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
IMAGE="${SLY2_IMAGE:-sly2-decomp-tools:latest}"
JOBS="${JOBS:-$( (sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4) )}"
cmd="${1:-help}"; [ $# -gt 0 ] && shift

docker_run() {  # docker_run [-it] CMD...
  # (no arrays: macOS ships bash 3.2, where an empty "${arr[@]}" trips `set -u`)
  local tty=""; if [ "${1:-}" = "-it" ]; then tty="-it"; shift; fi
  docker run --rm $tty --platform linux/amd64 \
    -v "$ROOT":/sly2 -w /sly2 -e JOBS="$JOBS" -e PYTHONDONTWRITEBYTECODE=1 \
    "$IMAGE" "$@"
}
logged() {  # logged NAME CMD...   (tee output to build/logs, keep the command's exit status)
  local name=$1; shift; mkdir -p "$ROOT/build/logs"
  local log="$ROOT/build/logs/$name-$(date +%Y%m%d-%H%M%S).log"
  { echo "# $(date)  $*"; echo "# host: $(uname -sm)  image: $IMAGE  JOBS=$JOBS"; } > "$log"
  set +e; "$@" 2>&1 | tee -a "$log"; local rc=${PIPESTATUS[0]}; set -e
  echo "# exit $rc" >> "$log"; echo "(log: ${log#$ROOT/})"; return "$rc"
}

host_setup_macos() {  # host-side pieces of the Apple Silicon hybrid setup (see docs/guides/setup.md)
  echo "== macOS host: wibo-macos (runs the Windows compilers natively under Rosetta 2)"
  if [ -x "$ROOT/tools/wibo-macos" ]; then echo "  ok (already there)"; else
    curl -fsSL -o "$ROOT/tools/wibo-macos" "https://github.com/decompals/wibo/releases/download/${WIBO_VERSION:-1.2.0}/wibo-macos"
    chmod +x "$ROOT/tools/wibo-macos"; echo "  downloaded tools/wibo-macos"; fi
  echo "== macOS host: m2c checkout (m2c_macros.h for m2c-generated and splicewrap sources)"
  if [ -d "$HOME/m2c" ]; then echo "  ok (already there)"; else
    git clone -q https://github.com/matt-kempster/m2c "$HOME/m2c" && echo "  cloned ~/m2c"; fi
  echo "== macOS host: python rabbitizer"
  if python3 -c "import rabbitizer" 2>/dev/null; then echo "  ok (already there)"; else
    echo "  !! run: python3 -m pip install rabbitizer"; fi
  echo "== macOS host: Rosetta 2"
  if /usr/bin/pgrep -q oahd 2>/dev/null || arch -x86_64 /usr/bin/true 2>/dev/null; then echo "  ok"; else
    echo "  !! run: softwareupdate --install-rosetta --agree-to-license"; fi
}

case "$cmd" in
  image)    logged image docker build --platform linux/amd64 -t "$IMAGE" -f "$ROOT/tools/docker/Dockerfile" "$ROOT/tools/docker" ;;
  setup)    logged setup docker_run bash tools/setup_all.sh
            if [ "$(uname -s)" = Darwin ]; then logged setup-macos host_setup_macos; fi ;;
  doctor)   logged doctor docker_run bash tools/doctor.sh ;;
  check)    logged check docker_run python3 tools/matchtool.py check "$@" ;;
  progress) logged progress docker_run python3 tools/progress.py "$@" ;;
  run)      logged run docker_run "$@" ;;
  shell)    docker_run -it bash ;;
  *)        sed -n '2,16p' "$0" | sed 's/^# \{0,1\}//'; [ "$cmd" = help ] ;;
esac
