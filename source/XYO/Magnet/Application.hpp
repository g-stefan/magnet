// Magnet
// Copyright (c) 2020-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_MAGNET_APPLICATION_HPP
#define XYO_MAGNET_APPLICATION_HPP

#ifndef XYO_MAGNET_DEPENDENCY_HPP
#	include <XYO/Magnet/Dependency.hpp>
#endif

namespace XYO::Magnet {
	using namespace XYO::QuantumScript;

	class Application : public virtual IApplication {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Application);

		public:
			inline Application(){};

			void showUsage();
			void showLicense();
			void showVersion();

			int main(int cmdN, char *cmdS[]);

			static void initMemory();

			static void initExecutive(Executive *);
	};

};

#endif
