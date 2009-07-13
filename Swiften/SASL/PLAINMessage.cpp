#include "Swiften/SASL/PLAINMessage.h"

namespace Swift {

PLAINMessage::PLAINMessage(const String& authcid, const String& password, const String& authzid) : authcid(authcid), authzid(authzid), password(password) {
}

PLAINMessage::PLAINMessage(const ByteArray& value) {
	size_t i = 0;
	while (i < value.getSize() && value[i] != '\0') {
		authzid += value[i];
		++i;
	}
	if (i == value.getSize()) {
		return;
	}
	++i;
	while (i < value.getSize() && value[i] != '\0') {
		authcid += value[i];
		++i;
	}
	if (i == value.getSize()) {
		authcid = "";
		return;
	}
	++i;
	while (i < value.getSize()) {
		password += value[i];
		++i;
	}
}

ByteArray PLAINMessage::getValue() const {
	String s = authzid + '\0' + authcid + '\0' + password;
	return ByteArray(s.getUTF8Data(), s.getUTF8Size());
}

}
