#ifndef OSAPI_MUTEX_WINDOWS_H
#define OSAPI_MUTEX_WINDOWS_H


class Mutex : public MutexInterface
{
    HANDLE WINAPI handler;
public:
	Mutex() {
        handler = CreateSemaphore(NULL, 1, 1, NULL);
	}
	virtual ~Mutex() {
        CloseHandle(handler);
	}
	virtual bool lock(unsigned int timeout) {
        return WaitForSingleObject(handler, timeout) == 0;

	}
	virtual void unlock() {
        ReleaseSemaphore(handler, 1, NULL);
	}
};



#endif // OSAPI_MUTEX_WINDOWS_H
