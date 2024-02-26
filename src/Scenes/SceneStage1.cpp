#include "SceneStage1.h"

SceneStage1* SceneStage1::sInstance = new SceneStage1(SceneManager::GetInstance());

//double m_FloorSpeedTimer = 0.5;

SceneStage1::SceneStage1() : m_LevelBuilder(nullptr)
{
}	

SceneStage1::SceneStage1(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneStage1", this);
}

SceneStage1::~SceneStage1()
{
}

void SceneStage1::Load()
{
}

void SceneStage1::Init()
{
	std::cout << "Loading Scene Stage1" << std::endl;

	m_LevelBuilder = new SceneLevelBuilder();
}

void SceneStage1::Update(double dt)
{	
	static bool start = true;

	if (start)
	{
		m_LevelBuilder->Update(dt);

	}
}


void SceneStage1::Render()
{
	m_LevelBuilder->Render();
}

void SceneStage1::Exit()
{
	std::cout << "Exiting Scene SplashScreen" << std::endl;

	delete m_LevelBuilder;
	m_LevelBuilder = nullptr;
}



//2D Camera Movement - Screen Shake
//AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
//static float x = 0, y = 0;
//x += 0.3f;
//y += 0.3f;
//AEGfxSetCamPosition(x, y);

/**********************************************************/
//Calculation Profiler
//static int x = 580.0f, y = 194.f;
//if (AEInputCheckCurr(AEVK_W))
//{
//	y++;
//}
//if (AEInputCheckCurr(AEVK_S))
//{
//	y--;
//}
//if (AEInputCheckCurr(AEVK_A))
//{
//	x--;
//}
//if (AEInputCheckCurr(AEVK_D))
//{
//	x++;
//}
//static int mx = 0, my = 47;
//if (AEInputCheckCurr(AEVK_UP))
//{
//	my++;
//}
//if (AEInputCheckCurr(AEVK_DOWN))
//{
//	my--;
//}

//for (int i = 1; i < 9; i++)
//{
//	if (m_Floor[i].m_currFloorNum != 0)
//	{
//		m_Floor[i].m_TransformFloorCurr.m[0][0] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[0][0] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][0]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][0];
//		m_Floor[i].m_TransformFloorCurr.m[0][1] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[0][1] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][1]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][1];
//		m_Floor[i].m_TransformFloorCurr.m[0][2] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[0][2] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][2]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][2];
//		m_Floor[i].m_TransformFloorCurr.m[1][0] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[1][0] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][0]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][0];
//		m_Floor[i].m_TransformFloorCurr.m[1][1] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[1][1] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][1]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][1];
//		m_Floor[i].m_TransformFloorCurr.m[1][2] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[1][2] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][2]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][2];
//		m_Floor[i].m_TransformFloorCurr.m[2][0] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[2][0] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][0]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][0];
//		m_Floor[i].m_TransformFloorCurr.m[2][1] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[2][1] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][1]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][1];
//		m_Floor[i].m_TransformFloorCurr.m[2][2] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[2][2] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][2]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][2];
//	}
//	else
//	{
//		m_Floor[i].m_TransformFloorCurr.m[0][0] = m_Floor[6].m_TransformFloorData.m[0][0];
//		m_Floor[i].m_TransformFloorCurr.m[0][1] = m_Floor[6].m_TransformFloorData.m[0][1];
//		m_Floor[i].m_TransformFloorCurr.m[0][2] = m_Floor[6].m_TransformFloorData.m[0][2];
//		m_Floor[i].m_TransformFloorCurr.m[1][0] = m_Floor[6].m_TransformFloorData.m[1][0];
//		m_Floor[i].m_TransformFloorCurr.m[1][1] = m_Floor[6].m_TransformFloorData.m[1][1];
//		m_Floor[i].m_TransformFloorCurr.m[1][2] = m_Floor[6].m_TransformFloorData.m[1][2];
//		m_Floor[i].m_TransformFloorCurr.m[2][0] = m_Floor[6].m_TransformFloorData.m[2][0];
//		m_Floor[i].m_TransformFloorCurr.m[2][1] = m_Floor[6].m_TransformFloorData.m[2][1];
//		m_Floor[i].m_TransformFloorCurr.m[2][2] = m_Floor[6].m_TransformFloorData.m[2][2];
//
//	}
//	//Change CurrFloorNum
//	if (m_Floor[i].m_currFloorTimer > m_FloorSpeedTimer)
//	{
//		//m_Floor[i].m_currFloorTimer = 0.0;
//		m_Floor[i].m_currFloorTimer = m_FloorSpeedTimer;
//		//if (m_Floor[i].m_currFloorNum != 0)
//		//	m_Floor[i].m_currFloorNum--;
//		//else
//		//	m_Floor[i].m_currFloorNum = 6; //Loop to the top
//	}
//	else
//		m_Floor[i].m_currFloorTimer += dt;
//
//	//cout << (m_Floor[1].m_TransformFloorData.m[0][0] - m_Floor[2].m_TransformFloorData.m[0][0]) / (m_FloorSpeedTimer / dt) << endl;
//}




//AEMtx33 m_NextNextFloorData = m_Floor[m_Floor[i].m_currFloorNum - 2].m_TransformFloorData;
			//AEMtx33 m_NextFloorData = m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData;
			//AEMtx33 m_CurrFloorData = m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData;
			////Add its speed towards the next floor transition speed
			//m_Floor[i].m_currFloorSpeed += m_Floor[i].m_currFloorSpeed - (m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 100 ? ;
			//if (!((m_NextFloorData.m[0][0] - m_Floor[i].m_TransformFloorCurr.m[0][0]) < 1.0f &&
			//	(m_NextFloorData.m[1][0] - m_Floor[i].m_TransformFloorCurr.m[1][0]) < 1.0f &&
			//	(m_NextFloorData.m[2][0] - m_Floor[i].m_TransformFloorCurr.m[2][0]) < 1.0f &&
			//	(m_NextFloorData.m[0][1] - m_Floor[i].m_TransformFloorCurr.m[0][1]) < 1.0f &&
			//	(m_NextFloorData.m[1][1] - m_Floor[i].m_TransformFloorCurr.m[1][1]) < 1.0f &&
			//	(m_NextFloorData.m[2][1] - m_Floor[i].m_TransformFloorCurr.m[2][1]) < 1.0f &&
			//	(m_NextFloorData.m[0][2] - m_Floor[i].m_TransformFloorCurr.m[0][2]) < 1.0f &&
			//	(m_NextFloorData.m[1][2] - m_Floor[i].m_TransformFloorCurr.m[1][2]) < 1.0f &&
			//	(m_NextFloorData.m[2][2] - m_Floor[i].m_TransformFloorCurr.m[2][2]) < 1.0f))
			//{

			//	/*if (m_Floor[i].m_currFloorNum != 0)
			//	{
			//		m_Floor[i].m_TransformFloorCurr.m[0][0] += (m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][1] += (m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][2] += (m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][0] += (m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][1] += (m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][2] += (m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][0] += (m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][1] += (m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][2] += (m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 100;
			//	}*/
			//	if (m_Floor[i].m_currFloorNum != 0)
			//	{
			//		m_Floor[i].m_TransformFloorCurr.m[0][0] += (m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][1] += (m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][2] += (m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][0] += (m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][1] += (m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][2] += (m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][0] += (m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][1] += (m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][2] += (m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 100;
			//	}
			//	else
			//	{
			//		m_Floor[i].m_TransformFloorCurr.m[0][0] = m_LastFloorData.m[0][0];
			//		m_Floor[i].m_TransformFloorCurr.m[0][1] = m_LastFloorData.m[0][1];
			//		m_Floor[i].m_TransformFloorCurr.m[0][2] = m_LastFloorData.m[0][2];
			//		m_Floor[i].m_TransformFloorCurr.m[1][0] = m_LastFloorData.m[1][0];
			//		m_Floor[i].m_TransformFloorCurr.m[1][1] = m_LastFloorData.m[1][1];
			//		m_Floor[i].m_TransformFloorCurr.m[1][2] = m_LastFloorData.m[1][2];
			//		m_Floor[i].m_TransformFloorCurr.m[2][0] = m_LastFloorData.m[2][0];
			//		m_Floor[i].m_TransformFloorCurr.m[2][1] = m_LastFloorData.m[2][1];
			//		m_Floor[i].m_TransformFloorCurr.m[2][2] = m_LastFloorData.m[2][2];

			//	}