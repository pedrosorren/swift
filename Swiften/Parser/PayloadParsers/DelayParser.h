/*
 * Copyright (c) 2010 Kevin Smith
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include "Swiften/Elements/Delay.h"
#include "Swiften/Parser/GenericPayloadParser.h"

namespace Swift {
	class DelayParser : public GenericPayloadParser<Delay> {
		public:
			DelayParser(const std::locale& locale);

			virtual void handleStartElement(const std::string& element, const std::string&, const AttributeMap& attributes);
			virtual void handleEndElement(const std::string& element, const std::string&);
			virtual void handleCharacterData(const std::string& data);

		private:
			boost::posix_time::ptime dateFromString(const std::string& string);

		private:
			std::locale locale;
			int level_;
	};
}
