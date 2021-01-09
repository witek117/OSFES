#ifndef OSAPI_MORTAL_THREAD_H
#define OSAPI_MORTAL_THREAD_H


class MortalThread : public Thread
{
    bool killSignal = false;
public:
    MortalThread(int priority, unsigned int stackSize, const char* name = "unnamed") : Thread(priority, stackSize, JOINABLE, name) {
        ;
    }

    virtual ~MortalThread() {
        ;
    }

    /** Sends termination signal to the thread. */
    void kill() {
        killSignal = true;
    }
    virtual void begin() { ; }
    virtual void loop () = 0;
    virtual void end() { ; }
private:
    /** Implementation of the body method */
    virtual void body(void) {
        begin();
        while(!killSignal){
            loop();
        }
        end();
    }
};

#endif // OSAPI_MORTAL_THREAD_H
