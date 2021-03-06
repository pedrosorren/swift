/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <Swiften/Base/API.h>
#include <Swiften/Parser/XMLParserClient.h>
#include <Swiften/Parser/AttributeMap.h>

namespace Swift {
	class XMLParser;
	class XMPPParserClient;
	class XMLParserFactory;	
	class ElementParser;
	class PayloadParserFactoryCollection;

	class SWIFTEN_API XMPPParser : public XMLParserClient, boost::noncopyable {
		public:
			XMPPParser(
					XMPPParserClient* parserClient, 
					PayloadParserFactoryCollection* payloadParserFactories,
					XMLParserFactory* xmlParserFactory);
			~XMPPParser();

			bool parse(const std::string&);

		private:
			virtual void handleStartElement(
					const std::string& element, 
					const std::string& ns, 
					const AttributeMap& attributes);
			virtual void handleEndElement(const std::string& element, const std::string& ns);
			virtual void handleCharacterData(const std::string& data);

			ElementParser* createElementParser(const std::string& element, const std::string& xmlns);

		private:
			XMLParser* xmlParser_;
			XMPPParserClient* client_;
			PayloadParserFactoryCollection* payloadParserFactories_;
			enum Level {
				TopLevel = 0,
				StreamLevel = 1,
				ElementLevel = 2
			};
			int level_;
			ElementParser* currentElementParser_;
			bool parseErrorOccurred_;
	};
}
