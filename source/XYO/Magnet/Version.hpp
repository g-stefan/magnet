// Magnet
// Copyright (c) 2020-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_MAGNET_VERSION_HPP
#define XYO_MAGNET_VERSION_HPP

#ifndef XYO_MAGNET_DEPENDENCY_HPP
#	include <XYO/Magnet/Dependency.hpp>
#endif

namespace XYO::Magnet::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
