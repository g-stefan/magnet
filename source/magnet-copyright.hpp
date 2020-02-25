//
// Magnet
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef MAGNET_COPYRIGHT_HPP
#define MAGNET_COPYRIGHT_HPP

#define MAGNET_COPYRIGHT            "Copyright (c) Grigore Stefan."
#define MAGNET_PUBLISHER            "Grigore Stefan"
#define MAGNET_COMPANY              MAGNET_PUBLISHER
#define MAGNET_CONTACT              "g_stefan@yahoo.com"
#define MAGNET_FULL_COPYRIGHT       MAGNET_COPYRIGHT " <" MAGNET_CONTACT ">"

#ifndef XYO_RC

namespace Magnet {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
