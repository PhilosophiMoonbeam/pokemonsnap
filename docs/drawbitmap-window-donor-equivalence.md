# Drawbitmap window/donor equivalence note

Current state on `hermes/final-stretch`:

- `src/window/8456E0.c::func_80371F54_845704` and `src/26D80.c::drawbitmap`
  are effectively the same guarded helper after symbol renaming and recent
  guarded load-path expansions.
- The meaningful remaining donor/window divergence is not in the helper body,
  but in the surrounding `spDraw` traversal logic.

Helper symbol mapping

- `func_80371F54_845704` <-> `drawbitmap`
- `D_803A6640_879DF0` <-> `prev_bmbuf`
- `D_803A6644_879DF4` <-> `sp_attr`
- `D_803A6648_879DF8` <-> `scissor_xmin`
- `D_803A664C_879DFC` <-> `scissor_ymin`
- `D_803A6650_879E00` <-> `scissor_xmax`
- `D_803A6654_879E04` <-> `scissor_ymax`

Key divergence window

Window path (`src/window/8456E0.c`):
- starts at `b = s->bitmap + s->istart`
- computes `b_end = b + s->nbitmaps`
- advances with `b += s->istep`

Donor path (`src/26D80.c`):
- starts at `b = s->bitmap`
- iterates with `i < s->nbitmaps`
- advances with `b++`

Current live/default context

- `src/window/element.c` initializes donor-style traversal defaults:
  - `istart = 0`
  - `istep = 1`
- `src/window/845160.c` constructors copy Sprite templates verbatim.
- Current live layout callers do not patch `istart/istep` after construction.

Implication

- Future drawbitmap-family work should treat the helper body as largely aligned.
- New experiments should focus on the surrounding `spDraw`/caller traversal and
  state assumptions, not blind helper-body donorization.
- Do not assume donor direct-enable is the better active lane; earlier scratch
  measurement showed donor drawbitmap had a worse raw `.text` floor than the
  window helper on this branch.
