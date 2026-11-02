# Docs generators

Scripts that generate parts of `docs/`. They were written during the
agent-driven documentation pass and moved here so regeneration is
reproducible; they are process tooling, not product code.

| Script | Writes | Inputs |
| --- | --- | --- |
| `gen.py` | `docs/reference/tu-map.md`, `docs/reference/functions/*.md`, and `docs/engine/*.md` from `templates/*.md` (expands `{{KEYFUNCS}}` / `{{TUS}}`) | research name tables via `load.py` |
| `classtree.py` | `docs/reference/class-tree.md` | research class tables |
| `savevars_doc.py` | `docs/reference/save-variables.md` | research save-variable schema |
| `gen_classes.py` | `docs/reference/classes/*.md`, `hidden-types.md` | the ELF + research name tables |
| `linkcheck.py` | nothing; reports broken links in `docs/` | `docs/` only |

Research inputs are read from `$SLY2_RESEARCH` (default `../research`
next to the repo). Templates live in `templates/`; edit those, not the
generated `docs/engine/*.md`.

Known gaps:
- `load.py` still reads the v2 name tables; until it is switched to the
  v3 tables (or to `docs/reference/name-confidence.csv`), running
  `gen.py` would downgrade names in the generated pages. Don't run it
  yet.
- Source paths in generated pages must follow the current tree; after
  regenerating, run `linkcheck.py` (0 broken after the restructure).
