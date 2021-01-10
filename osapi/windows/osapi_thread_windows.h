#ifndef OSAPI_THREAD_WINDOWS_H
#define OSAPI_THREAD_WINDOWS_H

#include <Windows.h>
/** Thread interface implementation for Windows. */
class Thread : public ThreadInterface
{
    int priority = 0;
    unsigned int stackSize = 0;
    Joinable isJoinableVariable = Joinable::NOT_JOINABLE;
    const char* name = nullptr;
    HANDLE handle = nullptr;
    bool isRunningVariable = false;

	public:
	
		/** Thread constructor.
	     *  @param[in] priority thread priority
	     *  @param[in] stackSize thread stack size in bytes
         *  @param[in] isJoinable decides if the thread supports join operation or not
         *  @param[in] name optional thread name
         */
        Thread(int priority, unsigned int stackSize, Joinable isJoinable, const char* name = "unnamed") :
            priority(priority), stackSize(stackSize), isJoinableVariable(isJoinable), name(name) {
        }
		
		/** Virtual destructor required to properly destroy derived class objects. */
		virtual ~Thread() {
			CloseHandle(handle);
		}

        static void threadFunction(void* argument) {
            Thread* osapiThreadObject = reinterpret_cast<Thread*>(argument);
            if (osapiThreadObject) {
                osapiThreadObject->body();
            }
       }

		/** Runs the thread.
		 *  @retval true if the thread was started successfully, 
		 *  @retval false if the thread was not started successfully, or the thread was already running
		 */
		virtual bool run() {
            handle = CreateThread(
                    nullptr,
                    stackSize,
                    reinterpret_cast<LPTHREAD_START_ROUTINE>(threadFunction),
                    this,
                    0, // run immediately
                    nullptr
            );

            if (handle != nullptr) {
                isRunningVariable = true;
                SetThreadPriority(handle, priority);
                return true;
            }
            return false;
		}
		
		/** Checks if the thread is running.
		 *  @retval true if the thread is running
         *  @retval false if the thread is not running
		 */
		virtual bool isRunning() {
			return handle != nullptr && isRunningVariable;
		}	

		/** Waits for the thread to finish executing, with a given timeout.
         *  @param timeout[in] number of milliseconds to wait for the thread to finish executing
		 *  @retval true if the thread was successfully joined in the given time
		 *  @retval false if the thread was not joined within the given time or the thread is not joinable at all
		 */
		virtual bool join(unsigned int timeout) {
			if (isJoinable()) {
			    return !WaitForSingleObject(handle, timeout);
			} else {
			    return false;
			}
		}

		/** Checks, if the thread is joinable.
		 *  @retval true if the thread is joinable
		 *  @retval false if the thread is not joinable
		 */
		virtual bool isJoinable() {
            return isJoinableVariable == Joinable::JOINABLE;
		}

		/** Suspends thread execution.
		 *  @retval true if the thread was suspended successfully
		 *  @retval false if the thread was not suspended for some reason
		 */
		virtual bool suspend() {
		    if (static_cast<int>(SuspendThread(handle)) != -1) {
		        isRunningVariable = false;
                return true;
		    } else {
		        return false;
		    }

		}

		/** Resumes thread execution.
		 *  @retval true if the thread was resumed successfully
		 *  @retval false if the thread was not resumed for some reason
		 */
		virtual bool resume() {
		    if (static_cast<int>(ResumeThread(handle)) != -1) {
		        isRunningVariable = true;
		        return true;
		    } else {
		        return false;
		    }
		}

		/** Sets thread priority
		 *  @param[in] priority new thread priority
		 *  @retval true if the priority for the thread was set successfully
		 *  @retval false if the priority for the thread was not set successfully for some reason
		 */
		virtual	bool setPriority(int priority) {
            return SetThreadPriority(handle, priority) != 0;
		}

		/** Gets the thread priority
		 *  @return current thread priority
		 */
		virtual	int getPriority() {
            return GetThreadPriority(handle);
        }
        
        /** Gets thread name
         *  @return name of the thread
         */
        virtual const char* getName() {
            return name;
        }
	
	protected:
		
		/** Delays thread execution for a given time.
		 *  @param time[in] number of milliseconds to delay thread execution
		 */
		virtual void delay(unsigned int time) {
            Sleep(time);
		}

		/** Causes the thread to temporarily yield execution in favor of other threads.
		 */
		virtual void yield() {
            delay(0);
		}
};


#endif /* OSAPI_THREAD_WINDOWS_H */
