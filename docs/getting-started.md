# Getting started

## 1. Get the executable

### From a release

Each release has these archives (`release/` after `fabricare release`, or
the GitHub releases page):

| Archive | Contents | Needs |
|---------|----------|-------|
| `xyo.magnet.vX.Y.Z.win64-msvc-2026.static.bin.zip` | `magnet.exe`, static build (about 4 MB) | nothing, copy it anywhere on the `PATH` |
| `xyo.magnet.vX.Y.Z.win64-msvc-2026.bin.zip` | `magnet.exe` | `quantum-script--magnet.dll`, the 31 `quantum-script--*.dll` extensions, `quantum-script.dll` and the XYO libraries below them, next to it or on the `PATH` — the XYO SDK |
| `xyo.magnet.vX.Y.Z.ubuntu-24.04.bin.zip`, `ubuntu-26.04` | `magnet` | the same libraries as `.so` files on the library path (`~/.fabricare/<platform>/bin` or `LD_LIBRARY_PATH`) |
| `xyo.magnet.vX.Y.Z.sha512.json` | SHA-512 of every archive | |

On a machine without the XYO SDK, use the static Windows build.

### Build it

`magnet` is built with [fabricare](https://github.com/g-stefan/fabricare).
`quantum-script--magnet` (and through it `quantum-script`, every bundled
extension and the XYO libraries) must be installed to the SDK first. From
the repository root:

```bash
fabricare make       # build into output/  (output/bin/magnet[.exe])
fabricare test       # run the tests in test/ against output/bin/magnet
fabricare install    # copy output/bin to ~/.fabricare/<platform>/bin
fabricare clean      # remove output/ and temp/
```

After `install`, `magnet` is in `~/.fabricare/<platform>/bin`, which is on
the `PATH` of a fabricare build (and usually of the developer's shell).
`fabricare release` packs `output/` into the `release/` archives listed
above.

## 2. First runs

```
magnet --version
```

```
version 5.0.0 build 11 [2026-10-06 10:54:53]
```

`magnet` with no arguments (or `--help`) prints the usage. Run one line of
code:

```
magnet --run "Script.requireExtension('Console'); Console.writeLn(6 * 7);"
```

```
42
```

Strings can use single quotes, which keeps the command line simple in
`cmd.exe`, PowerShell and `sh`.

A script file:

```javascript
// report.js - list the .txt files of the current folder as JSON
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("JSON");
Script.requireExtension("SHA512");
Script.requireExtension("DateTime");

var files = Shell.getFileList("*.txt");
var report = {time: (new DateTime()).toUnixTime(), files: []};
for (var i = 0; i < files.length; ++i) {
	report.files[report.files.length] = {
		name: files[i],
		size: Shell.getFileSize(files[i]),
		sha512: SHA512.fileHash(files[i]).substring(0, 16)
	};
};
Shell.filePutContents("report.json", JSON.encodeWithIndentation(report));
Console.writeLn(Shell.fileGetContents("report.json"));
```

```
magnet report.js
```

```
{
	"files": [
		{
			"size": 6,
			"name": "in.txt",
			"sha512": "e7c22b994c59d9cf"
		}
	],
	"time": 1791284694
}
```

Every extension a script uses is loaded with `Script.requireExtension`;
all 31 bundled ones are available (see [Scripting](scripting.md)).
`Console` is needed even for printing.

## 3. Arguments and exit codes

Everything after the script name is left to the script. Read it with the
`Application` extension:

```javascript
// copy-upper.js - upper case a text file
Script.requireExtension("Console");
Script.requireExtension("Application");
Script.requireExtension("Shell");

if (Application.hasFlag("help") || Script.isUndefined(Application.getArgument(1))) {
	Console.writeLn("usage: magnet copy-upper.js input [output] [--force]");
	Script.exit(1);
};

var input = Application.getArgument(1);
var output = Application.getArgument(2, input + ".upper");
if (Shell.fileExists(output) && !Application.hasFlag("force")) {
	Console.writeLn("error: " + output + " exists, use --force");
	Script.exit(2);
};
Shell.filePutContents(output, Shell.fileGetContents(input).toUpperCaseASCII());
Console.writeLn("written " + output);
```

```
magnet copy-upper.js in.txt             -> written in.txt.upper, exit code 0
magnet copy-upper.js in.txt             -> error: in.txt.upper exists, use --force, exit code 2
magnet copy-upper.js in.txt --force     -> written in.txt.upper, exit code 0
```

- `Application.getArgument(k)` is the k-th argument that does not start
  with `--`; `0` is the script itself.
- `Application.hasFlag("force")` is true for `--force` and `--force=...`;
  `Application.getFlagValue("out", "default")` reads `--out=value`.
- `Script.exit(n)` stops the script and makes `n` the exit code of
  `magnet`; `Script.setExitCode(n)` sets it and lets the script continue.
- An uncaught exception prints `Error: <message>` and the stack trace and
  exits with `1`.

Callers can rely on it: `magnet build.js && deploy`, `if errorlevel 1`,
`if ! magnet check.js; then ...`. (`magnet` 4.9.0 and earlier always
exited `0` when the script ended without an exception; see
[Reference](reference.md#changes).)

## 4. Scripts as commands

With `--cmd`, magnet skips the first two lines of the file. That leaves
room for a launcher.

### Windows: a `.cmd` file

```bat
@magnet --cmd "%~f0" %*
@exit /b %ERRORLEVEL%
Script.requireExtension("Console");
Script.requireExtension("Application");
Console.writeLn("hello " + Application.getArgument(1));
Script.exit(4);
```

Saved as `hello.cmd` in a folder on the `PATH`:

```
hello world
```

```
hello world
```

The exit code is `4`. Line 1 runs magnet on the file itself with all the
arguments, line 2 returns its exit code to the caller. Line numbers in
error messages count from the top of the file, the two skipped lines
included.

### Linux: an executable script

```javascript
#!/usr/bin/env -S magnet --cmd
// line 2 is skipped too, keep a comment here
Script.requireExtension("Console");
Script.requireExtension("Application");
Console.writeLn("hello " + Application.getArgument(1));
```

```bash
chmod +x hello
./hello world
```

A plain `#!/usr/bin/env magnet` first line does **not** work: `#` is not
valid Quantum Script, the script fails with `Error: Compile error ... line 1`.
`env -S` (GNU coreutils 8.30 and later) splits `magnet --cmd` into two
words.

## 5. Next

- [Command line](command-line.md) — every option, where option parsing
  stops, errors and exit codes.
- [Scripting](scripting.md) — the bundled extensions, `requireExtension`
  in magnet, includes, threads, recipes.
- The language: `quantum-script` repository, `docs/language.md` (no
  hoisting, `typeof(x)` needs parentheses, `&&` / `||` return booleans,
  statements end with `;`).
