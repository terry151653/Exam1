#include "mbed.h"
#include "uLCD_4DGL.h"

using namespace chrono;
uLCD_4DGL uLCD(D1, D0, D2);
bool flag = 0;
int cnt = 0;
Ticker startFlag;
Timer Debounce;
InterruptIn button(BUTTON1);

void tm()
{
    cnt++;
}
void btnfallirq()
{
    if (duration_cast<milliseconds>(Debounce.elapsed_time()).count() > 1000)
    {
        if (flag)
        {
            startFlag.detach();
            flag = 0;
        }
        else
        {
            flag = 1;
            startFlag.attach(tm, 10ms);
        }
        Debounce.reset();
    }
}
int main()
{
    Debounce.start();
    button.fall(btnfallirq);

    uLCD.background_color(WHITE);
	uLCD.text_width(2);
	uLCD.text_height(2);
	

    while (1)
    {
        uLCD.printf("\r%d:%d:%d", cnt / 6000, (cnt / 100) % 60, cnt % 100);
        //ThisThread::sleep_for(50ms);
    }
}