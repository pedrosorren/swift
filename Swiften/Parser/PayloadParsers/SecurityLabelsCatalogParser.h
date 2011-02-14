/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#ifndef SWIFTEN_SecurityLabelsCatalogParser_H
#define SWIFTEN_SecurityLabelsCatalogParser_H

#include "Swiften/Elements/SecurityLabelsCatalog.h"
#include "Swiften/Parser/GenericPayloadParser.h"

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
				LabelLevel = 2
			};
			int level_;
			SecurityLabelParserFactory* labelParserFactory_;
			SecurityLabelParser* labelParser_;
	};
}

#endif
