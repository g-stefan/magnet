// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

messageAction("test");

// ---

for(var k=1;k<=1;++k){
	exitIf(Shell.execute("output/bin/magnet  --execution-time test/test.000"+k+".js"));
};

// ---

var magnet = "output/bin/magnet";
var outputFile = "temp/test.output.txt";
if (OS.isWindows()) {
	magnet = "output\\bin\\magnet";
	outputFile = "temp\\test.output.txt";
};
Shell.mkdirRecursivelyIfNotExists("temp");

// run magnet through the shell, output to outputFile, return the exit code
var runMagnet = function(cmd) {
	Shell.removeFile(outputFile);
	return Shell.system(magnet + " " + cmd + " > " + outputFile);
};

var outputHas = function(text) {
	var output = Shell.fileGetContents(outputFile);
	if (Script.isNil(output)) {
		return false;
	};
	return output.indexOf(text) >= 0;
};

// exit code set by Script.exit / Script.setExitCode must be the process exit code
exitIfTest(runMagnet("test/test.exit-code.js") != 3, "exit-code");
exitIfTest(runMagnet("test/test.set-exit-code.js") != 4, "set-exit-code");
exitIfTest(runMagnet("--cmd test/test.exit-code.js") != 3, "cmd exit-code");
exitIfTest(runMagnet("--run \"Script.exit(5);\"") != 5, "run exit-code");
exitIfTest(runMagnet("\"--run=Script.exit(6);\"") != 6, "run= exit-code");

// --cmd skips the first 2 lines (.cmd launcher)
exitIfTest((runMagnet("--cmd test/test.cmd.js") != 8) || !outputHas("-> test cmd ok"), "cmd launcher");

// uncaught exception: message, exit code 1
exitIfTest((runMagnet("test/test.throw.js") != 1) || !outputHas("Error: test throw"), "uncaught exception");

// options after the script / the code belong to the script
exitIfTest((runMagnet("test/test.arguments.js --cmd --run \"Script.exit(9);\" --license --execution-time") != 0) || !outputHas("-> test arguments ok") || outputHas("Execution time"), "script arguments");
exitIfTest((runMagnet("--run \"Script.exit(7);\" --execution-time") != 7) || outputHas("Execution time"), "run arguments");
exitIfTest((runMagnet("--execution-time --run \"Script.exit(0);\"") != 0) || !outputHas("Execution time: "), "execution-time");

// informational options print and exit, nothing is executed
exitIfTest((runMagnet("--license test/test.exit-code.js") != 0) || !outputHas("MIT License"), "license");
exitIfTest((runMagnet("--version") != 0) || !outputHas("version "), "version");
exitIfTest((runMagnet("--help") != 0) || !outputHas("usage:"), "help");
exitIfTest((runMagnet("--usage") != 0) || !outputHas("usage:"), "usage");
exitIfTest((runMagnet("") != 0) || !outputHas("usage:"), "no arguments");

// errors
exitIfTest((runMagnet("--run") != 1) || !outputHas("Error: No code specified!"), "run without code");
exitIfTest((runMagnet("test/no-such-file.js") != 1) || !outputHas("Error: "), "missing script");

Shell.removeFile(outputFile);
