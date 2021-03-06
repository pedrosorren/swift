/*
 * Copyright (c) 2010-2014 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <Swiften/Base/API.h>
#include <Swiften/JID/JID.h>
#include <Swiften/Base/boost_bsignals.h>
#include <Swiften/Base/Error.h>
#include <string>
#include <Swiften/Elements/ToplevelElement.h>
#include <Swiften/Elements/Stanza.h>
#include <Swiften/Session/SessionStream.h>

namespace Swift {
	class ComponentAuthenticator;
	class CryptoProvider;

	class SWIFTEN_API ComponentSession : public boost::enable_shared_from_this<ComponentSession> {
		public:
			enum State {
				Initial,
				WaitingForStreamStart,
				Authenticating,
				Initialized,
				Finishing,
				Finished
			};

			struct Error : public Swift::Error {
				enum Type {
					AuthenticationFailedError,
					UnexpectedElementError
				} type;
				Error(Type type) : type(type) {}
			};

			~ComponentSession();

			static boost::shared_ptr<ComponentSession> create(const JID& jid, const std::string& secret, boost::shared_ptr<SessionStream> stream, CryptoProvider* crypto) {
				return boost::shared_ptr<ComponentSession>(new ComponentSession(jid, secret, stream, crypto));
			}

			State getState() const {
				return state;
			}

			void start();
			void finish();

			void sendStanza(boost::shared_ptr<Stanza>);

		public:
			boost::signal<void ()> onInitialized;
			boost::signal<void (boost::shared_ptr<Swift::Error>)> onFinished;
			boost::signal<void (boost::shared_ptr<Stanza>)> onStanzaReceived;
		
		private:
			ComponentSession(const JID& jid, const std::string& secret, boost::shared_ptr<SessionStream>, CryptoProvider*);

			void finishSession(Error::Type error);
			void finishSession(boost::shared_ptr<Swift::Error> error);

			void sendStreamHeader();

			void handleElement(boost::shared_ptr<ToplevelElement>);
			void handleStreamStart(const ProtocolHeader&);
			void handleStreamClosed(boost::shared_ptr<Swift::Error>);

			bool checkState(State);

		private:
			JID jid;
			std::string secret;
			boost::shared_ptr<SessionStream> stream;
			CryptoProvider* crypto;
			boost::shared_ptr<Swift::Error> error;
			State state;
	};
}
