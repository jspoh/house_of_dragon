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
	static T* GetInstance()
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