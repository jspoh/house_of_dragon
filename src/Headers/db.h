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


class Database {
private:
	Database();

	static Database* _instance;
	std::ifstream ifs;


public:
	~Database();

	static Database* getInstance();

	nlohmann::json data;

};
