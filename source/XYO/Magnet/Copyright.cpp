// Magnet
// Copyright (c) 2020-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/Magnet/Copyright.hpp>
#include <XYO/Magnet/Copyright.rh>

namespace XYO::Magnet::Copyright {

	static const char *copyright_ = XYO_MAGNET_COPYRIGHT;
	static const char *publisher_ = XYO_MAGNET_PUBLISHER;
	static const char *company_ = XYO_MAGNET_COMPANY;
	static const char *contact_ = XYO_MAGNET_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
