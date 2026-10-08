// Magnet
// Copyright (c) 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Magnet/Application.hpp>
#include <XYO/Magnet/Copyright.hpp>
#include <XYO/Magnet/License.hpp>
#include <XYO/Magnet/Version.hpp>

#include <XYO/QuantumScript.Extension/Magnet.hpp>

namespace XYO::Magnet {
	using namespace XYO::QuantumScript;

	void Application::showUsage() {
		printf("Magnet\n");
		showVersion();
		printf("%s\n\n", Magnet::Copyright::copyright());

		printf("%s",
		       "usage:\n"
		       "    magnet [options] script.js [arguments ...]\n"
		       "    magnet [options] --run \"code\" [arguments ...]\n"
		       "\n"
		       "options:\n"
		       "    --help, --usage        show this help\n"
		       "    --version              show version\n"
		       "    --license              show license\n"
		       "    --cmd script           execute script, skip first 2 lines, to be used on shell scripts\n"
		       "    script.js              execute script\n"
		       "    --run \"code\"           run code\n"
		       "    --run=\"code\"           run code\n"
		       "    --execution-time       show execution time\n"
		       "    --execution-time-cmd   --execution-time + --cmd\n"
		       "\n"
		       "options are read up to the script or the code to run,\n"
		       "the arguments after it are left to the script\n");
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", Magnet::License::license().c_str());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", Magnet::Version::version(), Magnet::Version::build(), Magnet::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
	};

	void Application::initExecutive(Executive *executive) {
		Extension::Magnet::registerInternalExtension(executive);
		executive->compileString("Script.requireExtension=Script.requireInternalExtension;");
		executive->compileString("Script.requireInternalExtension(\"Magnet\");");
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		char *opt;
		char *fileIn;
		bool executionTime = false;
		uint64_t beginTimestampInMilliseconds;
		uint64_t endTimestampInMilliseconds;
		uint64_t intervalTimestampInMilliseconds;
		fileIn = nullptr;
		bool isCmd = false;
		bool runCode = false;
		bool isOk;
		String code;

		// Options are read up to the script (or the code to run),
		// the arguments after it belong to the script
		for (i = 1; i < cmdN; ++i) {
			if (strncmp(cmdS[i], "--", 2) != 0) {
				fileIn = cmdS[i];
				break;
			};
			opt = &cmdS[i][2];
			if ((strcmp(opt, "help") == 0) || (strcmp(opt, "usage") == 0)) {
				showUsage();
				return 0;
			};
			if (strcmp(opt, "license") == 0) {
				showLicense();
				return 0;
			};
			if (strcmp(opt, "version") == 0) {
				showVersion();
				return 0;
			};
			if (strcmp(opt, "execution-time") == 0) {
				executionTime = true;
				continue;
			};
			if (strcmp(opt, "cmd") == 0) {
				isCmd = true;
				continue;
			};
			if (strcmp(opt, "execution-time-cmd") == 0) {
				executionTime = true;
				isCmd = true;
				continue;
			};
			if (strcmp(opt, "run") == 0) {
				runCode = true;
				++i;
				if (i < cmdN) {
					code = cmdS[i];
				};
				break;
			};
			if (strncmp(opt, "run=", 4) == 0) {
				runCode = true;
				code = &opt[4];
				break;
			};
		};

		if (!runCode) {
			if (fileIn == nullptr) {
				showUsage();
				return 0;
			};
		} else {
			if (code.length() == 0) {
				printf("Error: No code specified!\n");
				return 1;
			};
		};

		if (executionTime) {
			beginTimestampInMilliseconds = DateTime::timestampInMilliseconds();
		};

		if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
			if (runCode) {
				isOk = ExecutiveX::executeString(code);
			} else if (isCmd) {
				isOk = ExecutiveX::executeFileSkipLines(fileIn, 2);
			} else {
				isOk = ExecutiveX::executeFile(fileIn);
			};
			if (isOk) {
				int exitCode = ExecutiveX::getExitCode();
				ExecutiveX::endProcessing();
				if (executionTime) {
					endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
					intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
					printf("Execution time: " XYO_PLATFORM_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
				};
				return exitCode;
			};
		};

		fflush(stdout);
		printf("%s\n", (ExecutiveX::getError()).value());
		printf("%s", (ExecutiveX::getStackTrace()).value());
		fflush(stdout);

		ExecutiveX::endProcessing();
		return 1;
	};
};

#ifndef XYO_MAGNET_LIBRARY
XYO_APPLICATION_MAIN(XYO::Magnet::Application);
#endif
