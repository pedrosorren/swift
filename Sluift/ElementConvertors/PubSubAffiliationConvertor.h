/*
 * Copyright (c) 2013 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>

#include <Sluift/GenericLuaElementConvertor.h>
#include <Swiften/Elements/PubSubAffiliation.h>

namespace Swift {
	class LuaElementConvertors;

	class PubSubAffiliationConvertor : public GenericLuaElementConvertor<PubSubAffiliation> {
		public:
			PubSubAffiliationConvertor(LuaElementConvertors* convertors);
			virtual ~PubSubAffiliationConvertor();

			virtual boost::shared_ptr<PubSubAffiliation> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
			virtual void doConvertToLua(lua_State*, boost::shared_ptr<PubSubAffiliation>) SWIFTEN_OVERRIDE;
			virtual boost::optional<Documentation> getDocumentation() const SWIFTEN_OVERRIDE;

		private:
			LuaElementConvertors* convertors;
	};
}
