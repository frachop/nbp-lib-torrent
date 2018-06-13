//
//  alertObserver.cpp
//  transceiver
//
//  Created by Frachop on 17/04/2018.
//  Copyright © 2018 Ymagis. All rights reserved.
//
#include "common.hpp"

namespace libtorrent
{
	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	CAlertObserver::CAlertObserver()
	:	_handler(nullptr)
	{
	}

	CAlertObserver::~CAlertObserver()
	{
		if (_handler)
			_handler->unsubscribe(this);
	}
}
