# Scripting

How to write scripts for `magnet`. The language is Quantum Script; this
page covers what is specific to magnet and the most common tasks.

## The language in one paragraph

Quantum Script looks like JavaScript (ES3 era) with a few differences
that matter in practice: **no hoisting** (define a function before the
line that calls it), `typeof(x)` needs parentheses, `&&` / `||` return
`true` / `false` (not one of the operands), `null == 0` is true,
`array.sort()` returns a new array, there is no `let`, `const`, arrow
function, `class`, regular expression literal or `**`. Statements, and
blocks, end with `;` (`};`). The full description is in the
`quantum-script` repository: `docs/language.md` and
`docs/standard-library.md` (`Script`, `String`, `Array`, `Convert`,
`Error`, `Console`).

## Extensions in magnet

Every extension a script uses must be loaded first, by name:

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("JSON");
```

In magnet, `Script.requireExtension` **is** `Script.requireInternalExtension`:
it loads only the extensions built into magnet and never searches for a
`quantum-script--<name>.dll` / `.so`. A script cannot pick up a different
version of an extension from its folder or the `PATH`, and an extension
that is not bundled cannot be loaded at all
(`Error: Unable to open "SCard"`).

Names are not case sensitive; loading an extension twice does nothing.
Some extensions load others they need (`HTTP` loads `File`, `Socket`,
`URL`, `JSON`, ...). `Script.getExtensionList()` lists what is loaded,
with versions.

### The bundled extensions

| Extension | Objects | What it is for |
|-----------|---------|----------------|
| `Application` | `Application` | command line: `getCmdN`, `getCmdS`, `getArgument`, `hasFlag`, `getFlagValue`, path of the executable |
| `ApplicationVersion` | `ApplicationVersion`, `VersionCompare` | compare version strings `major.minor.patch.build` |
| `Base16`, `Base32`, `Base64` | same name | encode / decode strings and buffers |
| `Buffer` | `Buffer` | binary data |
| `Console` | `Console` | `write`, `writeLn`, `readLn` |
| `Crypt` | `Crypt` | symmetric, signed encryption of strings, buffers and files |
| `CSV` | `CSV` | decode / encode one CSV line |
| `DateTime` | `DateTime` | local date and time (`new DateTime()`), Unix time, `DateTime.timestampInMilliseconds()` |
| `File` | `File` | streaming files and `stdin` / `stdout` / `stderr`, text and binary |
| `HTTP` | `HTTP` | plain `http://` client: requests, JSON, downloads |
| `Job` | `Job` | run programs and script functions in parallel, a few at a time |
| `JSON` | `JSON` | `decode`, `encode`, `encodeWithIndentation` |
| `Make` | `Make`, `MakeError` | make-style incremental builds with parallel recipes |
| `Math` | `Math` | the JavaScript `Math` object |
| `MD5`, `SHA256`, `SHA512` | same name | digests of strings, buffers and files (`SHA512.fileHash(file)`) |
| `OpenSSL` | `OpenSSL`, `HTTPS` | `https://` client, TLS, RSA |
| `Pixel32` | `Pixel32` | RGBA images: PNG load / save, resize, crop, blend, filters |
| `ProcessInteractive` | `ProcessInteractive` | run a program with pipes to its input and output |
| `Random` | `Random` | seedable Mersenne Twister (not for security) |
| `Shell` | `Shell` | files, directories, paths, environment, processes |
| `ShellFind` | `ShellFind` | directory iterator with wildcards |
| `Socket` | `Socket` | TCP clients and servers, IPv4 / IPv6 |
| `SSHRemote` | `SSHRemote` | commands and file copy over SSH with PuTTY `plink` / `pscp` |
| `Task` | `Task`, `TaskQueue` | timers and polled tasks with an event loop |
| `Thread` | `Thread`, `CurrentThread`, `Atomic`, `Processor` | run script functions on other threads |
| `URL` | `URL` | percent-encoding, split a URL |
| `XML` | `XML`, `XMLDocument`, `XMLNode`, ... | load, query, change and write XML |

Plus `Magnet` itself (already loaded by magnet; it defines no object).
There is no extension named `Version`: version comparison is
`ApplicationVersion`.

The API of each extension is documented in its own repository,
`quantum-script--<lowercase name>` (`README.md`, `docs/`, and a Claude
Code skill in `.claude/skills/`); `Console` is in the `quantum-script`
repository, `docs/standard-library.md`. On GitHub:
`https://github.com/g-stefan/quantum-script--<lowercase name>`.

## Including other scripts

```javascript
Script.include("lib/util.js");      // runs lib/util.js in the global scope
Script.includeOnce("lib/util.js");  // only the first time
```

A relative name is searched, in order:

1. relative to the **current folder**;
2. in the folder of the magnet executable;
3. relative to the folder of the file that is including it.

So a script can include its neighbors (`Script.include("lib.js")` next to
`main.js`) whatever the current folder is, as long as no file with the
same name is in the current folder. To be explicit, build the path from
the script's own folder:

```javascript
Script.requireExtension("Application");
Script.requireExtension("Shell");
var scriptPath = Shell.getFilePath(Application.getArgument(0));
Script.include(scriptPath + "/lib/util.js");    // "/" works on Windows too
```

## Threads

Each thread runs its own engine. Magnet sets it up the same way (the
bundled extensions are available), but the extensions loaded by the main
script are **not** loaded in the thread: require them again inside the
thread function.

```javascript
Script.requireExtension("Console");
Script.requireExtension("Thread");

var t = new Thread();
t.start(function() {
	Script.requireExtension("Console");     // needed: a new engine
	Script.requireExtension("JSON");
	Console.writeLn(JSON.encode({a: 1}));
});
t.join();
```

An error inside a thread is **not** printed: a thread that calls
`Console.writeLn` without requiring `Console` stops silently. When a
thread does nothing visible, check its `requireExtension` calls first.
For running many jobs, see the `Job` extension.

## Exit codes and errors

```javascript
if (!Shell.fileExists("input.txt")) {
	Console.writeLn("error: input.txt not found");
	Script.exit(2);                // stop now, exit code 2
};
Script.setExitCode(1);             // exit code 1 when the script ends
throw new Error("bad input");      // Error: bad input + stack trace, exit code 1
```

Prefer `Script.exit(n)` with a message for expected failures (the caller
gets a precise code) and let exceptions signal bugs. See
[Command line](command-line.md#exit-codes).

## Recipes

### Run a program, stop on failure

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");

var code = Shell.system("git --version");    // through cmd.exe / sh: pipes, >, && work
if (code != 0) {
	Console.writeLn("git failed: " + code);
	Script.exit(code);
};
```

`Shell.system` runs through the shell on both systems; `Shell.execute`
starts the program directly on Windows (no shell built-ins, no
redirection). Both return the exit code. To read a program's output,
redirect it to a file and read the file, or use `ProcessInteractive`.

### Read and update a JSON file

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("JSON");

var config = {name: "app", version: "1.0.0"};
if (Shell.fileExists("config.json")) {
	config = JSON.decode(Shell.fileGetContents("config.json"));
};
config.version = "1.0.1";
Shell.filePutContents("config.json", JSON.encodeWithIndentation(config));
```

`JSON.encode` does not keep the order of the keys.

### Files and folders

```javascript
Script.requireExtension("Shell");

Shell.mkdirRecursivelyIfNotExists("output/logs");
var list = Shell.getFileList("source/*.cpp");      // names matching a wildcard
for (var i = 0; i < list.length; ++i) {
	Shell.copyFile(list[i], "output/" + Shell.getFileName(list[i]));
};
```

`Shell.fileGetContents` returns `undefined` when the file cannot be read;
test with `Script.isNil(...)`. For large files use `File` (streaming)
instead of reading everything into a string.

### Time something

```javascript
Script.requireExtension("Console");
Script.requireExtension("DateTime");

var begin = DateTime.timestampInMilliseconds();
// ... work ...
Console.writeLn("took " + (DateTime.timestampInMilliseconds() - begin) + " ms");
```

For the whole script, `magnet --execution-time script.js`.

## Scripts that also run in `quantum-script`

The dynamic `quantum-script` interpreter can run magnet scripts after
loading the bundle:

```javascript
Script.requireExtension("Magnet");   // does nothing in magnet, registers the bundle in quantum-script
```

In `quantum-script`, `requireExtension` prefers an external
`quantum-script--<name>.dll` found on the include path over the bundled
one; use `Script.requireInternalExtension(...)` to force the bundled one.
