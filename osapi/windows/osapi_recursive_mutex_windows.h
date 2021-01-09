#ifndef OSAPI_RECURSIVE_MUTEX_WINDOWS_H
#define OSAPI_RECURSIVE_MUTEX_WINDOWS_H

class RecursiveMutex : public MutexInterface
{
    HANDLE WINAPI handler;
public:
	RecursiveMutex() {
        handler = CreateMutex(NULL, TRUE, NULL);
	}
	virtual ~RecursiveMutex() {
        CloseHandle(handler);
	}
	virtual bool lock(unsigned int timeout) {
        return WaitForSingleObject(handler, timeout) == 0;
	}
	virtual void unlock() {
        ReleaseMutex(handler);
	}
};


#endif // OSAPI_RECURSIVE_MUTEX_WINDOWS_H
