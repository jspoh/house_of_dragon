/* Start Header ************************************************************************/
/*!
\file Scene.h
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief base scene class
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Load() = 0;
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif // SCENE_H