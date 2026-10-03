# Fresh Ubuntu server setup

This runbook prepares a private checkout on a fresh Ubuntu 26.04.1 host. Ubuntu 26.04.1 is the target platform; this procedure has not been exercised on that OS. It does not install system packages, install uv/Rust, access a server, resume matching, or run matching probes.

## Branch and worktree boundaries

Use the cumulative private branch only for this private working copy:

```sh
git clone --branch hermes/final-stretch --single-branch \
  https://github.com/PhilosophiMoonbeam/pokemonsnap.git pokemonsnap
cd pokemonsnap
git remote add upstream https://github.com/ethteck/pokemonsnap.git
git fetch upstream
git switch hermes/final-stretch
```

`origin` is PUBLIC: `https://github.com/PhilosophiMoonbeam/pokemonsnap.git`. Publish only vetted, portable source/research there. Keep restricted binary evidence, ROMs, extracted copyrighted assets, and private configuration/state off public remotes. `upstream` (`https://github.com/ethteck/pokemonsnap.git`) and its `main` branch are read-only. `hermes/final-stretch` is cumulative private work, not an upstream PR base. For an optional contribution, create a separate, clean worktree rooted at the current `upstream/main`, and transfer only the intended contribution into it; this contribution workflow is not required for migration. For example:

```sh
git fetch upstream
git worktree add ../pokemonsnap-pr -b pr/match-topic upstream/main
```

Do not add ROMs, extracted copyrighted assets, binary evidence, or private configuration/state to Git or publish them to either remote. The public origin may receive only vetted, portable source/research.

## Host prerequisites

Install these prerequisites yourself before running the bootstrap. The script intentionally does not run `apt`, install uv or Rust, or invoke installer scripts. Example Ubuntu package command (review and run manually, with appropriate privileges):

```sh
sudo apt-get update
sudo apt-get install --no-install-recommends \
  binutils-mips-linux-gnu ninja-build git python3 python3-venv \
  build-essential curl ca-certificates coreutils rsync
```

Also required:

- `uv` installed for your login user, available on `PATH` (the project has a `uv.lock`; the bootstrap uses frozen synchronization).
- Rust stable installed for your login user, with `cargo` available on `PATH`; `rustup default stable` selects the toolchain.
- `pigment64` installed and available on `PATH`, for example with `cargo install pigment64` after Rust is ready.
- Bash, `iconv`, and the standard utilities `sha1sum`, `cmp`, `cp`, and `mktemp` (provided by the listed Ubuntu packages).
- Network access for the frozen Python environment and the one-time `configure.py --setup` downloads of the project-configured IDO compilers and asm-processor.

Install uv and Rust as the account that will run the build, never as root. For example, after the package prerequisites are installed:

```sh
curl -LsSf https://astral.sh/uv/install.sh | sh
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y --default-toolchain stable
. "$HOME/.cargo/env"
rustup default stable
cargo install pigment64
```

Start a fresh shell or refresh its `PATH` before running the bootstrap. `binutils-mips-linux-gnu` supplies MIPS binutils; modern host C compilers are not a substitute for the project's configured normal IDO compiler. The bootstrap leaves IDO compiler selection to `configure.py --setup` and does not replace those configured compilers with system compilers.

## Restore portable research and Git protections

From the cloned checkout, restore the public source bundle before applying private state:

```sh
uv sync --frozen
uv run python tools/migration_archive.py restore research/portable-research.tar.gz --root .
tools/install-local-git-tools.sh
```


## ROM and any private transfer inputs

Obtain and place a legally usable US Pokémon Snap ROM at the checkout root as `pokemonsnap.z64`. Before extraction or building, the script requires its SHA-1 to be exactly:

```text
edc7c49cc568c045fe48be0d18011c30f393cbaf
```

Never commit or upload the ROM. Extraction/build may produce copyrighted assets; keep those local and restricted as well.

The private transfer contains `private-state.tar.gz`, `local-refs.bundle`, `production-checkpoint.tar.gz`, `classification.json`, `inventory.json`, and `SHA256SUMS`. Transfer the directory privately, outside the public fork. The private archive has 23,358 members; it preserves private research, binary probe/reference evidence, root scratch, configured tools, local guidance (`AGENTS.md`), and `.git/local-tools/binutils`. The public `research/portable-research.tar.gz` has 5,997 research files plus its checksum manifest. The inventory covers all 37,246 initial paths. `production-checkpoint.tar.gz` retains the two pre-format guarded renderer sources with their paused-checkpoint hashes and is read-only historical source metadata: do not overlay it onto production source.

Verify the transferred artifact checksums before extracting anything:

```sh
cd /path/to/private-transfer
sha256sum -c SHA256SUMS
```

The archive paths are project-relative. Extract into a newly created, empty staging directory:

```sh
mkdir -p /path/to/private-transfer/staging
tar -xzf /path/to/private-transfer/private-state.tar.gz \
  -C /path/to/private-transfer/staging
```

From the checkout root, overlay staged private files without replacing clone files. Exclude only the root Git administration directory and archived `.omp` (historical local metadata with obsolete absolute paths); nested research repositories must retain their archived objects and refs. Credential-bearing Git configuration was excluded when packing.

```sh
rsync -a --ignore-existing --exclude '/.git/' --exclude '/.omp/' \
  /path/to/private-transfer/staging/ ./
```

Restore separately inventoried `.git/local-tools/binutils` into this clone's Git common directory, not the checkout's `.git` path (which may be a file in a worktree):

```sh
git_common_dir="$(git rev-parse --path-format=absolute --git-common-dir)"
mkdir -p "$git_common_dir/local-tools"
rsync -a --ignore-existing \
  /path/to/private-transfer/staging/.git/local-tools/binutils/ \
  "$git_common_dir/local-tools/binutils/"
```

Rebind inventoried absolute symlinks only after overlaying private files:

```sh
uv run python tools/relink-migration-symlinks.py \
  --inventory /path/to/private-transfer/inventory.json --root .
```

Only inventoried links whose targets were absolute within the original root are rebound to this checkout. Relative links are unchanged. The 50 pre-existing broken binutils documentation/manpage links are unchanged; no external symlink dependencies were found. Do not blindly copy `.git/config`, hooks, worktrees, or other repository administration state; the tracked installer handles local hooks. Keep the ROM private and verify its checksum too.

Inspect the refs bundle without changing the current branch or anchors:

```sh
git bundle verify /path/to/private-transfer/local-refs.bundle
git bundle list-heads /path/to/private-transfer/local-refs.bundle
```

If needed, recover research branches only under new namespaced refs after reviewing the listed heads; do not overwrite current branches or anchors blindly.

The complete restore order is: host prerequisites, clone, frozen dependency sync, public research restore, Git tooling installer, private checksum verification, staging extraction, private overlay, symlink rebinding, then bootstrap and checksum. Keep the public restore before private overlay; do not overlay `production-checkpoint.tar.gz` onto production source. The steps above include the full command sequence.

Optionally create a clean contribution worktree with the `git pr-new` workflow; this is not a migration action:

```sh
git pr-new match-topic
```

No server is configured by this preparation. Follow host prerequisites and the bootstrap below only when deployment preparation is requested; matching remains paused and must not resume automatically.

## Bootstrap and build

From the checkout root, first run the non-mutating readiness report:

```sh
tools/server-bootstrap.sh --help
tools/server-bootstrap.sh --check-only
```

A successful check requires the listed host tools, the repository lockfile, and the correctly checksummed ROM. A missing or mismatched ROM is reported as not ready. Full mode independently checks the ROM before it runs dependency synchronization, extraction, or a build; it refuses to proceed without the expected ROM.

Once prerequisites and ROM are ready, run from the checkout root:

```sh
tools/server-bootstrap.sh
sha1sum -c checksum.sha1
```

Full mode runs frozen `uv` dependency synchronization, project tool setup if needed, extraction/disassembly, then `ninja`. Around extraction, it backs up the tracked `asm/nonmatchings/window/847B60/func_80374714_847EC4.s`, restores the exact saved bytes if extraction overwrites/removes it, and invalidates the affected window object and final checksum so the subsequent build regenerates them. It does not run `configure.py --clean`, `git reset`, `git clean`, matching probes, or matching workflows. Keep the working copy private; inspect any local changes before pushing, and push only to `origin` when authorized.
