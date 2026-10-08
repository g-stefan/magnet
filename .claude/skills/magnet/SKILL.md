---
name: magnet
description: >-
  How to use magnet, the XYO command line Quantum Script interpreter with
  the 31 standard extensions built in (Application, ApplicationVersion,
  Base16/32/64, Buffer, Console, Crypt, CSV, DateTime, File, HTTP, Job,
  JSON, Make, Math, MD5, OpenSSL/HTTPS, Pixel32, ProcessInteractive,
  Random, SHA256, SHA512, Shell, ShellFind, Socket, SSHRemote, Task,
  Thread, URL, XML): running scripts (magnet script.js args, --run "code",
  --run="code", --cmd for .cmd launchers and #!/usr/bin/env -S magnet
  --cmd scripts, --execution-time, --version, --help, --license); option
  parsing stops at the script, later arguments go to the script (read with
  Application.getArgument / hasFlag / getFlagValue); exit codes
  (Script.exit(n), Script.setExitCode(n), uncaught exception = 1; magnet
  4.9.0 and earlier always exited 0); requireExtension is
  requireInternalExtension in magnet (no DLL search, non-bundled
  extensions fail); include search order; threads need their own
  requireExtension calls and fail silently. Use when writing, running or
  debugging magnet scripts or launchers, calling magnet from build
  scripts, or working inside the magnet repository.
---

# magnet

Quantum Script interpreter with every standard extension built in. A tool,
not a library. The language is Quantum Script — see the `quantum-script`
skill for it (no hoisting, `typeof(x)` needs parentheses, `&&` / `||`
return booleans, no `let` / `const` / arrow / `class` / regex literals,
statements and blocks end with `;`). Each extension's API is in its own
skill / repository, `quantum-script--<lowercase name>`; the bundle itself
is the `quantum-script--magnet` skill.

Full documentation: `docs/` in the magnet repository
(`X:\Storage\XYO\Gitea\CPP\magnet\docs` on this machine): README,
getting-started, **command-line**, **scripting**, reference. The whole
tool is `source/XYO/Magnet/Application.cpp` (about 180 lines). When in
doubt, run it: `magnet --run "..."`.

## Run

| Want | Command |
|------|---------|
| Run a script with arguments | `magnet script.js in.txt --force` |
| Run one line | `magnet --run "Script.requireExtension('Console'); Console.writeLn(6 * 7);"` (single quotes inside avoid escaping) |
| Same, `quantum-script` form | `magnet "--run=Script.exit(3);"` |
| Time it | `magnet --execution-time script.js` → last line `Execution time: <N> ms` |
| Version / help / license | `magnet --version`, `--help` / `--usage`, `--license` (print, exit 0, run nothing) |
| Windows command in Quantum Script | `.cmd` file, lines 1-2: `@magnet --cmd "%~f0" %*` / `@exit /b %ERRORLEVEL%`, script from line 3 |
| Linux executable script | line 1 `#!/usr/bin/env -S magnet --cmd`, line 2 any text (skipped), `chmod +x` |

## Hard rules

1. **Options only before the script** (or before the code of `--run`).
   Everything after it goes to the script unchanged, even `--cmd`, `--run`,
   `--license`, `--execution-time`. `magnet build.js --execution-time`
   prints no time; write `magnet --execution-time build.js`. Unknown
   options before the script are ignored.
2. **Exit code**: `Script.exit(n)` stops now with `n`;
   `Script.setExitCode(n)` sets it for the end; uncaught exception,
   compile error, missing script → `1` (a `setExitCode` before the
   exception is ignored). **magnet 4.9.0 and earlier always exit 0** when
   the script ends without an exception; with those, throw to signal
   failure. Check the version with `magnet --version` (fixed in 5.0.0).
3. **All output, errors included, goes to stdout.** Errors:
   `Error: <message>` then `- file <name> line <n>` lines;
   `Error: Unable to open "x.js"`; `Error: Compile error in x.js line 7`
   (`--run`: `... on line 1`).
4. **Load every extension you use**, `Console` included:
   `Script.requireExtension("Shell");`. In magnet `requireExtension` is
   `requireInternalExtension`: only the 31 bundled extensions, never a
   DLL / `.so`; anything else fails `Error: Unable to open "SCard"`. There
   is no `Version` extension (it is `ApplicationVersion`). `Magnet` is
   already loaded.
5. **Arguments**: `Script.requireExtension("Application");`.
   `Application.getArgument(0)` is the script (with `--run "code"`: the
   code), `getArgument(1)` the first argument for it — `getArgument(k)`
   skips everything starting with `--`, `undefined` past the end.
   `hasFlag("force")` = `--force` or `--force=...`;
   `getFlagValue("out", default)` reads `--out=value`. `getCmdN()` /
   `getCmdS(k)` give the raw line (index 0 = magnet, magnet's own options
   included).
6. **Includes**: `Script.include("x.js")` / `includeOnce` search the
   current folder, then magnet's folder, then the including file's
   folder. For certainty: `Shell.getFilePath(Application.getArgument(0)) + "/x.js"`.
7. **Threads have their own engine**: require the extensions again inside
   the thread function. An error inside a thread is not printed — a
   thread calling `Console.writeLn` without requiring `Console` does
   nothing, silently.
8. **No `#!` support by the parser**: a plain `#!/usr/bin/env magnet` line
   is a compile error on line 1; use `--cmd` (skips 2 lines). Line numbers
   in errors count the skipped lines.
9. **Builds**: static Windows build = one `magnet.exe`, no DLLs. The
   dynamic Windows / Linux builds need `quantum-script--magnet` and all
   extension libraries from the XYO SDK (`~/.fabricare/<platform>/bin`) on
   the `PATH` / library path.

## Script skeleton

```javascript
// tool.js - magnet tool.js input [output] [--force]
Script.requireExtension("Console");
Script.requireExtension("Application");
Script.requireExtension("Shell");

if (Application.hasFlag("help") || Script.isUndefined(Application.getArgument(1))) {
	Console.writeLn("usage: magnet tool.js input [output] [--force]");
	Script.exit(1);
};

var input = Application.getArgument(1);
var output = Application.getArgument(2, input + ".out");
if (Shell.fileExists(output) && !Application.hasFlag("force")) {
	Console.writeLn("error: " + output + " exists, use --force");
	Script.exit(2);
};

var text = Shell.fileGetContents(input);       // undefined if unreadable
if (Script.isNil(text)) {
	Console.writeLn("error: cannot read " + input);
	Script.exit(3);
};
Shell.filePutContents(output, text.toUpperCaseASCII());
```

Common calls: `Shell.system(cmd)` (through the shell, returns exit
code), `Shell.getFileList("dir/*.ext")`, `Shell.mkdirRecursivelyIfNotExists`,
`Shell.copyFile`, `Shell.getFileName` / `getFilePath`,
`JSON.decode` / `JSON.encodeWithIndentation` (key order not kept),
`SHA512.fileHash(file)`, `new DateTime()`,
`DateTime.timestampInMilliseconds()`, `new Thread()` + `start(fn)` +
`join()`.

## Calling magnet

```bat
rem Windows batch
magnet build.js --release
if errorlevel 1 exit /b 1
```

```bash
magnet check.js || exit 1
```

```js
// fabricare script
exitIf(Shell.system("magnet tool.js input.txt"));
```

## Working inside this repository

- One fabricare project, `magnet` (`make: exe`, source `source/XYO/Magnet`,
  depends on `quantum-script--magnet`). See the `fabricare` skill:
  `fabricare make`, then `fabricare test` (needs `output/bin/magnet`),
  `fabricare install`, `fabricare clean`. Also check Linux through WSL.
- `fabricare/test.js` runs magnet via `Shell.system(... > temp/test.output.txt)`
  and checks exit codes and output with `exitIfTest`; add a check there
  (and a script in `test/`) for every behavior change.
- `Application::main`: option loop that `break`s at the first non `--`
  argument or after `--run`; one `initExecutive` → execute → read
  `ExecutiveX::getExitCode()` **before** `endProcessing()` → return.
  `initExecutive` registers Magnet and replaces `requireExtension`.
- Keep `docs/`, `README.md` and this skill in step with `Application.cpp`
  (the outputs in the docs are real tool output).
- Files are CRLF. Licensing follows REUSE (`python -m reuse lint`):
  `source/`, `docs/`, `README.md` MIT; config, `fabricare/`, `test/`,
  `version.json`, `.claude/` Unlicense; every new top level file or folder
  needs a `Files:` entry in `.reuse/dep5`.
