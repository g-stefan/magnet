// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: the exit code set by Script.setExitCode was ignored
// Used by fabricare/test.js, the process exit code must be 4

Script.setExitCode(4);
