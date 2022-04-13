#include "mbed.h"
#include "stm32l475e_iot01_accelero.h"
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ratio>

#define PI 3.14159265
using namespace std::chrono;

DigitalOut led(LED1);
InterruptIn btnRecord(BUTTON1);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
Ticker k;
Timer durationtest;
Timeout ledduration;

int16_t pDataXYZ[3] = {0};
int16_t initpDataXYZ[3] = {0};
double initb = 0;
int16_t avgpDataXYZ[3] = {0};
double theta = 0;
int durationofsample = 0;

void record(void)
{
    static int cnt = 0;
    BSP_ACCELERO_AccGetXYZ(pDataXYZ);
    avgpDataXYZ[0] = pDataXYZ[0] / 10;
    avgpDataXYZ[1] = pDataXYZ[1] / 10;
    avgpDataXYZ[2] = pDataXYZ[2] / 10;
    if (cnt++ == 9)
    {
        theta = acos ((avgpDataXYZ[0] * initpDataXYZ[0] + avgpDataXYZ[1] * initpDataXYZ[1] + avgpDataXYZ[2] * initpDataXYZ[2]) / (initb * sqrt(avgpDataXYZ[0] * avgpDataXYZ[0] + avgpDataXYZ[1] * avgpDataXYZ[1] + avgpDataXYZ[2] * avgpDataXYZ[2]))) * 180.0 / PI;
        cnt = 0;
        printf("%f\n", theta);
    }
}

void startRecord(void)
{
    printf("---start---\n");
    durationtest.start();
    k.attach(queue.event(record), 10ms);
}
void stopRecord(void)
{

    printf("---stop---\n");
    k.detach();
    durationtest.stop();
    led = 1;
    durationofsample = chrono::duration_cast<chrono::milliseconds>(durationtest.elapsed_time()).count();
    ledduration.attach_us([]{led = !led;}, durationofsample * 1000);
    durationtest.reset();
    printf("%d\n", durationofsample);
    for (auto &i : avgpDataXYZ)
        i = 0;
}

int main() {
    int cnt = 0;
   printf("Start accelerometer init\n");
   BSP_ACCELERO_Init();
   BSP_ACCELERO_AccGetXYZ(initpDataXYZ);
   initb = sqrt(initpDataXYZ[0] * initpDataXYZ[0] + initpDataXYZ[1] * initpDataXYZ[1] + initpDataXYZ[2] * initpDataXYZ[2]);
   t.start(callback(&queue, &EventQueue::dispatch_forever));
   btnRecord.fall(queue.event(startRecord));
   btnRecord.rise(queue.event(stopRecord));
   while (1)
   {
       ThisThread::sleep_for(1s);
   }
}