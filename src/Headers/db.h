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


#pragma once

#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>


const std::string dbPath = "data.json";

// !TODO: imlement all texture references and load them on init in RenderHelper. call from GameManager::Init
class Database {
private:
	Database();

	static Database* _instance;
	std::ifstream ifs;


public:
	~Database();

	static Database* getInstance();

	/**
	 * Warning: All data stored in this file will be updated on instance deletion.
	 * 
	 * Force update data at anytime with the `forceUpdate` function.
	 */
	nlohmann::json data;

	/**
	 * Update the json object data before calling this function.
	 * 
	 * updates the data.json file with current data in `data`
	 */
	bool forceUpdate();

};
