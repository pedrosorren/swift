/*
 * Copyright (c) 2013 Tobias Markmann
 * Licensed under the Simplified BSD license.
 * See Documentation/Licenses/BSD-simplified.txt for more information.
 */

#pragma once

#include <vector>
#include <boost/tuple/tuple.hpp>
#include <SwifTools/SpellChecker.h>

namespace Swift {
	class MacOSXChecker : public SpellChecker {
		public:
			MacOSXChecker();
			virtual ~MacOSXChecker();
			virtual bool isCorrect(const std::string& word);
			virtual void getSuggestions(const std::string& word, std::vector<std::string>& list);
			virtual void checkFragment(const std::string& fragment, PositionPairList& misspelledPositions);
	};
}
