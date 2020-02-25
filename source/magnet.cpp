//
// Magnet
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyo-pixel32.hpp"
#include "quantum-script.hpp"
#include "quantum-script-extension-application.hpp"
#include "quantum-script-extension-buffer.hpp"
#include "quantum-script-extension-base16.hpp"
#include "quantum-script-extension-base32.hpp"
#include "quantum-script-extension-base64.hpp"
#include "quantum-script-extension-console.hpp"
#include "quantum-script-extension-crypt.hpp"
#include "quantum-script-extension-csv.hpp"
#include "quantum-script-extension-datetime.hpp"
#include "quantum-script-extension-file.hpp"
#include "quantum-script-extension-http.hpp"
#include "quantum-script-extension-job.hpp"
#include "quantum-script-extension-json.hpp"
#include "quantum-script-extension-make.hpp"
#include "quantum-script-extension-math.hpp"
#include "quantum-script-extension-md5.hpp"
#include "quantum-script-extension-processinteractive.hpp"
#include "quantum-script-extension-random.hpp"
#include "quantum-script-extension-sha256.hpp"
#include "quantum-script-extension-sha512.hpp"
#include "quantum-script-extension-shell.hpp"
#include "quantum-script-extension-shellfind.hpp"
#include "quantum-script-extension-socket.hpp"
#include "quantum-script-extension-task.hpp"
#include "quantum-script-extension-thread.hpp"
#include "quantum-script-extension-url.hpp"
#include "quantum-script-extension-version.hpp"
#include "quantum-script-extension-xml.hpp"
#include "quantum-script-extension-openssl.hpp"
#include "quantum-script-extension-pixel32.hpp"
#include "quantum-script-extension-sshremote.hpp"

#include "magnet-license.hpp"
#include "magnet-copyright.hpp"
#include "magnet-version.hpp"

namespace Main {

	using namespace XYO;
	using namespace Quantum::Script;

	class Application :
		public virtual IMain {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Application);
		protected:

			static void initExecutive(Executive *);

			void showUsage();
			void showLicense();

		public:

			inline Application() {};

			int main(int cmdN, char *cmdS[]);
	};

	void Application::initExecutive(Executive *executive) {
		Extension::Application::registerInternalExtension(executive);
		Extension::Base16::registerInternalExtension(executive);
		Extension::Base32::registerInternalExtension(executive);
		Extension::Base64::registerInternalExtension(executive);
		Extension::Buffer::registerInternalExtension(executive);
		Extension::Console::registerInternalExtension(executive);
		Extension::Crypt::registerInternalExtension(executive);
		Extension::CSV::registerInternalExtension(executive);
		Extension::DateTime::registerInternalExtension(executive);
		Extension::File::registerInternalExtension(executive);
		Extension::HTTP::registerInternalExtension(executive);
		Extension::Job::registerInternalExtension(executive);
		Extension::JSON::registerInternalExtension(executive);
		Extension::Make::registerInternalExtension(executive);
		Extension::Math::registerInternalExtension(executive);
		Extension::MD5::registerInternalExtension(executive);
		Extension::OpenSSL::registerInternalExtension(executive);
		Extension::Pixel32::registerInternalExtension(executive);
		Extension::ProcessInteractive::registerInternalExtension(executive);
		Extension::Random::registerInternalExtension(executive);
		Extension::SHA256::registerInternalExtension(executive);
		Extension::SHA512::registerInternalExtension(executive);
		Extension::Shell::registerInternalExtension(executive);
		Extension::ShellFind::registerInternalExtension(executive);
		Extension::Socket::registerInternalExtension(executive);
		Extension::SSHRemote::registerInternalExtension(executive);
		Extension::Task::registerInternalExtension(executive);
		Extension::Thread::registerInternalExtension(executive);
		Extension::URL::registerInternalExtension(executive);
		Extension::Version::registerInternalExtension(executive);
		Extension::XML::registerInternalExtension(executive);
		//
		executive->compileString("Script.requireExtension=Script.requireInternalExtension;");
	};

	void Application::showUsage() {
		printf("Magnet\n");
		printf("version %s build %s [%s]\n", Magnet::Version::version(), Magnet::Version::build(), Magnet::Version::datetime());
		printf("%s\n\n", Magnet::Copyright::fullCopyright());

		printf("%s",
			"options:\n"
			"    --license              show license\n"
			"    --cmd script           execute script, skip first 2 lines, to be used on shell scripts\n"
			"    script.js              execute script\n"
			"    --execution-time       show execution time\n"
			"    --execution-time-cmd   --execution-time + --cmd\n"
		);
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", Magnet::License::content());
		printf("%s", Extension::OpenSSL::License::content());
		printf("%s", XYO::Pixel32::License::content());
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
				continue;
			};
			if (!fileIn) {
				fileIn = cmdS[i];
			};
		};

		if(fileIn == nullptr) {
			showUsage();
			return 0;
		};

		if(executionTime) {
			beginTimestampInMilliseconds = DateTime::timestampInMilliseconds();
		};

		if(ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
			if(isCmd) {
				if(ExecutiveX::executeFileSkipLines(fileIn, 2)) {
					ExecutiveX::endProcessing();
					if(executionTime) {
						endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
						intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
						printf("Execution time: " XYO_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
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

			if(ExecutiveX::executeFile(fileIn)) {
				ExecutiveX::endProcessing();
				if(executionTime) {
					endTimestampInMilliseconds = DateTime::timestampInMilliseconds();
					intervalTimestampInMilliseconds = endTimestampInMilliseconds - beginTimestampInMilliseconds;
					printf("Execution time: " XYO_FORMAT_SIZET " ms\n", (size_t)intervalTimestampInMilliseconds);
				};
				return 0;
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

XYO_APPLICATION_MAIN_STD(Main::Application);

