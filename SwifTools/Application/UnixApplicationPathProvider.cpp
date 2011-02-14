/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "SwifTools/Application/UnixApplicationPathProvider.h"

namespace Swift {

UnixApplicationPathProvider::UnixApplicationPathProvider(const std::string& name) : ApplicationPathProvider(name) {
	resourceDirs.push_back(getExecutableDir() / "../resources"); // Development
	char* xdgDataDirs = getenv("XDG_DATA_DIRS");
	if (xdgDataDirs) {
		std::vector<std::string> dataDirs = std::string(xdgDataDirs).split(':');
		if (!dataDirs.empty()) {
			foreach(const std::string& dir, dataDirs) {
				resourceDirs.push_back(boost::filesystem::path(dir) / "swift");
			}
			return;
		}
	}
	resourceDirs.push_back("/usr/local/share/swift");
	resourceDirs.push_back("/usr/share/swift");
}

boost::filesystem::path UnixApplicationPathProvider::getHomeDir() const {
	char* home = getenv("HOME");
	return home ? boost::filesystem::path(home) : boost::filesystem::path();
}

boost::filesystem::path UnixApplicationPathProvider::getDataDir() const {
	char* xdgDataHome = getenv("XDG_DATA_HOME");
	std::string dataDir;
	if (xdgDataHome) {
		dataDir = std::string(xdgDataHome);
	}

	boost::filesystem::path dataPath = (dataDir.empty() ? 
			getHomeDir() / ".local" / "share" 
			: boost::filesystem::path(dataDir)) / getApplicationName().getLowerCase();

	try {
		boost::filesystem::create_directories(dataPath);
	}
	catch (const boost::filesystem::filesystem_error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
	return dataPath;
}

}
