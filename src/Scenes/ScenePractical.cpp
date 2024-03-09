#include "Pch.h"
//#include "SceneCollision.h"
//#include "GL\glew.h"
//#include "Application.h"
//#include <sstream>
//
//SceneCollision::SceneCollision()
//{
//}
//
//SceneCollision::~SceneCollision()
//{
//}
//
//void SceneCollision::Init()
//{
//	SceneBase::Init();
//
//	//Physics code here
//	m_speed = 1.f;
//	
//	Math::InitRNG();
//
//	m_objectCount = 0;
//
//	m_ghost = new GameObject(GameObject::GO_BALL);
//}
//
//GameObject* SceneCollision::FetchGO()
//{
//	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
//	{
//		GameObject *go = (GameObject *)*it;
//		if(!go->active)
//		{
//			go->active = true;
//			++m_objectCount;
//			return go;
//		}
//	}
//	for(unsigned i = 0; i < 10; ++i)
//	{
//		GameObject *go = new GameObject(GameObject::GO_BALL);
//		m_goList.push_back(go);
//	}
//	GameObject *go = m_goList.back();
//	go->active = true;
//	++m_objectCount;
//	return go;
//}
//
//bool SceneCollision::CheckCollision(GameObject *go1, GameObject *go2, float dt)
//{
//	return false;
//}
//
//void SceneCollision::Update(double dt)
//{
//	SceneBase::Update(dt);
//	
//	if(Application::IsKeyPressed('9'))
//	{
//		m_speed = Math::Max(0.f, m_speed - 0.1f);
//	}
//	if(Application::IsKeyPressed('0'))
//	{
//		m_speed += 0.1f;
//	}
//
//	//Mouse Section
//	static bool bLButtonState = false;
//	if(!bLButtonState && Application::IsMousePressed(0))
//	{
//		bLButtonState = true;
//		std::cout << "LBUTTON DOWN" << std::endl;
//		
//		double x, y;
//		Application::GetCursorPos(&x, &y);
//		int w = Application::GetWindowWidth();
//		int h = Application::GetWindowHeight();
//		float posX = static_cast<f32>(x) / w * m_worldWidth;
//		float posY = (h - static_cast<f32>(y)) / h * m_worldHeight;
//
//		m_ghost->pos.Set(posX, m_worldHeight * 0.5f, 0); //IMPT
//		m_ghost->active = true;
//		float sc = 2;
//		m_ghost->scale.Set(sc, sc, sc);
//	}
//	else if(bLButtonState && !Application::IsMousePressed(0))
//	{
//		bLButtonState = false;
//		std::cout << "LBUTTON UP" << std::endl;
//
//		//spawn small GO_BALL
//		GameObject *go = FetchGO();
//		go->active = true;
//		go->type = GameObject::GO_BALL;
//		double x, y;
//		Application::GetCursorPos(&x, &y);
//		int w = Application::GetWindowWidth();
//		int h = Application::GetWindowHeight();
//		float posX = static_cast<f32>(x) / w * m_worldWidth;
//		float posY = (h - static_cast<f32>(y)) / h * m_worldHeight;
//
//		go->pos = m_ghost->pos;
//		go->vel.Set(m_ghost->pos.x - posX, 0, 0);
//		m_ghost->active = false;
//		float sc = 2;
//		go->scale.Set(sc, sc, sc);
//		go->mass = sc * sc * sc;
//	}
//	static bool bRButtonState = false;
//	if(!bRButtonState && Application::IsMousePressed(1))
//	{
//		bRButtonState = true;
//		std::cout << "RBUTTON DOWN" << std::endl;
//
//		double x, y;
//		Application::GetCursorPos(&x, &y);
//		int w = Application::GetWindowWidth();
//		int h = Application::GetWindowHeight();
//		float posX = static_cast<f32>(x) / w * m_worldWidth;
//		float posY = (h - static_cast<f32>(y)) / h * m_worldHeight;
//
//		m_ghost->pos.Set(posX, m_worldHeight * 0.5f, 0); //IMPT
//		m_ghost->active = true;
//		float sc = 3;
//		m_ghost->scale.Set(sc, sc, sc);
//	}
//	else if(bRButtonState && !Application::IsMousePressed(1))
//	{
//		bRButtonState = false;
//		std::cout << "RBUTTON UP" << std::endl;
//
//		//spawn large GO_BALL
//		GameObject *go = FetchGO();
//		go->active = true;
//		go->type = GameObject::GO_BALL;
//		double x, y;
//		Application::GetCursorPos(&x, &y);
//		int w = Application::GetWindowWidth();
//		int h = Application::GetWindowHeight();
//		float posX = static_cast<f32>(x) / w * m_worldWidth;
//		float posY = (h - static_cast<f32>(y)) / h * m_worldHeight;
//
//		go->pos = m_ghost->pos;
//		go->vel.Set(m_ghost->pos.x - posX, 0, 0);
//		m_ghost->active = false;
//		float sc = 3;
//		go->scale.Set(sc, sc, sc);
//		go->mass = sc * sc * sc;
//	}
//
//	//Physics Simulation Section
//	dt *= m_speed;
//
//	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
//	{
//		GameObject *go = (GameObject *)*it;
//		if(go->active)
//		{
//			go->pos += go->vel * static_cast<f32>(dt);
//
//			//Exercise 2a: Rebound game object at screen edges
//			//Exercise 2b: Unspawn if it really leave the screen
//			if (go->pos.x > m_worldWidth || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.y < 0)
//			{
//				go->active = false;
//				--m_objectCount;
//			}
//
//			for(std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
//			{
//				GameObject *go2 = static_cast<GameObject *>(*it2);
//				if(go2->active)
//				{
//					//Exercise 1: move collision code to CheckCollision()
//					float distSquared = (go->pos - go2->pos).LengthSquared();
//					float combinedRadius = go->scale.x + go2->scale.x;
//					
//					//Practical 4, Exercise 13: improve collision detection algorithm
//					if(distSquared <= combinedRadius * combinedRadius)
//					{
//						m1 = go->mass;
//						m2 = go2->mass;
//						u1 = go->vel;
//						u2 = go2->vel;
//
//						initialMomentum = m1 * u1 + m2 * u2;
//
//						Vector3 momentum1 = go->mass * go->vel;
//						Vector3 momentum2 = go2->mass * go2->vel;
//						
//						go->vel = momentum2 * (1.f / go->mass);
//						go2->vel = momentum1 * (1.f / go2->mass);
//
//						v1 = go->vel;
//						v2 = go2->vel;
//						
//						finalMomentum = m1 * v1 + m2 * v2;
//
//						//Exercise 3: audit kinetic energy
//					}
//				}
//			}
//		}
//	}
//}
//
//
//void SceneCollision::RenderGO(GameObject *go)
//{
//	switch(go->type)
//	{
//	case GameObject::GO_BALL:
//		modelStack.PushMatrix();
//		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
//		RenderMesh(meshList[GEO_BALL], false);
//		modelStack.PopMatrix();
//		break;
//	}
//}
//
//void SceneCollision::Render()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	
//	//Calculating aspect ratio
//	m_worldHeight = 100.f;
//	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
//
//	// Projection matrix : Orthographic Projection
//	Mtx44 projection;
//	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
//	projectionStack.LoadMatrix(projection);
//	
//	// Camera matrix
//	viewStack.LoadIdentity();
//	viewStack.LookAt(
//						camera.position.x, camera.position.y, camera.position.z,
//						camera.target.x, camera.target.y, camera.target.z,
//						camera.up.x, camera.up.y, camera.up.z
//					);
//	// Model matrix : an identity matrix (model will be at the origin)
//	modelStack.LoadIdentity();
//	
//	RenderMesh(meshList[GEO_AXES], false);
//
//	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
//	{
//		GameObject *go = (GameObject *)*it;
//		if(go->active)
//		{
//			RenderGO(go);
//		}
//	}
//	if(m_ghost->active)
//		RenderGO(m_ghost);
//
//	//On screen text
//	std::ostringstream ss;
//	ss << "Object count: " << m_objectCount;
//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);
//
//	ss.str(std::string());
//	ss.precision(5);
//	ss << "Initial momentum: " << initialMomentum;
//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);
//	
//	ss.str(std::string());
//	ss.precision(5);
//	ss << "Final   momentum: " << finalMomentum;
//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);
//
//	//Exercise 3: render initial and final kinetic energy
//	
//	ss.str(std::string());
//	ss.precision(3);
//	ss << "Speed: " << m_speed;
//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
//	
//	ss.str(std::string());
//	ss.precision(5);
//	ss << "FPS: " << fps;
//	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
//	
//	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
//}
//
//void SceneCollision::Exit()
//{
//	SceneBase::Exit();
//	//Cleanup GameObjects
//	while(m_goList.size() > 0)
//	{
//		GameObject *go = m_goList.back();
//		delete go;
//		m_goList.pop_back();
//	}
//	if(m_ghost)
//	{
//		delete m_ghost;
//		m_ghost = NULL;
//	}
//}
