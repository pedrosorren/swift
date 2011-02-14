/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "Swiften/Session/BasicSessionStream.h"

#include <boost/bind.hpp>

#include "Swiften/StreamStack/XMPPLayer.h"
#include "Swiften/StreamStack/StreamStack.h"
#include "Swiften/StreamStack/ConnectionLayer.h"
#include "Swiften/StreamStack/WhitespacePingLayer.h"
#include "Swiften/StreamStack/CompressionLayer.h"
#include "Swiften/StreamStack/TLSLayer.h"
#include "Swiften/TLS/TLSContextFactory.h"
#include "Swiften/TLS/TLSContext.h"

namespace Swift {

BasicSessionStream::BasicSessionStream(
		StreamType streamType, 
		boost::shared_ptr<Connection> connection, 
		PayloadParserFactoryCollection* payloadParserFactories, 
		PayloadSerializerCollection* payloadSerializers, 
		TLSContextFactory* tlsContextFactory, 
		TimerFactory* timerFactory) : 
			available(false), 
			connection(connection), 
			payloadParserFactories(payloadParserFactories), 
			payloadSerializers(payloadSerializers), 
			tlsContextFactory(tlsContextFactory), 
			timerFactory(timerFactory), 
			streamType(streamType),
			compressionLayer(NULL),
			tlsLayer(NULL),
			whitespacePingLayer(NULL) {
	xmppLayer = new XMPPLayer(payloadParserFactories, payloadSerializers, streamType);
	xmppLayer->onStreamStart.connect(boost::bind(&BasicSessionStream::handleStreamStartReceived, this, _1));
	xmppLayer->onElement.connect(boost::bind(&BasicSessionStream::handleElementReceived, this, _1));
	xmppLayer->onError.connect(boost::bind(&BasicSessionStream::handleXMPPError, this));
	xmppLayer->onDataRead.connect(boost::bind(&BasicSessionStream::handleDataRead, this, _1));
	xmppLayer->onWriteData.connect(boost::bind(&BasicSessionStream::handleDataWritten, this, _1));

	connection->onDisconnected.connect(boost::bind(&BasicSessionStream::handleConnectionFinished, this, _1));
	connectionLayer = new ConnectionLayer(connection);

	streamStack = new StreamStack(xmppLayer, connectionLayer);

	available = true;
}

BasicSessionStream::~BasicSessionStream() {
	delete compressionLayer;

	if (tlsLayer) {
		tlsLayer->onError.disconnect(boost::bind(&BasicSessionStream::handleTLSError, this));
		tlsLayer->onConnected.disconnect(boost::bind(&BasicSessionStream::handleTLSConnected, this));
		delete tlsLayer;
	}
	delete whitespacePingLayer;
	delete streamStack;

	connection->onDisconnected.disconnect(boost::bind(&BasicSessionStream::handleConnectionFinished, this, _1));
	delete connectionLayer;

	xmppLayer->onStreamStart.disconnect(boost::bind(&BasicSessionStream::handleStreamStartReceived, this, _1));
	xmppLayer->onElement.disconnect(boost::bind(&BasicSessionStream::handleElementReceived, this, _1));
	xmppLayer->onError.disconnect(boost::bind(&BasicSessionStream::handleXMPPError, this));
	xmppLayer->onDataRead.disconnect(boost::bind(&BasicSessionStream::handleDataRead, this, _1));
	xmppLayer->onWriteData.disconnect(boost::bind(&BasicSessionStream::handleDataWritten, this, _1));
	delete xmppLayer;
}

void BasicSessionStream::writeHeader(const ProtocolHeader& header) {
	assert(available);
	xmppLayer->writeHeader(header);
}

void BasicSessionStream::writeElement(boost::shared_ptr<Element> element) {
	assert(available);
	xmppLayer->writeElement(element);
}

void BasicSessionStream::writeFooter() {
	assert(available);
	xmppLayer->writeFooter();
}

void BasicSessionStream::close() {
	connection->disconnect();
}

bool BasicSessionStream::isOpen() {
	return available;
}

bool BasicSessionStream::supportsTLSEncryption() {
	return tlsContextFactory && tlsContextFactory->canCreate();
}

void BasicSessionStream::addTLSEncryption() {
	assert(available);
	tlsLayer = new TLSLayer(tlsContextFactory);
	if (hasTLSCertificate() && !tlsLayer->setClientCertificate(getTLSCertificate())) {
		onClosed(boost::shared_ptr<Error>(new Error(Error::InvalidTLSCertificateError)));
	}
	else {
		streamStack->addLayer(tlsLayer);
		tlsLayer->onError.connect(boost::bind(&BasicSessionStream::handleTLSError, this));
		tlsLayer->onConnected.connect(boost::bind(&BasicSessionStream::handleTLSConnected, this));
		tlsLayer->connect();
	}
}

bool BasicSessionStream::isTLSEncrypted() {
	return tlsLayer;
}

Certificate::ref BasicSessionStream::getPeerCertificate() const {
	return tlsLayer->getPeerCertificate();
}

boost::shared_ptr<CertificateVerificationError> BasicSessionStream::getPeerCertificateVerificationError() const {
	return tlsLayer->getPeerCertificateVerificationError();
}

ByteArray BasicSessionStream::getTLSFinishMessage() const {
	return tlsLayer->getContext()->getFinishMessage();
}

void BasicSessionStream::addZLibCompression() {
	compressionLayer = new CompressionLayer();
	streamStack->addLayer(compressionLayer);
}

void BasicSessionStream::setWhitespacePingEnabled(bool enabled) {
	if (enabled) {
		if (!whitespacePingLayer) {
			whitespacePingLayer = new WhitespacePingLayer(timerFactory);
			streamStack->addLayer(whitespacePingLayer);
		}
		whitespacePingLayer->setActive();
	}
	else if (whitespacePingLayer) {
		whitespacePingLayer->setInactive();
	}
}

void BasicSessionStream::resetXMPPParser() {
	xmppLayer->resetParser();
}

void BasicSessionStream::handleStreamStartReceived(const ProtocolHeader& header) {
	onStreamStartReceived(header);
}

void BasicSessionStream::handleElementReceived(boost::shared_ptr<Element> element) {
	onElementReceived(element);
}

void BasicSessionStream::handleXMPPError() {
	available = false;
	onClosed(boost::shared_ptr<Error>(new Error(Error::ParseError)));
}

void BasicSessionStream::handleTLSConnected() {
	onTLSEncrypted();
}

void BasicSessionStream::handleTLSError() {
	available = false;
	onClosed(boost::shared_ptr<Error>(new Error(Error::TLSError)));
}

void BasicSessionStream::handleConnectionFinished(const boost::optional<Connection::Error>& error) {
	available = false;
	if (error == Connection::ReadError) {
		onClosed(boost::shared_ptr<Error>(new Error(Error::ConnectionReadError)));
	}
	else if (error) {
		onClosed(boost::shared_ptr<Error>(new Error(Error::ConnectionWriteError)));
	}
	else {
		onClosed(boost::shared_ptr<Error>());
	}
}

void BasicSessionStream::handleDataRead(const ByteArray& data) {
	onDataRead(std::string(data.getData(), data.getSize()));
}

void BasicSessionStream::handleDataWritten(const ByteArray& data) {
	onDataWritten(std::string(data.getData(), data.getSize()));
}

};
