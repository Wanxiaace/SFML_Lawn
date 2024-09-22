#include "GameApp.h"
#include "Graphics.h"
#include "Particle.h"
#include <Windows.h>
using namespace sgf;

#undef main;

GameApp* app = nullptr;
Graphics* graphics = nullptr;
Emitter* emitter = nullptr;
Emitter* peaEmitter = nullptr;
ParticleManager* particleManager = nullptr;
SimpleImage* imageParticle = nullptr;
SimpleImage* Image1 = nullptr;
SimpleImage* Image2 = nullptr;
SimpleImage* Image3 = nullptr;
SimpleImage* Image4 = nullptr;
SimpleImage* shadowImage = nullptr;

static void Display(GameAppBase* app, int) {
	particleManager->Update();

	particleManager->Draw(graphics);

	graphics->Present();

	static int tick = 0;
	tick++;
	if(!(tick % 20))
		particleManager->EmittParticles(emitter, 1);

}

int main() {
	app = new GameApp(800,600,"Demo1",true,true);
	app->SetDisplayFunction(Display);
	app->mResourceManager.AttachBasePath("Demo/");
	app->LoadGraphics();
	app->mShowDebugWindow = true;

	SRand(GetTickCount());
	graphics = app->mGraphics;
	shadowImage = new SimpleImage();


	imageParticle = new SimpleImage();
	imageParticle->LoadFromFile("demo/particle/Pea_particles.png");

	Image1 = new SimpleImage();
	Image1->LoadFromFile("demo/particle/ZombieHead.png");
	Image2 = new SimpleImage();
	Image2->LoadFromFile("demo/particle/pea_splats_02.png");
	Image3 = new SimpleImage();
	Image3->LoadFromFile("demo/particle/pea_splats_03.png");
	Image4 = new SimpleImage();
	Image4->LoadFromFile("demo/particle/pea_splats_04.png");

	shadowImage->LoadFromFile("demo/plantshadow.png");

	emitter = new Emitter();
	emitter->LoadFromFile("demo/paxml/ZomHeadDrop.xml");
	emitter->AttachImage(Image1);

	peaEmitter = new Emitter();
	peaEmitter->LoadFromFile("demo/paxml/PeaSplats.xml");
	peaEmitter->AttachImage(Image1);
	//peaEmitter->AttachImage(Image2);
	//peaEmitter->AttachImage(Image3);
	//peaEmitter->AttachImage(Image4);

	particleManager = new ParticleManager();

	particleManager->MoveTo(200, 400, -200);
	//particleManager->EmittParticle(peaEmitter);
	particleManager->EmittParticles(emitter,5);

	app->EnterMainLoop();
	delete app;
}