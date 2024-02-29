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

#include "db.h"

using json = nlohmann::json;


namespace {
	// shouldnt ever have to be called. json file should be supplied for levels
	void _initDb() {
		json data = {
			{
				"player", {
					{ "xpToNextLevel", 0 },
					{ "level", 1 },
					{ "inventory", json::array() },
				}
			},
			{
				"enemies", {
					{"cat", {
						{"texturePath", "./Assets/animals/cat.jpg"}
						}
					}
				}
			},
			{
				"items", {
					{
						"bacon", {
							{"texturePath", "./Assets/food/bacon.jpg"}
						}
					},
					{
						"beef", {
							{"texturePath", "./Assets/food/beef.jpg"}
						}
					},
					{
						"chicken", {
							{"texturePath", "./Assets/food/chicken.jpg"}
						}
					}
				}
			},
			{
				"levels", json::array()
			}
		};

		std::ofstream ofs{ "data.json" };
		ofs << data.dump(4);
	}
}


Database* Database::_instance = nullptr;


Database::Database() {
	// damn gay its looking in the exe directory!!!!!!!!!!!!!!!
	ifs = std::ifstream{ "data.json" };

	if (!ifs.is_open()) {
		std::cerr << "failed to open file" << std::endl;
	}

	std::string contents;
	std::string line;

	while (std::getline(ifs, line)) {
		contents += line + "\n";
	}

	std::cout << "contents: " << contents << std::endl;
	std::cout << "done dumping contents" << std::endl;

	//_initDb();
}


Database::~Database() {

}


Database* Database::getInstance() {
	if (!_instance) {
		_instance = new Database();
	}
	return _instance;
}
