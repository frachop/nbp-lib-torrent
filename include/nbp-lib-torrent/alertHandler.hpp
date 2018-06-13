//
//  alertHandler.hpp
//  transceiver
//
//  Created by Frachop on 17/04/2018.
//  Copyright © 2018 Ymagis. All rights reserved.
//

#pragma once

namespace libtorrent
{
	struct CAlertObserver;

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct CAlertHandler
	{
		CAlertHandler( session & s );
		~CAlertHandler();

		void subscribe(CAlertObserver* o);
		void dispatchAlerts() const;
		void unsubscribe(CAlertObserver* o);

	private:
		void dispatchAlerts(const std::vector<alert *>& alerts) const;
		void run();

	private:
		friend struct CAlertObserver;
		session & _session;

		std::thread _thread;
		mutable std::mutex _m;
		std::atomic_bool _continue;
		std::set<CAlertObserver *> _observers;
	};
}
