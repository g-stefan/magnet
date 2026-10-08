// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Used by fabricare/test.js: an uncaught exception must print
// "Error: test throw" and the stack trace, the process exit code must be 1

Script.setExitCode(5);
throw new Error("test throw");
