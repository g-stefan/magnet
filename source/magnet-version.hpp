//
// Magnet
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef MAGNET_VERSION_HPP
#define MAGNET_VERSION_HPP

#define MAGNET_VERSION_ABCD 2,23,0,52
#define MAGNET_VERSION_STR "2.23.0"
#define MAGNET_VERSION_STR_BUILD "52"
#define MAGNET_VERSION_STR_DATETIME "2022-04-04 01:44:59"

#ifndef XYO_RC

namespace Magnet {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif
