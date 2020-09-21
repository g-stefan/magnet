//
// Magnet
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef MAGNET_VERSION_HPP
#define MAGNET_VERSION_HPP

#define MAGNET_VERSION_ABCD                1,1,0,3
#define MAGNET_VERSION_STR                 "1.1.0"
#define MAGNET_VERSION_STR_BUILD           "3"
#define MAGNET_VERSION_STR_DATETIME        "2020-09-21 17:40:12"

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

