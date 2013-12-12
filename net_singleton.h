/*
 * net_singleton.h
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#ifndef NET_SINGLETON_H_
#define NET_SINGLETON_H_

#include <stdio.h>
#include "net_mutex.h"

template<typename T>
class CSingleton
{
public:
	//����ʵ��
    static T& CreateInstance()
	{
		return GetInstance();
	}
	//��ȡʵ��
	static T& GetInstance()
	{
		if (NULL == ms_pInstance)
		{
			MUTEX_GUARD(lock, m_stLock);
			if(NULL == ms_pInstance)
			{
				ms_pInstance = new T();
			}
		}

		return *ms_pInstance;
	}
	//����ʵ��
	static void DestroyInstance()
	{
		if (NULL != ms_pInstance)
		{
			MUTEX_GUARD(lock, m_stLock);
			if(NULL != ms_pInstance)
			{
				delete ms_pInstance;
				ms_pInstance = NULL;
			}
		}
	}

	//�����µ�ʵ�����ɵ�ʵ��������
	static T* SwapInstance(T* new_one)
	{
		MUTEX_GUARD(lock, m_stLock);

		T* old_one = ms_pInstance;
		ms_pInstance = new_one;
		return old_one;
	}

protected:
	static T	*ms_pInstance;
	static CriticalSection	m_stLock;

protected:
	CSingleton()
	{
	}
	virtual ~CSingleton()
	{
	}

private:
    // Prohibited actions
    CSingleton(const CSingleton &);
    CSingleton& operator = (const CSingleton &);

};

template<typename T>
T* CSingleton<T>::ms_pInstance = NULL;

template<typename T>
CriticalSection CSingleton<T>::m_stLock;


#endif /* NET_SINGLETON_H_ */
