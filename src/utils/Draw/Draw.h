/* Start Header ************************************************************************/
/*!
\file Draw.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief abstracts away drawing of shapes
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#pragma once

#include "../utils.h"


struct Color;


class Draw /*: Singleton<Draw>*/ {
private:
	static Draw* _instance;
	AEGfxVertexList* _mesh;

	Draw();
	~Draw();

protected:

public:
	static Draw* getInstance();

	//Draw* getInstance();

	/**
	 * .
	 *
	 * \param color
	 * \param scaleX
	 * \param scaleY
	 * \param rotation anticlockwise rotation of `rotation` degrees
	 * \param transX
	 * \param transY
	 * \param opacity [0.0, 1.0]
	 */
	void rect(f32 transX = 0, f32 transY = 0, Color color = Color{ 1,1,1,1 }, f32 scaleX = 50, f32 scaleY = 50, f32 rotation = 0, f32 opacity = 1);
};