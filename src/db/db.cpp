/* Start Header ************************************************************************/
/*!
\file db.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief json implementation
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Pch.h"
#include "db.h"

using json = nlohmann::json;


namespace {

}


Database* Database::_instance = nullptr;


Database::Database() {
	ifs = std::ifstream{ dbPath };

	if (!ifs.is_open()) {
		std::cerr << "failed to open file" << std::endl;
	}

	this->data = json::parse(ifs);

	ifs.close();
}


Database::~Database() {
	forceUpdate();
}


Database* Database::getInstance() {
	if (!_instance) {
		_instance = new Database();
	}
	return _instance;
}


bool Database::forceUpdate() {
	try {
		std::ofstream ofs{ dbPath };
		ofs << data.dump(2);
		ofs.close();
	}
	catch (std::exception& e) {
		std::cerr << "failed to update file: " << e.what() << std::endl;
		return false;
	}
	return true;
}
