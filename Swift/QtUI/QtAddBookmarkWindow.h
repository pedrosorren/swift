/*
 * Copyright (c) 2010-2014 Kevin Smith
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include "QtBookmarkDetailWindow.h"

#include "Swift/Controllers/UIEvents/UIEventStream.h"
#include "Swift/Controllers/UIEvents/AddMUCBookmarkUIEvent.h"

namespace Swift {
	class QtAddBookmarkWindow : public QtBookmarkDetailWindow {
		Q_OBJECT
		public:
			QtAddBookmarkWindow(UIEventStream* eventStream);
			QtAddBookmarkWindow(UIEventStream* eventStream, const MUCBookmark& bookmark);
			bool commit();
		private:
			UIEventStream* eventStream_;
	};
}
