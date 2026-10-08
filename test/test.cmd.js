@magnet --cmd "%~f0" %*
@exit /b %ERRORLEVEL%
// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Used by fabricare/test.js: magnet --cmd test/test.cmd.js
// --cmd must skip the 2 launcher lines above (not valid script code)
// and the exit code must be the process exit code, 8

Script.requireExtension("Console");
Console.writeLn("-> test cmd ok");
Script.exit(8);
