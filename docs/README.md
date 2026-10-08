# Magnet — Documentation

`magnet` is **Quantum Script with batteries included**: a command line
interpreter for Quantum Script (the JavaScript-like script language of the
XYO C++ stack) with the 31 standard extensions built in. Files,
directories, processes, JSON, XML, CSV, HTTP / HTTPS, sockets, SSH, threads,
jobs, hashes, encryption, images: everything is available to a script
without installing or finding any extension library.

```javascript
// hello.js
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("SHA512");

var files = Shell.getFileList("*.zip");
for (var i = 0; i < files.length; ++i) {
	Console.writeLn(SHA512.fileHash(files[i]) + "  " + files[i]);
};
```

```
magnet hello.js
```

It is a tool, not a library: there is nothing to link. The engine behind it
is the `quantum-script--magnet` extension, which C++ programs can embed
themselves (see that repository).

```
your scripts (.js, .cmd launchers, shebang scripts)
magnet                        <-- this tool: command line, runs the script, exit code
quantum-script--magnet        registers the 31 bundled extensions as internal extensions
quantum-script--application, --shell, --json, --xml, --http, --openssl, ... (the extensions)
quantum-script                the language: parser, VM, Script.*, standard library
xyo-system, xyo-cryptography, xyo-networking, xyo-pixel32, ...
```

## Why it exists

| Need | What `magnet` gives |
|------|---------------------|
| Write automation scripts (builds, deployment, file processing) in one language on Windows and Linux | the same script runs on both; the bundled `Shell`, `File`, `ProcessInteractive` hide the platform differences |
| No search for extension libraries | every bundled extension is *internal*: `Script.requireExtension("JSON")` never looks for `quantum-script--json.dll` |
| One file to copy | the static Windows build is a single `magnet.exe` with no DLL dependencies |
| Scripts that behave like commands | `.cmd` launchers on Windows and `#!` scripts on Linux (`--cmd`), arguments and exit codes passed through |
| Predictable environment | a script cannot pick up a stray extension DLL from its folder or the `PATH` |

## Concepts at a glance

| Need | Use |
|------|-----|
| Run a script | `magnet script.js [arguments ...]` |
| Run a line of code | `magnet --run "code" [arguments ...]` or `magnet --run="code"` |
| A Windows command written in Quantum Script | a `.cmd` file whose first two lines start `magnet --cmd` (see [Getting started](getting-started.md#4-scripts-as-commands)) |
| A Linux executable script | first line `#!/usr/bin/env -S magnet --cmd`, second line any text |
| Time a script | `magnet --execution-time script.js` |
| Load an extension | `Script.requireExtension("Shell");` — every bundled one, by name |
| Read the arguments | `Script.requireExtension("Application");` then `Application.getArgument(k)`, `hasFlag`, `getFlagValue` |
| Return an exit code | `Script.exit(n)` (stop now) or `Script.setExitCode(n)` (at the end) |
| Report a failure | throw: magnet prints `Error: ...` and the stack trace, exit code `1` |
| Version / help / license | `magnet --version`, `--help`, `--usage`, `--license` |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Download or build, the first script, arguments, exit codes, `.cmd` and `#!` launchers |
| [Command line](command-line.md) | Options, where option parsing stops, arguments passed to the script, output, errors, exit codes |
| [Scripting](scripting.md) | Writing magnet scripts: the bundled extensions, how `requireExtension` works in magnet, includes, threads, recipes |
| [Reference](reference.md) | Source layout, the `Application` class, macros, fabricare project, tests, changes between versions |

The language itself (syntax, differences from JavaScript, `Script.*`,
`String`, `Array`, ...) is documented in the `quantum-script` repository,
`docs/`; the API of every extension in its own repository
(`quantum-script--<lowercase name>`, `docs/`). [Scripting](scripting.md)
lists where each one is.

## Source map

```
source/XYO/Magnet/
    Application[.hpp, .cpp]             the tool: options, init the engine with Magnet, run, exit code
    Dependency.hpp                      <XYO/QuantumScript.hpp>, namespace XYO::Magnet uses XYO::System
    Copyright / License / Version       tool metadata (XYO::Magnet::Version::version(), ...)
    Version.Template.rh -> Version.rh   version header, generated from version.json
    Application.rc / .rh / .manifest    Windows executable resources (icon, version info, manifest)
    Application.ico                     icon
fabricare.json                          one project, magnet (exe), depends on quantum-script--magnet
fabricare/test.js                       `fabricare test`: runs test/*.js, checks exit codes and output
test/                                   test scripts
version.json                            the tool's version
```

## AI assistant skill

A Claude Code skill describing how to use this tool lives in
[`.claude/skills/magnet/`](../.claude/skills/magnet/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects where you write
magnet scripts.
