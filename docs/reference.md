# Reference

For people working on `magnet` itself. Using the tool is covered in
[Command line](command-line.md) and [Scripting](scripting.md).

## fabricare project

```json
{
	"name" : "magnet",
	"make" : "exe",
	"SPDX-License-Identifier": "MIT",
	"sourcePath" : "XYO/Magnet",
	"dependency" : [ "quantum-script--magnet" ]
}
```

One project, an executable built from every source in
`source/XYO/Magnet/`. `quantum-script--magnet` brings in `quantum-script`,
the 31 bundled `quantum-script--*` extensions and the XYO libraries below
them (`xyo-system`, `xyo-cryptography`, `xyo-networking`, `xyo-pixel32`,
...). On a `.static` platform all of them are linked into `magnet.exe`.

The version lives in `version.json` under the key `magnet`;
`Version.rh` is generated from `Version.Template.rh` (`fabricare version`
bumps the build number).

## Headers

| Header | Contents |
|--------|----------|
| `<XYO/Magnet/Dependency.hpp>` | includes `<XYO/QuantumScript.hpp>`; `namespace XYO::Magnet { using namespace XYO::System; }` |
| `<XYO/Magnet/Application.hpp>` | `XYO::Magnet::Application` |
| `<XYO/Magnet/Copyright.hpp>`, `License.hpp`, `Version.hpp` | tool metadata |
| `Application.rh`, `Copyright.rh`, `Version.rh` | macros shared by the C++ code and the Windows resource script |

`Application.cpp` also includes `<XYO/QuantumScript.Extension/Magnet.hpp>`
from `quantum-script--magnet`.

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_MAGNET_LIBRARY` | if defined, `Application.cpp` does not define `main` (`XYO_APPLICATION_MAIN` is skipped), so the class can be compiled into another program; no fabricare project sets it |
| `XYO_MAGNET_NO_VERSION` | `Version.rh` gives `0.0.0` / build `0` instead of the generated version |
| `XYO_MAGNET_VERSION_ABCD`, `_STR`, `_STR_BUILD`, `_STR_DATETIME`, `_STR_WITH_BUILD` | version, from `version.json` |
| `XYO_MAGNET_COPYRIGHT`, `_PUBLISHER`, `_COMPANY`, `_CONTACT` | copyright strings |

## class XYO::Magnet::Application

```cpp
class Application : public virtual IApplication {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

	public:
		inline Application(){};

		void showUsage();
		void showLicense();
		void showVersion();

		int main(int cmdN, char *cmdS[]);

		static void initMemory();

		static void initExecutive(Executive *);
};
```

| Member | Does |
|--------|------|
| `main` | reads the options up to the script / the code of `--run` (see [Command line](command-line.md#where-option-parsing-stops)), initializes the engine with `ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)`, runs `ExecutiveX::executeString` / `executeFileSkipLines(file, 2)` / `executeFile`, returns `ExecutiveX::getExitCode()`; on failure prints `ExecutiveX::getError()` and `getStackTrace()` and returns `1` |
| `initExecutive` | the engine set-up, run for the main engine and again for every thread: `Extension::Magnet::registerInternalExtension(executive)`, then `Script.requireExtension=Script.requireInternalExtension;` and `Script.requireInternalExtension("Magnet");` |
| `initMemory` | `String::initMemory()` before `main` |
| `showUsage`, `showVersion`, `showLicense` | the texts of `--help`, `--version`, `--license` |

`XYO_APPLICATION_MAIN(XYO::Magnet::Application)` provides the C `main`
(`xyo-system`).

`ExecutiveX::endProcessing()` is called before printing the execution
time and returning; the exit code is read with `getExitCode()` **before**
it.

## Metadata functions

```cpp
namespace XYO::Magnet::Version {
	const char *version();          // "5.0.0"
	const char *build();            // "11"
	const char *versionWithBuild(); // "5.0.0.11"
	const char *datetime();         // "2026-10-06 10:54:53"
};
namespace XYO::Magnet::Copyright {
	const char *copyright();
	const char *publisher();
	const char *company();
	const char *contact();
};
namespace XYO::Magnet::License {
	std::string license();          // MIT license text, --license
	std::string shortLicense();
};
```

## Tests

`fabricare test` runs `fabricare/test.js` (a local override of the
built-in test action). It needs `output/bin/magnet` (run `fabricare make`
first) and works on Windows and Linux:

| Check | Script / command |
|-------|------------------|
| a script runs (Mandelbrot in ASCII, with `--execution-time`) | `test/test.0001.js` |
| `Script.exit(n)` / `Script.setExitCode(n)` are the exit code, for `script.js`, `--cmd`, `--run`, `--run=` | `test/test.exit-code.js`, `test/test.set-exit-code.js` |
| `--cmd` skips the 2 launcher lines | `test/test.cmd.js` |
| uncaught exception: `Error: ...`, exit code `1` | `test/test.throw.js` |
| arguments after the script / the code are left to it | `test/test.arguments.js` |
| `--execution-time` before / after `--run` | |
| `--license`, `--version`, `--help`, `--usage`, no arguments: print, exit `0`, run nothing | |
| `--run` without code, missing script: exit `1` | |

The test runner starts magnet through the shell with the output
redirected to `temp/test.output.txt`, then checks the exit code and the
output; each check prints `PASS` or `FAIL` and the first failure stops
the run with a non-zero exit code.

## Licensing

REUSE compliant (`python -m reuse lint`): `source/`, `docs/`, `README.md`
are MIT, source files also carry SPDX headers; configuration,
`fabricare.json`, `fabricare/`, `version.json`, `test/` and `.claude/`
are Unlicense. Every new top level file or folder needs a `Files:` entry
in `.reuse/dep5`.

## Changes

### 5.0.0

- **Exit code.** `Script.exit(n)` and `Script.setExitCode(n)` are the exit
  code of `magnet` (`ExecutiveX::getExitCode()`). Before, magnet returned
  `0` whenever the script ended without an uncaught exception.
- **Option parsing stops at the script.** Arguments after the script (or
  after the code of `--run`) are left to the script. Before, they were
  read as magnet options too: a script argument `--cmd` made magnet skip
  the first two lines of the script, `--run x` ran `x` instead of the
  script, `--execution-time` printed the time.
- **`--license`** prints the license and exits. Before, with more
  arguments, it printed the license and then ran the script.
- **New options** `--version`, `--help`, `--usage`, and `--run="code"` (the
  `quantum-script` form; before, it was ignored and the usage printed).
- `Error: No code specified!` ends with a new line.

Scripts that put magnet options **after** the script name
(`magnet build.js --execution-time`) must move them before it.
