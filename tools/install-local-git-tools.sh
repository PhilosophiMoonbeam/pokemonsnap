#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'EOF'
Usage: install-local-git-tools.sh [--help]

Install this repository's five local Git helper scripts and protective hooks
into the Git common directory. Configure clone-local aliases and push defaults.

The scripts are copied, not tracked as Git config. `git pr-preflight --build`
still requires the private archived local-tools/binutils tree, which is not
included in tracked files.
EOF
}

if (( $# )); then
    if (( $# == 1 )) && [[ $1 == -h || $1 == --help ]]; then
        usage
        exit 0
    fi
    usage >&2
    exit 2
fi

if ! git rev-parse --git-dir >/dev/null 2>&1; then
    echo "error: run this installer inside a Git working tree" >&2
    exit 1
fi

common_dir=$(git rev-parse --path-format=absolute --git-common-dir)
source_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/migration-git" && pwd)
expected_https=https://github.com/PhilosophiMoonbeam/pokemonsnap.git
expected_ssh=git@github.com:PhilosophiMoonbeam/pokemonsnap.git
origin_url=$(git remote get-url origin 2>/dev/null || true)
if [[ $origin_url != "$expected_https" && $origin_url != "$expected_ssh" ]]; then
    echo "error: origin must be exactly $expected_https or $expected_ssh (got: ${origin_url:-missing})" >&2
    exit 1
fi

hooks_path=$(git config --get core.hooksPath 2>/dev/null || true)
if [[ -n $hooks_path && $hooks_path != "$common_dir/hooks" ]]; then
    echo "error: core.hooksPath is configured as '$hooks_path'; refusing to install disabled hooks" >&2
    exit 1
fi

local_tools=$common_dir/local-tools
hooks_dir=$common_dir/hooks
for name in new-pr-worktree sync-upstream-main pr-preflight; do
    if [[ ! -f $source_dir/$name || ! -x $source_dir/$name ]]; then
        echo "error: helper source is not a regular executable file: $source_dir/$name" >&2
        exit 1
    fi
done
for name in pre-commit pre-push; do
    if [[ ! -f $source_dir/$name || ! -x $source_dir/$name ]]; then
        echo "error: hook source is not a regular executable file: $source_dir/$name" >&2
        exit 1
    fi
done
for name in new-pr-worktree sync-upstream-main pr-preflight; do
    if [[ -e $local_tools/$name ]] && ! cmp -s -- "$source_dir/$name" "$local_tools/$name"; then
        echo "error: refusing to replace differing helper: $local_tools/$name" >&2
        exit 1
    fi
done
for name in pre-commit pre-push; do
    if [[ -e $hooks_dir/$name ]] && ! cmp -s -- "$source_dir/$name" "$hooks_dir/$name"; then
        echo "error: refusing to replace differing hook: $hooks_dir/$name" >&2
        exit 1
    fi
done

upstream_url=$(git config --local --get-all remote.upstream.url || true)
if [[ -z $upstream_url ]]; then
    git remote add upstream https://github.com/ethteck/pokemonsnap.git
elif [[ $upstream_url != https://github.com/ethteck/pokemonsnap.git && $upstream_url != git@github.com:ethteck/pokemonsnap.git ]]; then
    echo "error: upstream already has a different URL; refusing to overwrite it" >&2
    exit 1
fi

mkdir -p -- "$local_tools" "$hooks_dir"

for name in new-pr-worktree sync-upstream-main pr-preflight; do
    install -m 755 -- "$source_dir/$name" "$local_tools/$name"
done
for name in pre-commit pre-push; do
    install -m 755 -- "$source_dir/$name" "$hooks_dir/$name"
done

git config --local alias.pr-new '!f() { "$(git rev-parse --path-format=absolute --git-common-dir)/local-tools/new-pr-worktree" "$@"; }; f'
git config --local alias.upstream-sync '!f() { "$(git rev-parse --path-format=absolute --git-common-dir)/local-tools/sync-upstream-main" "$@"; }; f'
git config --local alias.pr-preflight '!f() { "$(git rev-parse --path-format=absolute --git-common-dir)/local-tools/pr-preflight" "$@"; }; f'
git config --local remote.pushDefault origin
exclude_file=$(git rev-parse --path-format=absolute --git-common-dir)/info/exclude
mkdir -p -- "$(dirname -- "$exclude_file")"
for pattern in '/AGENTS.md' '/.omp/' '/.cpp-linter_cache/' '/tools/cross/' '/tools/local-debs/'; do
    if [[ ! -f $exclude_file ]] || ! grep -Fqx -- "$pattern" "$exclude_file" 2>/dev/null; then
        printf '%s\n' "$pattern" >> "$exclude_file"
    fi
done

printf 'Installed local Git helpers and protective hooks in %s\n' "$common_dir"
