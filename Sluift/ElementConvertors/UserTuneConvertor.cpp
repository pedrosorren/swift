/*
 * Copyright (c) 2014 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#include <Sluift/ElementConvertors/UserTuneConvertor.h>

#include <lua.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>



#pragma clang diagnostic ignored "-Wunused-private-field"

using namespace Swift;

UserTuneConvertor::UserTuneConvertor(LuaElementConvertors* convertors) : 
		GenericLuaElementConvertor<UserTune>("user_tune"),
		convertors(convertors) {
}

UserTuneConvertor::~UserTuneConvertor() {
}

boost::shared_ptr<UserTune> UserTuneConvertor::doConvertFromLua(lua_State* L) {
	boost::shared_ptr<UserTune> result = boost::make_shared<UserTune>();
	lua_getfield(L, -1, "rating");
	if (lua_isnumber(L, -1)) {
		result->setRating(boost::numeric_cast<unsigned int>(lua_tonumber(L, -1)));
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "title");
	if (lua_isstring(L, -1)) {
		result->setTitle(std::string(lua_tostring(L, -1)));
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "track");
	if (lua_isstring(L, -1)) {
		result->setTrack(std::string(lua_tostring(L, -1)));
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "artist");
	if (lua_isstring(L, -1)) {
		result->setArtist(std::string(lua_tostring(L, -1)));
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "uri");
	if (lua_isstring(L, -1)) {
		result->setURI(std::string(lua_tostring(L, -1)));
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "source");
	if (lua_isstring(L, -1)) {
		result->setSource(std::string(lua_tostring(L, -1)));
	}
	lua_pop(L, 1);
	lua_getfield(L, -1, "length");
	if (lua_isnumber(L, -1)) {
		result->setLength(boost::numeric_cast<unsigned int>(lua_tonumber(L, -1)));
	}
	lua_pop(L, 1);
	return result;
}

void UserTuneConvertor::doConvertToLua(lua_State* L, boost::shared_ptr<UserTune> payload) {
	lua_createtable(L, 0, 0);
	if (payload->getRating()) {
		lua_pushnumber(L, (*payload->getRating()));
		lua_setfield(L, -2, "rating");
	}
	if (payload->getTitle()) {
		lua_pushstring(L, (*payload->getTitle()).c_str());
		lua_setfield(L, -2, "title");
	}
	if (payload->getTrack()) {
		lua_pushstring(L, (*payload->getTrack()).c_str());
		lua_setfield(L, -2, "track");
	}
	if (payload->getArtist()) {
		lua_pushstring(L, (*payload->getArtist()).c_str());
		lua_setfield(L, -2, "artist");
	}
	if (payload->getURI()) {
		lua_pushstring(L, (*payload->getURI()).c_str());
		lua_setfield(L, -2, "uri");
	}
	if (payload->getSource()) {
		lua_pushstring(L, (*payload->getSource()).c_str());
		lua_setfield(L, -2, "source");
	}
	if (payload->getLength()) {
		lua_pushnumber(L, (*payload->getLength()));
		lua_setfield(L, -2, "length");
	}
}

boost::optional<LuaElementConvertor::Documentation> UserTuneConvertor::getDocumentation() const {
	return Documentation(
		"UserTune",
		"This table has the following fields:\n\n"
		"- `rating`: number (Optional)\n"
		"- `title`: string (Optional)\n"
		"- `track`: string (Optional)\n"
		"- `artist`: string (Optional)\n"
		"- `uri`: string (Optional)\n"
		"- `source`: string (Optional)\n"
		"- `length`: number (Optional)\n"
	);
}
