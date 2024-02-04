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
#include <unordered_map>
#include <string>


struct Color;

class Draw /*: Singleton<Draw>*/ {
private:
	static Draw* _instance;
	AEGfxVertexList* _mesh;

	std::unordered_map<std::string, AEGfxTexture*> _textureRef;

	Draw();
	~Draw();

protected:

public:
	static Draw* getInstance();

	//Draw* getInstance();

	/**
	 * Sets background color.
	 * 
	 * \param color
	 */
	void background(Color color);

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

	/*texture stuff*/
	/**
	 * register a texture(img) for `.texture` method to work properly.
	 * 
	 * \param reference reference you want to use
	 * \param path path to asset
	 * \returns if registration was successful
	 */
	bool registerTexture(std::string reference, std::string path);

	/**
	 * get AEGfxTexture pointer by reference.
	 * DO NOT free pointers returned by this function. call `removeTextureByRef` instead.
	 * 
	 * \param reference
	 * \return 
	 */
	AEGfxTexture* getTextureByRef(std::string reference);

	void removeTextureByRef(std::string reference);

	void texture(std::string textureRef, f32 transX = 0, f32 transY = 0, Color color = Color{ 0,0,0,0 }, f32 scaleX = 50, f32 scaleY = 50, f32 rotation = 0, f32 opacity = 1);
};

/*
class DrawSpritesheet {
private:
	std::vector<AEGfxVertexList*> _meshes;

public:
	DrawSpritesheet();
	~DrawSpritesheet();
};
*/
