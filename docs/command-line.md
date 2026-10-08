# Command line

```
magnet [options] script.js [arguments ...]
magnet [options] --run "code" [arguments ...]
magnet [options] --run="code" [arguments ...]
magnet --help | --usage | --license | --version
```

## Options

| Option | Effect |
|--------|--------|
| `script.js` | run the script; the first argument that does not start with `--` |
| `--run "code"` | run `code` (the next argument) instead of a file |
| `--run="code"` | the same, the code after `=` (the form of the `quantum-script` interpreter) |
| `--cmd` | run the script, skipping its first 2 lines (`.cmd` launchers, `#!` scripts) |
| `--execution-time` | after the script, print `Execution time: <N> ms` |
| `--execution-time-cmd` | `--execution-time` + `--cmd` |
| `--help`, `--usage` | print the usage, exit `0` |
| `--version` | print `version X.Y.Z build N [date time]`, exit `0` |
| `--license` | print the MIT license, exit `0` |

`--help`, `--usage`, `--version` and `--license` act at once: nothing is
run, the arguments after them are ignored. With no script and no `--run`,
magnet prints the usage and exits `0`.

Unknown options (before the script) are ignored. There is no `--compile`,
`--verify` or `@file` as in the `quantum-script` interpreter: magnet runs
source scripts only.

## Where option parsing stops

Options are read **up to the script** (or up to the code of `--run`).
Everything after it is left to the script, unchanged, **including
arguments that look like magnet options**:

```
magnet --execution-time build.js --cmd --run x --license
       \_____ magnet _____/ \_____ the script's arguments _____/
```

`build.js` runs once, completely, and sees `--cmd`, `--run`, `x`,
`--license` as its own arguments; no time is printed for
`magnet build.js --execution-time`, because `--execution-time` belongs to
the script there. Put magnet's options **before** the script.

The same for `--run`: `magnet --run "code" --execution-time` passes
`--execution-time` to the code.

`magnet` 4.9.0 and earlier read options everywhere on the line: an
argument `--cmd` or `--run` meant for a script changed how magnet ran it,
and `--license` printed the license and then ran the script. See
[Reference](reference.md#changes).

## What the script sees

The whole command line, as the operating system gave it, through the
`Application` extension:

| Call | `magnet --execution-time tool.js in.txt --force` | `magnet --run "code" a b` |
|------|-----------|-----------|
| `Application.getCmdN()` | `5` | `5` |
| `Application.getCmdS(0)` | path of `magnet` | path of `magnet` |
| `Application.getCmdS(1)` | `--execution-time` | `--run` |
| `Application.getArgument(0)` | `tool.js` | `code` |
| `Application.getArgument(1)` | `in.txt` | `a` |
| `Application.hasFlag("force")` | `true` | `false` |

`getArgument(k)` skips every argument that starts with `--`, so with
magnet options before the script `getArgument(0)` is still the script (or
the code of `--run`) and `getArgument(1)` its first argument. A `--run="code"`
argument starts with `--`, so there `getArgument(0)` is the first argument
after it.

The current folder is the one magnet was started in; the script's own
folder is `Shell.getFilePath(Application.getArgument(0))`.

## Output

Magnet itself writes only:

- the usage, version or license text, for those options;
- `Execution time: <N> ms` after the script, with `--execution-time`
  (only when the script ended without an uncaught exception);
- error messages, below.

Everything else comes from the script (`Console.write`, `Console.writeLn`,
programs it starts). All of it goes to **standard output**, errors
included.

## Errors

| Situation | Output | Exit code |
|-----------|--------|-----------|
| Script file not found | `Error: Unable to open "nosuch.js"` | `1` |
| Syntax error | `Error: Compile error in build.js line 7` (`--run`: `Error: Compile error on line 1`) | `1` |
| Uncaught exception | `Error: <message>` then the stack trace, one `- file <name> line <n>` per level | `1` |
| `--run` without code, or with `""` | `Error: No code specified!` | `1` |
| Extension that is not bundled | `Error: Unable to open "SCard"` | `1` |

```
magnet test\test.throw.js
Error: test throw
- file test\test.throw.js line 10
```

An exception ends the script; a `Script.setExitCode(n)` done before it is
ignored, the exit code is `1`.

## Exit codes

| Case | Exit code |
|------|-----------|
| The script ended normally | `0`, or the last value given to `Script.setExitCode(n)` |
| `Script.exit(n)` | `n`; the script stops at once |
| `Script.exit()` / `Script.setExitCode()` with no value | `0` |
| Uncaught exception, compile error, script not found | `1` |
| `--help`, `--usage`, `--version`, `--license`, no arguments | `0` |

The same for `script.js`, `--cmd` and `--run`. On Linux the exit code is
taken modulo 256 by the system, as for any program.

## Examples

```
magnet build.js
magnet build.js --release --out=dist
magnet --execution-time benchmark.js 1000000
magnet --run "Script.requireExtension('Console'); Console.writeLn('ok');"
magnet "--run=Script.exit(3);"
magnet --cmd tool.cmd arg1 arg2
magnet --version
```
