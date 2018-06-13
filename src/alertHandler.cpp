//
//  alertHandler.cpp
//  transceiver
//
//  Created by Frachop on 17/04/2018.
//  Copyright © 2018 Ymagis. All rights reserved.
//

#include "common.hpp"

namespace libtorrent
{
	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	CAlertHandler::CAlertHandler( session & s )
	:	_session(s)
	,	_observers()
	,	_continue(true)
	{
		_thread = std::thread(&CAlertHandler::run, this);
	}

	CAlertHandler::~CAlertHandler()
	{
		_continue= false;
		_thread.join();
	}

	void CAlertHandler::subscribe(CAlertObserver* o)
	{
		std::unique_lock<std::mutex> l(_m);
		assert(o && (o->_handler==nullptr));
		o->_handler = this;
		_observers.insert(o);
	}

	void CAlertHandler::dispatchAlerts() const
	{
		std::vector<alert*> alert_queue;
		_session.pop_alerts(&alert_queue);
		if (alert_queue.size())
			dispatchAlerts(alert_queue);
	}

	void CAlertHandler::unsubscribe(CAlertObserver* o)
	{
		//LOGT("{} START", BOOST_CURRENT_FUNCTION);
		std::unique_lock<std::mutex> l(_m);
		if (o == nullptr)
			return;

		assert(o->_handler == this);
		o->_handler = nullptr;
		_observers.erase(o);
		//LOGT("{} DONE", BOOST_CURRENT_FUNCTION);
	}

	void CAlertHandler::run()
	{
		//LOGT("{} Start", BOOST_CURRENT_FUNCTION);
		while (_continue)
		{
			dispatchAlerts();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		//LOGT("{} Done", BOOST_CURRENT_FUNCTION);
	}

	void CAlertHandler::dispatchAlerts(const std::vector<alert *>& alerts) const
	{
#		define HANDLE_ALERT(TYPE) case lt::TYPE::alert_type: \
			for (auto o : _observers) o->handle(*alert_cast<lt::TYPE>(a)); \
			break

		//LOGT("{} - {}", BOOST_CURRENT_FUNCTION, alerts.size());
		std::unique_lock<std::mutex> l(_m);
		for (lt::alert const* a : alerts)
		{
			// if ((a->category() & lt::alert::dht_log_notification) == 0) //85
			//if (a->type() != state_update_alert::alert_type)
			//	if (0) LOGT( "[{}]> {}", a->type(), a->message() );

			for (auto o : _observers)
				o->onAlert(*a);
		
			switch (a->type()) {
				HANDLE_ALERT(add_torrent_alert);
				HANDLE_ALERT(torrent_need_cert_alert);
				HANDLE_ALERT(torrent_finished_alert);
				HANDLE_ALERT(torrent_removed_alert);
				HANDLE_ALERT(tracker_error_alert);
				HANDLE_ALERT(torrent_error_alert);
				HANDLE_ALERT(save_resume_data_alert);
				HANDLE_ALERT(save_resume_data_failed_alert);
				HANDLE_ALERT(state_update_alert);
				HANDLE_ALERT(metadata_received_alert);
				HANDLE_ALERT(storage_moved_alert);
			};
		}
#		undef HANDLE_ALERT
	}

}
