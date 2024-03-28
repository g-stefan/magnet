// Magnet
// Copyright (c) 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
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
		printf("version %s build %s [%s]\n", Magnet::Version::version(), Magnet::Version::build(), Magnet::Version::datetime());
		printf("%s\n\n", Magnet::Copyright::copyright());

		printf("%s",
		       "options:\n"
		       "    --license              show license\n"
		       "    --cmd script           execute script, skip first 2 lines, to be used on shell scripts\n"
		       "    script.js              execute script\n"
		       "    --run \"code\"           run code\n"
		       "    --execution-time       show execution time\n"
		       "    --execution-time-cmd   --execution-time + --cmd\n");
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
		String code;

		for (i = 1; i < cmdN; ++i) {
			if (strncmp(cmdS[i], "--", 2) == 0) {
				opt = &cmdS[i][2];
				if (strcmp(opt, "license") == 0) {
					showLicense();
					if (cmdN == 2) {
						return 0;
					};
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
						continue;
					};
					break;
				};
				continue;
			};
			if (!fileIn) {
				fileIn = cmdS[i];
			};
		};

		if (!runCode) {
			if (fileIn == nullptr) {
				showUsage();
				return 0;
			};
		} else {
			if (code.length() == 0) {
				printf("Error: No code specified!");
				return 1;
			};
		};

		if (executionTime) {
			beginTimestampInMilliseconds = DateTime::timestampInMilliseconds();
		};

		if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
			if (runCode) {
				if (ExecutiveX::executeString(code)) {
					ExecutiveX::endProcessing();
					if (executionTime) {
						endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
						intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
						printf("Execution time: " XYO_PLATFORM_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
					};
					return 0;
				};
			} else {
				if (isCmd) {
					if (ExecutiveX::executeFileSkipLines(fileIn, 2)) {
						ExecutiveX::endProcessing();
						if (executionTime) {
							endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
							intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
							printf("Execution time: " XYO_PLATFORM_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
						};
						return 0;
					};

					fflush(stdout);
					printf("%s\n", (ExecutiveX::getError()).value());
					printf("%s", (ExecutiveX::getStackTrace()).value());
					fflush(stdout);

					ExecutiveX::endProcessing();
					return 1;
				};

				if (ExecutiveX::executeFile(fileIn)) {
					ExecutiveX::endProcessing();
					if (executionTime) {
						endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
						intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
						printf("Execution time: " XYO_PLATFORM_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
					};
					return 0;
				};
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
