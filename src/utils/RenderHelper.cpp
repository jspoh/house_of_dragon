/* Start Header ************************************************************************/
/*!
\file RenderHelper.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief abstracts away drawing of shapes and textures
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Pch.h"
#include "RenderHelper.h"

namespace {
	/**
	 * .
	 * 
	 * \param transX
	 * \param transY
	 * \param scaleX
	 * \param scaleY
	 * \param rotation
	 * \param matrix [out] output transform matrix
	 */
	void getTransformMatrix(f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 rotation, AEMtx33& matrix) {
		// create matrix
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, scaleX, scaleY);
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, rotation);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, transX, transY);

		AEMtx33Concat(&matrix, &rotate, &scale);
		AEMtx33Concat(&matrix, &translate, &matrix);
	}
}

/*class draw*/
RenderHelper* RenderHelper::m_instance = nullptr;

RenderHelper::RenderHelper()
{

}

void RenderHelper::load() {
	// init reusable mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,  // bottom-left
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,   // bottom-right
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f	   // top-left
	);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,   // bottom-right
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,    // top-right
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f    // top-left
	);
	m_defaultMesh = AEGfxMeshEnd();

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00000000, 0.0f, 1.0f,  // bottom-left
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,   // bottom-right
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f	   // top-left
	);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,   // bottom-right
		0.5f, 0.5f, 0x00000000, 1.0f, 0.0f,    // top-right
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f    // top-left
	);
	m_invisibleMesh = AEGfxMeshEnd();

	m_meshRef["default"] = m_defaultMesh;
	m_meshRef["invis"] = m_invisibleMesh;

	// font
	m_font = AEGfxCreateFont("./Assets/Fonts/liberation-mono.ttf", m_fontSize);

	// init static array used for int ids
	std::fill(m_textureIdRefs.begin(), m_textureIdRefs.end(), nullptr);
}

bool RenderHelper::registerMeshByRef(std::string reference, AEGfxVertexList* mesh) {
	try {
		m_meshRef[reference] = mesh;
		return true;
	}
	catch (const std::exception& e) {
		cerr << "ERROR: " << "Failed to register mesh " << mesh << " with reference " << reference << " with error: " << e.what() << "\n";
		//throw std::exception();
		return false;
	}
	return false;
}

AEGfxVertexList* RenderHelper::getMeshByRef(std::string reference) {
	auto it = m_meshRef.find(reference);
	if (it != m_meshRef.end()) {
		return it->second;
	}
	cerr << "ERROR: " << "Mesh with ref " << reference << " does not exist!\n";
	//throw std::exception();
	return nullptr;
}

bool RenderHelper::removeMeshByRef(std::string reference) {
	try {
		AEGfxMeshFree(m_meshRef[reference]);
		m_meshRef.erase(reference);
		return true;
	}
	catch (const std::exception& e) {
		cerr << "ERROR: " << "Failed to remove mesh with reference " << reference << " with error: " << e.what() << "\n";
		//throw std::exception();
		return false;
	}
	return false;
}

RenderHelper::~RenderHelper() {
	//AEGfxMeshFree(m_defaultMesh);
	//AEGfxMeshFree(m_invisibleMesh);
	
	for (std::pair<std::string, AEGfxTexture*> map : m_textureRef) {
		cout << "Automatically removing texture with ref " << map.first << "\n";
		AEGfxTextureUnload(map.second);
		map.second = nullptr;
	}

	for (std::pair<std::string, AEGfxVertexList*> map : m_meshRef) {
		cout << "Automatically removing mesh with ref " << map.first << "\n";
		AEGfxMeshFree(map.second);
		map.second = nullptr;
	}

	for (AEGfxTexture* pTex : m_textureIdRefs) {
		if (pTex == nullptr) {
			continue;
		}
		cout << "Automatically removing texture with id \n";
		AEGfxTextureUnload(pTex);
		pTex = nullptr;
	}

	m_textureRef.clear();
	m_meshRef.clear();
}

RenderHelper* RenderHelper::getInstance() {
	if (m_instance == nullptr) {
		m_instance = new RenderHelper;
	}
	return m_instance;
}

void RenderHelper::setBackgroundColor(Color color) {
	AEGfxSetBackgroundColor(color.r, color.g, color.b);
}

void RenderHelper::rect(f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 rotation, Color color, f32 opacity) {
	AEMtx33 transform = { 0 };
	getTransformMatrix(transX, transY, scaleX, scaleY, rotation, transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(m_defaultMesh, AE_GFX_MDM_TRIANGLES);
}

void RenderHelper::rect(std::string meshRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 rotation, Color color, f32 opacity) {
	// guard to ensure mesh exists
	if (m_meshRef.find(meshRef) == m_meshRef.end()) {
		cerr << "ERROR: " << "Mesh with reference " << meshRef << " does not exist\n";
		return;
	}
	
	AEGfxVertexList* pMesh = m_meshRef[meshRef];
	
	AEMtx33 transform = { 0 };
	getTransformMatrix(transX, transY, scaleX, scaleY, rotation, transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

bool RenderHelper::registerTexture(std::string reference, std::string path) {
	cout << "Loading texture " << path << " with reference " << reference << "\n";

	if (m_textureRef.find(reference) != m_textureRef.end()) {
		// it is intended behaviour to catch all textures that has already been loaded to mitigate the issue of 
		// load being called on state reload. we do not want to free assets that has already been loaded to save
		// resources and load times
		
		//cerr << "ERROR: " << "Texture ref " << reference << " already in use!\n";
		//throw std::exception();
		return false;
	}

	AEGfxTexture* pTex = AEGfxTextureLoad(path.c_str());
	if (!pTex) {
		cerr << "ERROR: " << "Texture with reference " << reference << " and path " << path << " failed to load\n";
		//throw std::exception();
		return false;
	}
	m_textureRef[reference] = pTex;
	cout << "Loaded texture with string reference " << reference << " at location " << pTex << "\n";
	return true;
}

bool RenderHelper::registerTexture(int reference, std::string path) {
	if (reference >= m_MAX_TEXTURE_IDS) {
		cerr << "ERROR: " << "Texture reference id(" << reference << ") cannot be greater than array size!\n";
		//throw std::exception();
		return false;
	}
	if (m_textureIdRefs[reference] != nullptr) {
		// it is intended behaviour to catch all textures that has already been loaded to mitigate the issue of 
		// load being called on state reload. we do not want to free assets that has already been loaded to save
		// resources and load times

		//cerr << "ERROR: " << "int reference " << reference << " already used\n";
		//throw std::exception();
		return false;
	}

	AEGfxTexture* pTex = AEGfxTextureLoad(path.c_str());
	if (!pTex) {
		cerr << "ERROR: " << "Texture with reference " << reference << " and path " << path << " failed to load\n";
		//throw std::exception();
		return false;
	}
	m_textureIdRefs[reference] = pTex;
	cout << "Loaded texture with int reference " << reference << " at location " << pTex << "\n";
	return true;
}

AEGfxTexture* RenderHelper::getTextureByRef(std::string reference) {
	auto map = m_textureRef.find(reference.c_str());
	if (map == m_textureRef.end() || map->second == nullptr) {  // does not exist
		cerr << "ERROR: " << "Reference " << reference << " does not exist!\n";
		//throw std::exception();
		return nullptr;
	}
	return map->second;
}

AEGfxTexture* RenderHelper::getTextureByRef(int reference) {
	if (m_textureIdRefs[reference] == nullptr) {
		cerr << "ERROR: " << "Reference " << reference << " has not been set!\n";
	}
	return m_textureIdRefs[reference];
}

void RenderHelper::removeTextureByRef(std::string reference) {
	auto map = m_textureRef.find(reference.c_str());
	if (map == m_textureRef.end()) {  // does not exist
		cerr << "ERROR: " << "Reference " << reference << " does not exist!\n";
		return;
	}

	AEGfxTexture* pTex = getTextureByRef(reference);
	AEGfxTextureUnload(pTex);
	m_textureRef.erase(reference);

	cout << "Manually removed texture with ref " << reference << "\n";
}

void RenderHelper::removeTextureByRef(int reference) {
	AEGfxTexture* pTex = getTextureByRef(reference);
	if (pTex == nullptr) {
		cerr << "ERROR: " << "texture with int reference " << reference << " was never initialized!\n";
		return;
	}
	AEGfxTextureUnload(pTex);
	m_textureIdRefs[reference] = nullptr;

	cout << "Manually removed texture with ref " << reference << "\n";
}

void RenderHelper::texture(std::string textureRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 opacity, Color addColor, f32 rotation, Color defaultColor) {
	// create matrix
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, scaleX, scaleY);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, rotation);

	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, transX, transY);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	// prep texture
	AEGfxTexture* pTex = getTextureByRef(textureRef);  // doesnt


	// prepare to draw
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	AEGfxSetColorToAdd(addColor.r, addColor.g, addColor.b, addColor.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(pTex, 0, 0);
	AEGfxMeshDraw(m_defaultMesh, AE_GFX_MDM_TRIANGLES);
}

void RenderHelper::texture(int textureRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 opacity, Color addColor, f32 rotation, Color defaultColor) {
	// create matrix
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, scaleX, scaleY);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, rotation);

	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, transX, transY);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	// prep texture
	AEGfxTexture* pTex = getTextureByRef(textureRef);  // doesnt

	if (pTex == nullptr) {
		cerr << "ERROR: " << "Texture with reference " << textureRef << " wasnt initialized!\n";
		return;
	}


	// prepare to draw
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	AEGfxSetColorToAdd(addColor.r, addColor.g, addColor.b, addColor.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(pTex, 0, 0);
	AEGfxMeshDraw(m_defaultMesh, AE_GFX_MDM_TRIANGLES);
}

void RenderHelper::text(std::string s, float screenX, float screenY, float r, float g, float b, float a) {
	AEGfxSetRenderMode(AE_GFX_RM_NONE);

	AEVec2 p = ston(screenX, screenY);

	f32 width, height;
	AEGfxGetPrintSize(m_font, s.c_str(), 1, &width, &height);
	AEGfxPrint(m_font, s.c_str(), p.x - width / 2, p.y - height / 2, 1, r, g, b, a);
}

int RenderHelper::getFontSize() {
	return m_fontSize;
}

AEGfxVertexList* RenderHelper::getdefaultMesh()
{
	return m_defaultMesh;
}


