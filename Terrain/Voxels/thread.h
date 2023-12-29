#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <thread>
#include <mutex>
#include <vector>
#include <shared_mutex>

class CMutex
{
public:
	CMutex() {}
	~CMutex() {}

	void Init() { InitializeCriticalSection(&m_Cs); }
	int Shutdown() { DeleteCriticalSection(&m_Cs); }

	void Lock() { EnterCriticalSection(&m_Cs); }
	void Unlock() { LeaveCriticalSection(&m_Cs); }

private:
	CRITICAL_SECTION m_Cs;
};

template<class T>
class monitor
{
public:
	template<typename ...Args>
	monitor(Args&&... args) : m_cl(std::forward<Args>(args)...) {}

	struct monitor_helper
	{
		monitor_helper(monitor* mon) : m_mon(mon), m_ul(mon->m_lock) {}
		T* operator->() { m_ul->Lock(); T* m_nmon = &m_mon->m_cl; m_ul->Unlock(); return m_nmon; }
		monitor* m_mon;
		CMutex*  m_ul;
	};

	void Init_Cs(CMutex * cs) {
		m_lock = cs;
	}

	monitor_helper operator->() { return monitor_helper(this); }
	monitor_helper ManuallyLock() { return monitor_helper(this); }
	T& GetThreadUnsafeAccess() { return m_cl; }

private:
	T           m_cl;
	CMutex*		m_lock;
};

struct threadpool_information {
	int total_objects_in_queue;
	CMutex * cs;
};

typedef void(*threadpool_task_fn)(threadpool_information threadpoolinf, void *parameter);
class CThreadPool
{
public:
	enum threadpool_errors {
		TP_NO_ERROR = 0,
		TP_ERROR_CREATE_QUEUE,
		TP_ERROR_CREATE_THREADS_HEAP
	};

	enum threadpool_creation_options {
		THREADPOOL_EXEC_AFTER_CREATION = 0,	 //запустить пул потоков сразу после создания
		THREADPOOL_EXEC_DEFFERED			 //отложенный запуск пула потоков
	};

	struct worker_data {
		threadpool_task_fn callback;
		void *parameter;
	};


	CThreadPool() {}
	CThreadPool(int execution_state, int queue_size = 1, int queue_limit = 65535)
	{
		SYSTEM_INFO sysinf;
		GetSystemInfo(&sysinf);
		init(execution_state, sysinf.dwNumberOfProcessors, queue_size, queue_limit);
	}

	void Init(int execution_state, int queue_size = 1, int queue_limit = 65535)
	{
		SYSTEM_INFO sysinf;
		GetSystemInfo(&sysinf);
		init(execution_state, sysinf.dwNumberOfProcessors, queue_size, queue_limit);
	}

	~CThreadPool() {}

	monitor<std::vector<worker_data>> m_nQueue;

	void AddTask(threadpool_task_fn callback, void *parameter) {
		worker_data data;
		data.callback = callback;
		data.parameter = parameter;
		//printf("Pushed to queue. Queue size: %d\n", m_nQueue.QueueSize());
		m_nQueue->push_back(data);
	}

	int Shutdown() {
		for (int i = 0; i < m_hThreads->size(); i++)
			CloseHandle(m_hThreads->at(i));

		return 1;
	}

	void Execute() {
		for (int i = 0; i < m_hThreads->size(); i++)
			ResumeThread(m_hThreads->at(i));
	}

	void Pause() {
		for (int i = 0; i < m_hThreads->size(); i++)
			ResumeThread(m_hThreads->at(i));
	}

	static DWORD WINAPI TaskThreadProc(LPVOID data) {
		CThreadPool *_this = (CThreadPool *)data;
		int size;

		while (true) {
			_this->cs.Lock();
			if (!_this->m_nQueue->empty()) {

				worker_data* pwork_data = &_this->m_nQueue->back();
				_this->m_nQueue->pop_back();
				threadpool_information threadpool_info;
				threadpool_info.total_objects_in_queue = _this->m_nQueue->size();
				threadpool_info.cs = &_this->cs;

				//выполняем задание
				_this->cs.Unlock();
				pwork_data->callback(threadpool_info, pwork_data->parameter);
				continue;
			}
			else
			{
				_this->cs.Unlock();
				Sleep(1);
			}
			//LeaveCriticalSection(&_this->cs);
		}
		return 0;
	}
private:
	void init(int execution_state, int number_of_workers, int queue_size = 1, int queue_limit = 65535)
	{
		DWORD threadCreationFlags = NULL;
		if (execution_state == THREADPOOL_EXEC_DEFFERED)
			threadCreationFlags = CREATE_SUSPENDED;
		cs.Init();
		m_hThreads.Init_Cs(&cs);
		m_nQueue.Init_Cs(&cs);
		HANDLE hThread = NULL;
		for (size_t i = 0; i < number_of_workers; i++)
		{
			hThread = NULL;
			hThread = CreateThread(NULL, NULL, TaskThreadProc, this, threadCreationFlags, NULL);
			if (hThread == NULL)
				continue;
			m_hThreads->push_back(hThread);
		}
	}

	int m_nThreads;
	int m_nReadyThreads;
	CMutex cs;
	monitor<std::vector<HANDLE>> m_hThreads;
};