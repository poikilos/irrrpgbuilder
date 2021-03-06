#include "EffectsManager.h"
#include <iostream>

#include "../App.h"
#include "../camera/CameraSystem.h"
#include "../gui/GUIManager.h" //Required to send back stuff to the console (Will need to rework this to use APP instead)

//Plugged in componnent. There will be more
#include "postprocess/PostProcessManager.h"
#include "XEffects/XEffects.h"


EffectsManager::EffectsManager()
{

	// Init the post process manager (27 oct 2012, temporary disabled to improve loading speed, because of compiling shaders)
	// Will activate it later using the "initPostProcess()" command
	//initPostProcess();
	xeffects=false;
    //get main app scenemanager pointer
    ISceneManager* smgr=App::getInstance()->getDevice()->getSceneManager();
	driver = App::getInstance()->getDevice()->getVideoDriver();

	//Initialize the effect handler for XEffect.
	//effect = new EffectHandler(App::getInstance()->getDevice(), driver->getScreenSize(), true, true, true);
	effect = new EffectHandler(App::getInstance()->getDevice(), dimension2du(1980,1080), true, true, true);
	// Set a global ambient color. A very dark gray.
	effect->setAmbientColor(SColor(0,64,64,96));
	// Set the background clear color to black.
	effect->setClearColour(SColor(0x0));

	//TEsting the postFX from the XEffect system
	//effect->addPostProcessingEffectFromFile(core::stringc("../media/shaders/xeffects/BrightPass.glsl"));
	//effect->addPostProcessingEffectFromFile(core::stringc("../media/shaders/xeffects/BlurHP.glsl"));
	//effect->addPostProcessingEffectFromFile(core::stringc("../media/shaders/xeffects/BlurVP.glsl"));
	//effect->addPostProcessingEffectFromFile(core::stringc("../media/shaders/xeffects/BloomP.glsl"));
    
	//this is the main particle system, it is used to rain and snow effects
    mainParticleSystem = smgr->addParticleSystemSceneNode(false,//use default emitter
                                                          CameraSystem::getInstance()->getNode(),
                                                          -1,
                                                          vector3df(0,0,250));

    f32 emitterSize = 1024;
    emitter = mainParticleSystem->createBoxEmitter(
                                         aabbox3df(vector3df(0,0,0),vector3df(0,0,0)),
                                         vector3df(0.0f,-1.5f,0.5f),
                                         6500,
                                         9999,
                                         SColor(255,0,0,0),
                                         SColor(255,255,255,255),
										 12000,
										 24000);


    mainParticleSystem->setEmitter(emitter);
    mainParticleSystem->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    //default particle texture -> media/rain.png
    mainParticleSystem->setMaterialTexture(0,App::getInstance()->getDevice()->getVideoDriver()->getTexture("../media/rain.png"));

    mainParticleSystem->setVisible(false);

	// Set the default skydome
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../media/SKY_DOME.jpg"),32,16,0.95f,2.0f);
	
	skydome->setVisible(false);
	/*skybox=smgr->addSkyBoxSceneNode(
		driver->getTexture("../media/irrlicht2_up.jpg"),
		driver->getTexture("../media/irrlicht2_dn.jpg"),
		driver->getTexture("../media/irrlicht2_lf.jpg"),
		driver->getTexture("../media/irrlicht2_rt.jpg"),
		driver->getTexture("../media/irrlicht2_ft.jpg"),
		driver->getTexture("../media/irrlicht2_bk.jpg"));

	skybox->getMaterial(0).FogEnable=false;*/
	skydomestatus=true; // Default state for the skydome;

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

}

EffectsManager::~EffectsManager()
{
	if (postProcessManager)
		delete postProcessManager;

	if (effect)
		delete effect;

    emitter->drop();
}

EffectsManager* EffectsManager::getInstance()
{
    static EffectsManager *instance = 0;
    if (!instance) instance = new EffectsManager();
    return instance;
}

void EffectsManager::initPostProcess()
{
	// Temporary disable the post FX to check the shaders
	// Init the postProcess FX manager
	postProcessManager = new CPostProcessManager(App::getInstance()->getDevice());

	if (postProcessManager == 0)
		GUIManager::getInstance()->setConsoleText("LSuccess initialized the postprocess manager.",SColor(255,200,200,0));
	//postProcessManager->prepare(false);
	postProcessMode = 0;

}

void EffectsManager::setWeather(int maxParticles, float particlesSpeed, stringc textureFile="")
{
    //hide effect when user sets zero particles
    if(maxParticles == 0)
        mainParticleSystem->setVisible(false);
    else
        mainParticleSystem->setVisible(true);

    emitter->setMaxParticlesPerSecond(maxParticles);
    emitter->setMinParticlesPerSecond(u32(maxParticles/1.5f));
    emitter->setDirection(vector3df(0,-particlesSpeed,particlesSpeed*0.25f));

    if(textureFile!="")
        mainParticleSystem->setMaterialTexture(0,App::getInstance()->getDevice()->getVideoDriver()->getTexture(textureFile));
}

///TODO: define this function in Lua by setting AmbinetLight(COLOR_NAME)
void EffectsManager::setTimeOfDay(int newTime)
{
    newTime = newTime%24;

    int r=0,g=0,b=0;

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

void EffectsManager::update()
{
	// Update FX's
	// Will update the postprocess shader. Need to be called for every frame
	// There is also a "prepare function" needed to be called before rendering all the scene.
	// This is all in the "UPDATE" function in the APP class.

	// To check the framework, just set the postProcessMode to 1.
	//postProcessMode=1;
	if (postProcessMode>0)
	{
		//postProcessManager->render(EPPE_ADAPTIVE_BLOOM);
		postProcessManager->render(postEffect);
		postProcessManager->update();
		
		// bring back the gui after the RTT is done
		video::SMaterial mat;
		App::getInstance()->getDevice()->getVideoDriver()->setMaterial(mat);
	}

	//Update the position of the particle emitter for RAIN&SNOW FX
	vector3df pos1=CameraSystem::getInstance()->getPosition();
	vector3df pos=Player::getInstance()->getNode()->getPosition();
	pos.Y=pos1.Y;
	aabbox3df box=aabbox3df(vector3df(-1024+pos.X,0,-1024+pos.Z),vector3df(1024+pos.X,80,1024+pos.Z));
	emitter->setBox(box);


	/*
	printf("particles fx is update and position is: %f,%f,%f\n",pos.X,pos.Y,pos.Z);
	printf("Here is the box: %f,%f,%f and %f,%f,%f\n",
		emitter->getBox().MinEdge.X,
		emitter->getBox().MinEdge.Y,
		emitter->getBox().MinEdge.Z,
		emitter->getBox().MaxEdge.X,
		emitter->getBox().MaxEdge.Y,
		emitter->getBox().MaxEdge.Z); */

	if (xeffects)
	{
		//Reposition the directional light and try to cover the view frustum of the camera
		//vector3df campos = CameraSystem::getInstance()->getPosition();
		vector3df campos = Player::getInstance()->getNode()->getPosition();
		effect->getShadowLight(0).setPosition(vector3df(campos.X+5.0f,4096.0f,campos.Z-5.0f));
		effect->getShadowLight(0).setTarget(vector3df(campos.X,-256.0f,campos.Z));
		effect->update();
	}
	
}

void EffectsManager::preparePostFX(bool depth)
{
	if (postProcessMode>0)
		postProcessManager->prepare(depth);

}

void EffectsManager::DOFaddObject(ISceneNode * DOFNode)
{
	postProcessManager->addNodeToDepthPass(DOFNode);
}

void EffectsManager::DOFclearObjects()
{
	postProcessManager->clearDepthPass();
}

void EffectsManager::SetPostFX(stringc fxname)
{
	fxname.make_lower();
	if (fxname=="invert")
	{
		postEffect = EPPE_INVERT;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="sepia")
	{
		postEffect = EPPE_SEPIA;
		postProcessMode = 1;
		return;
	} else
	if (fxname == "grayscale")
	{
		postEffect = EPPE_GRAY_SCALE;
		postProcessMode = 1;
		return;
	}
	else
	if (fxname=="simple_bloom")
	{
		postEffect = EPPE_SIMPLE_BLOOM;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="adaptive_bloom")
	{
		postEffect = EPPE_ADAPTIVE_BLOOM;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="motion_blur")
	{
		postEffect = EPPE_MOTION_BLUR;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="night_vision")
	{
		postEffect = EPPE_NIGHT_VISION;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="dream_vision")
	{
		postEffect = EPPE_DREAM_VISION;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="posterize")
	{
		postEffect = EPPE_POSTERIZE;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="sharpen")
	{
		postEffect = EPPE_SHARPEN;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="embossed")
	{
		postEffect = EPPE_EMBOSSED;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="tiling")
	{
		postEffect = EPPE_TILING;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="displacement")
	{
		postEffect = EPPE_DISPLACEMENT;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="scratched")
	{
		postEffect = EPPE_SCRATCHED;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="pencil")
	{
		postEffect = EPPE_PENCIL;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="blur")
	{
		postEffect = EPPE_BLUR;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="water")
	{
		postEffect = EPPE_WATER;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="color")
	{
		postEffect = EPPE_COLOR;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="pulsing")
	{
		postEffect = EPPE_PULSING;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="shake")
	{
		postEffect = EPPE_SHAKE;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="desaturate")
	{
		postEffect = EPPE_DESATURATE;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="radial_blur")
	{
		postEffect = EPPE_RADIAL_BLUR;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="depth_of_field")
	{
		postEffect = EPPE_DEPTH_OF_FIELD;
		postProcessMode = 1;
		return;
	} else
	if (fxname=="vignette")
	{
		postEffect = EPPE_VIGNETTE;
		postProcessMode = 1;
		return;
	} else
	postProcessMode = 0;
}

// Set a new texture for the skydome
void EffectsManager::skydomeTexture(core::stringc file) 
{
	core::stringc path="../media/";
	path+=file;
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	video::ITexture* texture=App::getInstance()->getDevice()->getVideoDriver()->getTexture(path.c_str());
	if (texture)
		skydome->setMaterialTexture(0,texture);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
}

void EffectsManager::setBackgroundColor(const irr::video::SColor color)
{
	App::getInstance()->setIngameBackgroundColor(color);
}
