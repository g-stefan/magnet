# Magnet

Quantum Script with batteries included
- `magnet script.js [arguments ...]` runs a Quantum Script (JavaScript-like)
script with the 31 standard extensions built in: files, directories,
processes, JSON, XML, CSV, HTTP / HTTPS, sockets, SSH, threads, jobs,
hashes, encryption, images.
- Same scripts on Windows and Linux; `.cmd` launchers and `#!` scripts with
`--cmd`; arguments and the exit code (`Script.exit(n)`) passed through.
- Extensions are internal: no extension library to find or install; the
static Windows build is a single `magnet.exe`.

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("SHA512");

var files = Shell.getFileList("*.zip");
for (var i = 0; i < files.length; ++i) {
	Console.writeLn(SHA512.fileHash(files[i]) + "  " + files[i]);
};
```

```
magnet script.js
magnet --run "Script.requireExtension('Console'); Console.writeLn(6 * 7);"
magnet --execution-time script.js
```

Built on `quantum-script--magnet`.

## Quantum Script extensions included:

Application\
ApplicationVersion\
Base16\
Base32\
Base64\
Buffer\
Console\
Crypt\
CSV\
DateTime\
File\
HTTP\
Job\
JSON\
Make\
Math\
MD5\
OpenSSL\
Pixel32\
ProcessInteractive\
Random\
SHA256\
SHA512\
Shell\
ShellFind\
Socket\
SSHRemote\
Task\
Thread\
URL\
XML

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - download or build, first scripts, arguments, exit codes, `.cmd` and `#!` launchers
- [Command line](docs/command-line.md) - options, arguments passed to the script, output, errors, exit codes
- [Scripting](docs/scripting.md) - the bundled extensions, `requireExtension` in magnet, includes, threads, recipes
- [Reference](docs/reference.md) - source layout, `Application` class, macros, tests, changes

A Claude Code skill for this tool is in
[.claude/skills/magnet](.claude/skills/magnet/SKILL.md).

## License

Copyright (c) 2020-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
