/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Elements/SecurityLabelsCatalog.h>
#include <Swiften/Parser/GenericPayloadParser.h>

namespace Swift {
	class SecurityLabelParserFactory;
	class SecurityLabelParser;

	class SecurityLabelsCatalogParser : public GenericPayloadParser<SecurityLabelsCatalog> {
		public:
			SecurityLabelsCatalogParser();
			~SecurityLabelsCatalogParser();

			virtual void handleStartElement(const std::string& element, const std::string&, const AttributeMap& attributes);
			virtual void handleEndElement(const std::string& element, const std::string&);
			virtual void handleCharacterData(const std::string& data);

		private:
			enum Level { 
				TopLevel = 0, 
				PayloadLevel = 1,
				ItemLevel = 2,
				LabelLevel = 3
			};
			int level_;
			SecurityLabelParserFactory* labelParserFactory_;
			SecurityLabelParser* labelParser_;
			boost::shared_ptr<SecurityLabelsCatalog::Item> currentItem_;
	};
}
