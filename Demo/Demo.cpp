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


void Display(SimpleApp* app, int) {
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	graphics->ActiveTextureShader();
	particleManager->Update();

	particleManager->Draw(graphics);
	//graphics->DrawImage(testImage);
	graphics->Present();
	//SDL_GL_SwapWindow(app->mGameWindow);
}

int main() {
	app = new GameApp(800,600,"Demo1",true,true);
	app->SetDisplayFunction(Display);
	SRand(GetTickCount());
	graphics = new Graphics(app);
	shadowImage = new SimpleImage();


	imageParticle = new SimpleImage();
	imageParticle->LoadFromFile("assets/particle/Pea_particles.png");

	Image1 = new SimpleImage();
	Image1->LoadFromFile("assets/particle/pea_splats_01.png");
	Image2 = new SimpleImage();
	Image2->LoadFromFile("assets/particle/pea_splats_02.png");
	Image3 = new SimpleImage();
	Image3->LoadFromFile("assets/particle/pea_splats_03.png");
	Image4 = new SimpleImage();
	Image4->LoadFromFile("assets/particle/pea_splats_04.png");

	shadowImage->LoadFromFile("assets/image/plantshadow.png");

	emitter = new Emitter();
	emitter->LoadFromFile("assets/paxml/PeaParticleThrow.xml");
	emitter->AttachImage(imageParticle);

	peaEmitter = new Emitter();
	peaEmitter->LoadFromFile("assets/paxml/PeaSplats.xml");
	peaEmitter->AttachImage(Image1);
	peaEmitter->AttachImage(Image2);
	peaEmitter->AttachImage(Image3);
	peaEmitter->AttachImage(Image4);

	particleManager = new ParticleManager();

	particleManager->MoveTo(200, 400, -200);
	particleManager->EmittParticle(peaEmitter);
	particleManager->EmittParticles(emitter,5);

	app->EnterMainLoop();
	delete app;
}