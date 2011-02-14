/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#ifndef SWIFTEN_SecurityLabelSerializer_H
#define SWIFTEN_SecurityLabelSerializer_H

#include "Swiften/Serializer/GenericPayloadSerializer.h"
#include "Swiften/Elements/SecurityLabel.h"

namespace Swift {
	class SecurityLabelSerializer : public GenericPayloadSerializer<SecurityLabel> {
		public:
			SecurityLabelSerializer();

			virtual std::string serializePayload(boost::shared_ptr<SecurityLabel> version)  const;
	};
}

#endif
