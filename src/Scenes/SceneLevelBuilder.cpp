#include "SceneLevelBuilder.h"

int t_CenterFloorNum = static_cast<int>(SIZE_OF_FLOOR / 2);

SceneLevelBuilder::SceneLevelBuilder()
	:pMesh(nullptr)
{
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue
	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	//LOAD ALL TEXTURES - SHIFT TO RENDERHELPER
	pFloorTex = AEGfxTextureLoad("Assets/Scene_Floor_Grass_3D.png");
	pSideRightFloorTex = AEGfxTextureLoad("Assets/Scene_FloorSideRight_Sand_3D.png");
	pSideLeftFloorTex = AEGfxTextureLoad("Assets/Scene_FloorSideLeft_Sand_3D.png");
	pSkyTex = AEGfxTextureLoad("Assets/Scene_Sky_Clear.png");
	pSunOverlayTex = AEGfxTextureLoad("Assets/Scene_Sun_Overlaylighting.png");
	pFogTex = AEGfxTextureLoad("Assets/Scene_Fog_Color.png");

	m_Floor = new v_FloorData * [SIZE_OF_FLOOR];
	for (int i = 0; i < SIZE_OF_FLOOR; i++)
	{
		m_Floor[i] = new v_FloorData[NUM_OF_TILES];
	}

	Init();
}
SceneLevelBuilder::~SceneLevelBuilder()
{
	Exit();
}

void SceneLevelBuilder::Init()
{
	/*******************************************************************************/
	//MAIN FLOOR
	AEMtx33 scale = { 0 }, trans = { 0 };
	for (int j = 0; j < SIZE_OF_FLOOR; j++)
	{
		for (int i = 0; i < NUM_OF_TILES; i++)
		{
			switch (i)
			{
				//Out of Screen Floor
			case 0:
				AEMtx33Scale(&scale, 8000.f, 1262.f);
				AEMtx33Trans(&trans, 16000 * (j-t_CenterFloorNum), -2829);
				break;
			case 1:
				AEMtx33Scale(&scale, 7000.f, 1262.f);
				AEMtx33Trans(&trans, 5750 * (j - t_CenterFloorNum), -2229);
				break;
			case 2:
				AEMtx33Scale(&scale, 6000.f, 1262.f);
				AEMtx33Trans(&trans, 4350 * (j - t_CenterFloorNum), -1629);
				break;
				//First Floor
			case 3:
				AEMtx33Scale(&scale, 2940.f, 616.f);
				AEMtx33Trans(&trans, 2150 * (j - t_CenterFloorNum), -696);
				break;
				//Second Floor
			case 4:
				AEMtx33Scale(&scale, 1593.0f, 339.f);
				AEMtx33Trans(&trans, 1158 * (j - t_CenterFloorNum), -282);
				break;
				//Third floor
			case 5:
				AEMtx33Scale(&scale, 779.0f, 133.f);
				AEMtx33Trans(&trans, 556 * (j - t_CenterFloorNum), -50);
				break;
				//Fourth floor
			case 6:
				AEMtx33Scale(&scale, 381.0f, 47.f);
				AEMtx33Trans(&trans, 276 * (j - t_CenterFloorNum), 39);
				break;
				//Fifth floor
			case 7:
				AEMtx33Scale(&scale, 181.0f, 14.f);
				AEMtx33Trans(&trans, 131 * (j - t_CenterFloorNum), 69);
				break;
				//Sixth floor
			case 8:
				AEMtx33Scale(&scale, 85.0f, 4.f);
				AEMtx33Trans(&trans, 62 * (j - t_CenterFloorNum), 78);
				break;
				//Seventh floor
			case 9:
				AEMtx33Scale(&scale, 33.0f, 1.f);
				AEMtx33Trans(&trans, 25 * (j - t_CenterFloorNum), 80);
				break;
			default:
				std::cout << "Error pls check floor" << std::endl;
				break;
			}
			AEMtx33Concat(&m_Floor[j][i].m_TransformFloorData, &trans, &scale);
			m_Floor[j][i].m_currFloorNum = i;
			//Setting Movement Point To
			m_Floor[j][i].m_TransformFloorCurr = m_Floor[j][i].m_TransformFloorData;
		}
	}

	/////////////////////////////////////////////////////////////
	// ETC Transformations
	//DO SKY DATA
	AEMtx33Scale(&scale, 2000.0f, 400.f);
	AEMtx33Trans(&trans, 0, 200);
	AEMtx33Concat(&m_TransformSkyData, &trans, &scale);

	//Do Sun Data
	AEMtx33Scale(&scale, 50.0f, 50.f);
	AEMtx33Trans(&trans, 150, 250);
	AEMtx33Concat(&m_TransformSunData, &trans, &scale);
	AEMtx33Scale(&scale, 120.0f, 120.f);
	AEMtx33Trans(&trans, 149, 250);
	AEMtx33Concat(&m_TransformSunOverlayData, &trans, &scale);

	//DO FOG DATA
	AEMtx33Scale(&scale, 2000.0f, 70.f);
	AEMtx33Trans(&trans, 0, 80);
	AEMtx33Concat(&m_TransformFogData, &trans, &scale);
}
void SceneLevelBuilder::Update(double dt)
{
	////////////////////////////////////////////////////////////////
	//UPDATE FLOOR
	for (int j = 0; j < SIZE_OF_FLOOR; j++)
	{
		AEMtx33 m_LastFloorData = m_Floor[j][8].m_TransformFloorData;
		for (int i = NUM_OF_TILES - 1; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_Floor[j][i].m_currFloorNum != 0 ? m_Floor[j][m_Floor[j][i].m_currFloorNum - 1].m_TransformFloorData : m_Floor[j][i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_Floor[j][m_Floor[j][i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,//CHANGE THIS FOR PANNING CLOSER TO GROUND
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_Floor[j][i].m_currFloorSpeed.m[0][0] += m_Floor[j][i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_Floor[j][i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[0][1] += m_Floor[j][i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_Floor[j][i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[0][2] += m_Floor[j][i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_Floor[j][i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[1][0] += m_Floor[j][i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_Floor[j][i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[1][1] += m_Floor[j][i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_Floor[j][i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[1][2] += m_Floor[j][i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_Floor[j][i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[2][0] += m_Floor[j][i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_Floor[j][i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[2][1] += m_Floor[j][i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_Floor[j][i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_Floor[j][i].m_currFloorSpeed.m[2][2] += m_Floor[j][i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_Floor[j][i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_Floor[j][i].m_TransformFloorCurr.m[0][0] += m_Floor[j][i].m_currFloorSpeed.m[0][0];
			m_Floor[j][i].m_TransformFloorCurr.m[0][1] += m_Floor[j][i].m_currFloorSpeed.m[0][1];
			m_Floor[j][i].m_TransformFloorCurr.m[0][2] += m_Floor[j][i].m_currFloorSpeed.m[0][2];
			m_Floor[j][i].m_TransformFloorCurr.m[1][0] += m_Floor[j][i].m_currFloorSpeed.m[1][0];
			m_Floor[j][i].m_TransformFloorCurr.m[1][1] += m_Floor[j][i].m_currFloorSpeed.m[1][1];
			m_Floor[j][i].m_TransformFloorCurr.m[1][2] += m_Floor[j][i].m_currFloorSpeed.m[1][2];
			m_Floor[j][i].m_TransformFloorCurr.m[2][0] += m_Floor[j][i].m_currFloorSpeed.m[2][0];
			m_Floor[j][i].m_TransformFloorCurr.m[2][1] += m_Floor[j][i].m_currFloorSpeed.m[2][1];
			m_Floor[j][i].m_TransformFloorCurr.m[2][2] += m_Floor[j][i].m_currFloorSpeed.m[2][2];

			if (m_Floor[j][i].m_currFloorTimer > m_Floor[j][i].m_FloorSpeedTimer)
			{
				m_Floor[j][i].m_currFloorTimer = 0.0;
				if (m_Floor[j][i].m_currFloorNum > 1)
				{
					m_Floor[j][i].m_currFloorNum--;
					m_Floor[j][i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_Floor[j][i].m_currFloorNum = 8;
					m_Floor[j][i].m_currFloorSpeed = { 0 };
					m_Floor[j][i].m_TransformFloorCurr = m_LastFloorData;
					m_Floor[j][i].m_IsRender = false;
				}
			}
			else
				m_Floor[j][i].m_currFloorTimer += dt;
		}
	}
}
void SceneLevelBuilder::Render()
{
	// Set the background to black.
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	////////////////////////////////////////////////////////////////////////
	//SKY RENDER
	AEGfxSetTransparency(1.0f);
	//Sky
	AEGfxTextureSet(pSkyTex, 0, 0);
	AEGfxSetTransform(m_TransformSkyData.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	//Sun
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(0.0f);
	AEGfxSetTransform(m_TransformSunData.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(pSunOverlayTex, 0, 0);
	AEGfxSetTransform(m_TransformSunOverlayData.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	//////////////////////////////////////////////////////////////////////////////////////////
	//// Floors
	//// //////////////////////////////////////////////////////////////////////////////////
	//// Tell the engine to get ready to draw something with texture.
	//// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	//Main Floor
	AEGfxTextureSet(pFloorTex, 0, 0);
	for (int i = 0; i < NUM_OF_TILES; i++)
	{
		if (m_Floor[t_CenterFloorNum][i].m_IsRender)
		{
			AEGfxSetTransform(m_Floor[t_CenterFloorNum][i].m_TransformFloorCurr.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
	//Left Floor
	AEGfxTextureSet(pSideLeftFloorTex, 0, 0);
	for (int j = 0; j < SIZE_OF_FLOOR - (t_CenterFloorNum + 1); j++)
	{
		for (int i = 0; i < NUM_OF_TILES; i++)
		{
			if (m_Floor[j][i].m_IsRender)
			{
				AEGfxSetTransform(m_Floor[j][i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
	//Right Floor
	AEGfxTextureSet(pSideRightFloorTex, 0, 0);
	for (int j = (t_CenterFloorNum + 1); j < SIZE_OF_FLOOR; j++)
	{
		for (int i = 0; i < NUM_OF_TILES; i++)
		{
			if (m_Floor[j][i].m_IsRender)
			{
				AEGfxSetTransform(m_Floor[j][i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	//Fog
	AEGfxTextureSet(pFogTex, 0, 0);
	AEGfxSetTransform(m_TransformFogData.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

}
void SceneLevelBuilder::Exit()
{
	//Clear All Texture
	AEGfxMeshFree(pMesh);
	AEGfxTextureUnload(pFloorTex);
	AEGfxTextureUnload(pSideRightFloorTex);
	AEGfxTextureUnload(pSideLeftFloorTex);
	AEGfxTextureUnload(pSkyTex);
	AEGfxTextureUnload(pSunOverlayTex);
	AEGfxTextureUnload(pFogTex);

	//Clear Floor
	for (int i = 0; i < SIZE_OF_FLOOR; i++)
	{
		delete[] m_Floor[i];
	}
	delete[] m_Floor;

	//Clear Wall

	//Clear Object in scene
}