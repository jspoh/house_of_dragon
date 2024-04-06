/* Start Header ************************************************************************/
/*!
\file SingletonTemplate.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief base template class for singletons
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#ifndef SINGLETON_TEMPLATE_H
#define SINGLETON_TEMPLATE_H

/*!************************************************************************
\file  SingletonTemplate.h
\author Soh Wei Jie (weijie.soh)
\par DP email: weijie.soh\@digipen.edu
\par Course: csd1451
\par House 0F Dragons
\date 23-1-2024
\brief Header file for math helper functions
**************************************************************************/

template <typename T>
class Singleton
{
public:
	static T* getInstance()
	{
		if (s_mInstance == nullptr)
			s_mInstance = new T();

		return s_mInstance;
	}

	static void Destroy()
	{
		if (s_mInstance)
		{
			delete s_mInstance;
			s_mInstance = nullptr;
		}
	}

protected:
	Singleton(){};
	virtual ~Singleton(){};

private:
	static T* s_mInstance;
};

template <typename T>
T* Singleton<T>::s_mInstance = nullptr;

#endif // SINGLETON_TEMPLATE_H