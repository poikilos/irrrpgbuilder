#include "EffectsManager.h"

#include "App.h"

EffectsManager::EffectsManager()
{
    //ctor
}

EffectsManager::~EffectsManager()
{
    //dtor
}

EffectsManager* EffectsManager::getInstance()
{
    static EffectsManager *instance = 0;
    if (!instance) instance = new EffectsManager();
    return instance;
}

void EffectsManager::setWeather(float fogFactor, float rainFactor)
{

}

///TODO: define this function in Lua by setting AmbinetLight(COLOR_NAME)
void EffectsManager::setTimeOfDay(int newTime)
{
    newTime = newTime%24;

    int r,g,b;

    switch(newTime)
    {
        case 0:
            r=19;
            g=14;
            b=134;
            break;
        case 1:
            r=31;
            g=42;
            b=134;
            break;
        case 2:
            r=43;
            g=69;
            b=134;
            break;
        case 3:
            r=54;
            g=97;
            b=134;
            break;
        case 4:
            r=66;
            g=125;
            b=134;
            break;
        case 5:
            r=78;
            g=153;
            b=134;
            break;
        case 6:
            r=94;
            g=177;
            b=139;
            break;
        case 7:
            r=121;
            g=190;
            b=158;
            break;
        case 8:
            r=148;
            g=203;
            b=178;
            break;
        case 9:
            r=175;
            g=216;
            b=197;
            break;
        case 10:
            r=201;
            g=229;
            b=216;
            break;
        case 11:
            r=228;
            g=242;
            b=236;
            break;
        case 12:
            r=255;
            g=255;
            b=255;
            break;
        case 13:
            r=228;
            g=246;
            b=225;
            break;
        case 14:
            r=255;
            g=237;
            b=194;
            break;
        case 15:
            r=228;
            g=227;
            b=167;
            break;
        case 16:
            r=255;
            g=218;
            b=134;
            break;
        case 17:
            r=228;
            g=209;
            b=103;
            break;
        case 18:
            r=255;
            g=199;
            b=73;
            break;
        case 19:
            r=226;
            g=173;
            b=73;
            break;
        case 20:
            r=187;
            g=142;
            b=84;
            break;
        case 21:
            r=148;
            g=110;
            b=95;
            break;
        case 22:
            r=110;
            g=79;
            b=106;
            break;
        case 23:
            r=71;
            g=47;
            b=118;
            break;
    }

    App::getInstance()->getDevice()->getSceneManager()->setAmbientLight(SColorf(r/255.0f,g/255.0f,b/255.0f,1));
}
