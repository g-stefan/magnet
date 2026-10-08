// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: options after the script were taken by magnet,
// --cmd skipped the first 2 lines of the script, --run replaced it
// Used by fabricare/test.js:
//     magnet test/test.arguments.js --cmd --run "Script.exit(9);" --license --execution-time
// the arguments after the script must be left to the script

Script.requireExtension("Console");
Script.requireExtension("Application");

var expected = ["test/test.arguments.js", "--cmd", "--run", "Script.exit(9);", "--license", "--execution-time"];
var k;

if (Application.getCmdN() != expected.length + 1) {
	Console.writeLn("-> test arguments: expected " + (expected.length + 1) + " arguments, got " + Application.getCmdN());
	Script.exit(1);
};

for (k = 0; k < expected.length; ++k) {
	if (Application.getCmdS(k + 1) != expected[k]) {
		Console.writeLn("-> test arguments: argument " + (k + 1) + " is " + Application.getCmdS(k + 1) + ", expected " + expected[k]);
		Script.exit(2);
	};
};

Console.writeLn("-> test arguments ok");
