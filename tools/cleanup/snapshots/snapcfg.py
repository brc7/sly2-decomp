"""Shared command-line/env configuration for the snapshot tools (imported before common.py, so --help works without
an ELF or snapshots).

Every option can also be set with the environment variable in brackets; the command line wins.
  --root REPO        [SNAP_ROOT]    decomp checkout (config/, include/, src/); default: this repository
  --elf PATH         [SNAP_ELF]     the game's main ELF (SCUS_973.16); default: REPO/disc/SCUS_973.16
  --work DIR         [SNAP_WORK]    working directory: ram/ (input RAM images), objects/, *.json, REPORT.md;
                                    default: REPO/build/snapshots (git-ignored: game-derived data never goes in git)
  --census DIR       [SNAP_CENSUS]  output of tools/cleanup/census (offsets.json); default: REPO/build/census
  --census-config F  [SNAP_CENSUS_CONFIG] census config JSON ("globals": {TYPE: [names]}), optional
  --views F          [SNAP_VIEWS]   view-plan JSON ({"views": [{file, line, view, fix, real_type, type_chain,
                                    candidate_type, milestone, ...}]}), optional for most steps
  --vtjoin F         [SNAP_VTJOIN]  vtjoin.json (from vtjoin.py); default: WORK/vtjoin.json
"""
import argparse, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.realpath(os.path.join(HERE, '..', '..', '..'))
OPTS = [('root', 'SNAP_ROOT'), ('elf', 'SNAP_ELF'), ('work', 'SNAP_WORK'), ('census', 'SNAP_CENSUS'),
        ('census-config', 'SNAP_CENSUS_CONFIG'), ('views', 'SNAP_VIEWS'), ('vtjoin', 'SNAP_VTJOIN')]
_args = None


def parse(doc, extra=None):
    """Parse the shared options (+ extra(parser) for script-specific ones) once; export them to the environment."""
    global _args
    if _args is not None:
        return _args
    ap = argparse.ArgumentParser(description=doc, epilog=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    for name, env in OPTS:
        ap.add_argument('--' + name, metavar=env, help=argparse.SUPPRESS)
    if extra:
        extra(ap)
    _args = ap.parse_args()
    for name, env in OPTS:
        v = getattr(_args, name.replace('-', '_'))
        if v:
            os.environ[env] = os.path.abspath(v)
    return _args


def get(key):
    """Resolved path for a shared option (None for unset optional inputs)."""
    env = dict(OPTS)[key]
    v = os.environ.get(env)
    if v:
        return v
    root = get('root') if key != 'root' else REPO
    work = get('work') if key not in ('root', 'work', 'elf') else None
    return {'root': REPO, 'elf': os.path.join(root, 'disc', 'SCUS_973.16'),
            'work': os.path.join(root, 'build', 'snapshots'), 'census': os.path.join(root, 'build', 'census'),
            'vtjoin': work and os.path.join(work, 'vtjoin.json')}.get(key)


def need(key, what):
    p = get(key)
    if not p or not os.path.exists(p):
        sys.exit(f'missing {what}: {p or "(unset)"}; pass --{key} or set {dict(OPTS)[key]}')
    return p
