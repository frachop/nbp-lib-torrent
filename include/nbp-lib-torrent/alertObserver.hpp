//
//  alertObserver.hpp
//  transceiver
//
//  Created by Frachop on 17/04/2018.
//  Copyright © 2018 Ymagis. All rights reserved.
//

#pragma once

namespace libtorrent
{
	struct CAlertHandler;

	//- /////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct CAlertObserver
	{
		CAlertObserver();
		virtual ~CAlertObserver();

		virtual void handle(const add_torrent_alert &) {}
		virtual void handle(const torrent_need_cert_alert &) {}
		virtual void handle(const torrent_finished_alert &) {}
		virtual void handle(const torrent_removed_alert &) {}
		virtual void handle(const tracker_error_alert &) {}
		virtual void handle(const torrent_error_alert &) {}
		virtual void handle(const save_resume_data_alert &) {}
		virtual void handle(const save_resume_data_failed_alert &) {}
		virtual void handle(const state_update_alert &) {}
		virtual void handle(const metadata_received_alert &) {}
		virtual void handle(const storage_moved_alert &) {}

		// called for each alert before handle() method
		virtual void onAlert(const alert &) {}

	private:
		friend struct CAlertHandler;
		CAlertHandler * _handler;
	};

}
