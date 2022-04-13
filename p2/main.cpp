#include "mbed.h"
#include "mbed_events.h"
#include <chrono>

using namespace std::chrono;

AnalogOut OutSig(D7);

EventQueue printfQueue(64 * EVENTS_EVENT_SIZE);

double cnt = 0.0;



void wavegen()
{
	while (1)
	{
        cnt += 0.01;
      	if (cnt >= 1.0)//0 ~ n-1
        {
            cnt = 0.0;
            printfQueue.call(printf, "One period is over\n");
        }
            
        
        if (cnt < 0.5)
            OutSig = 3.0 * tanh(cnt * 6);
        else
            OutSig = 3.0 * (1 - tanh(cnt * 6 - 3));
        
        ThisThread::sleep_for(1ms);
	}
}


int main()
{

	Thread DACThread(osPriorityNormal);
    DACThread.start(wavegen);

    Thread printThread(osPriorityNormal);
    printThread.start(callback(&printfQueue, &EventQueue::dispatch_forever));
    

	while (1)
	{
		ThisThread::sleep_for(1s);
	}
}