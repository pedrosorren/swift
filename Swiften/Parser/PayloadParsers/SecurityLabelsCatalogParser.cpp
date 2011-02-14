/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "Swiften/Parser/PayloadParsers/SecurityLabelsCatalogParser.h"
#include "Swiften/Parser/PayloadParsers/SecurityLabelParserFactory.h"
#include "Swiften/Parser/PayloadParsers/SecurityLabelParser.h"

namespace Swift {

SecurityLabelsCatalogParser::SecurityLabelsCatalogParser() : level_(TopLevel), labelParser_(0) {
	labelParserFactory_ = new SecurityLabelParserFactory();
}

SecurityLabelsCatalogParser::~SecurityLabelsCatalogParser() {
	delete labelParserFactory_;
}

void SecurityLabelsCatalogParser::handleStartElement(const std::string& element, const std::string& ns, const AttributeMap& attributes) {
	++level_;
	if (level_ == PayloadLevel) {
		getPayloadInternal()->setTo(JID(attributes.getAttribute("to")));
		getPayloadInternal()->setName(attributes.getAttribute("name"));
		getPayloadInternal()->setDescription(attributes.getAttribute("desc"));
	}
	else if (level_ == LabelLevel) {
		assert(!labelParser_);
		if (labelParserFactory_->canParse(element, ns, attributes)) {
			labelParser_ = dynamic_cast<SecurityLabelParser*>(labelParserFactory_->createPayloadParser());
			assert(labelParser_);
		}
	}

	if (labelParser_) {
		labelParser_->handleStartElement(element, ns, attributes);
	}
}

void SecurityLabelsCatalogParser::handleEndElement(const std::string& element, const std::string& ns) {
	if (labelParser_) {
		labelParser_->handleEndElement(element, ns);
	}
	if (level_ == LabelLevel && labelParser_) {
		SecurityLabel* label = dynamic_cast<SecurityLabel*>(labelParser_->getPayload().get());
		assert(label);
		getPayloadInternal()->addLabel(SecurityLabel(*label));
		delete labelParser_;
		labelParser_ = 0;
	}
	--level_;
}

void SecurityLabelsCatalogParser::handleCharacterData(const std::string& data) {
	if (labelParser_) {
		labelParser_->handleCharacterData(data);
	}
}

}
