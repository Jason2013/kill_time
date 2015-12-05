#include "stdafx.h"

#include "world.h"

//class COgroEnemy;
//class CSodEnemy;
//class CCamera;
//class CMD2Model;
//class CPlayer;

CWorld::CWorld()
{
	terrain = new CTerrain(32, 0.5);
	timer = NULL;
	//m_weapon = wRocket;
}

CWorld::~CWorld()
{
	audioSystem->Shutdown();

	delete gui;
	delete terrain;
	delete player;
	delete audioSystem;
	delete worldSound;

	gui = NULL;
	terrain = NULL;
	audioSystem = NULL;
	worldSound = NULL;
}

CWorld::CWorld(CCamera *c)
{
	camera = c;
	terrain = new CTerrain(32, 1.0f);
	c->terrain = terrain;
	player = new CPlayer;
	audioSystem = new CAudioSystem;
	gui = new CGUI;
	gui->SetWorld(this);

	gameDone = false;		// game is just starting

	audioSystem->InitDirectXAudio(NULL);

	player->AttachTo(terrain);
	player->SetCamera(camera);
	player->SetTerrain(terrain);
	player->SetWorld(this);

	worldSound = audioSystem->Create("ambient.wav", false);
	audioSystem->Play(worldSound, DMUS_SEG_REPEAT_INFINITE);

	player->SetAudioSystem(audioSystem);

	//LoadWorld();
	timeStart = 300.0;			// 300 seconds = 5 minutes
	timeElapsed = 0.0;
	gui->SetCurrentTime(timeStart);
	gui->SetEnemiesLeft(numOgros + numSods);

	memset(m_models, 0, sizeof(m_models));
	//m_weapon = wRocket;

	timer = NULL;
}

void CWorld::Animate(float deltaTime)//, float fps = 0.0f)
{
	// set camera height based on player position on terrain
	camera->position.y = terrain->GetHeight(camera->position.x, camera->position.z) + (player->size) * 2.0f;

	if (camera->position.x <= terrain->GetScanDepth())
		camera->position.x = terrain->GetScanDepth();
	if (camera->position.x >= terrain->GetWidth()*terrain->GetMul() - terrain->GetScanDepth())
		camera->position.x = terrain->GetWidth()*terrain->GetMul() - terrain->GetScanDepth();
	if (camera->position.z <= terrain->GetScanDepth())
		camera->position.z = terrain->GetScanDepth();
	if (camera->position.z >= terrain->GetWidth()*terrain->GetMul() - terrain->GetScanDepth())
		camera->position.z = terrain->GetWidth()*terrain->GetMul() - terrain->GetScanDepth();

	terrain->Animate(deltaTime);                 // animate terrain

	const type_info &ogro = typeid(COgroEnemy);  // get ogro typeid
	const type_info &sod = typeid(CSodEnemy);    // get sod typeid

	numOgros = CountObjectTypes(ogro);           // count ogros
	numSods = CountObjectTypes(sod);             // count sods

	gui->SetEnemiesLeft(numOgros + numSods);
	gui->SetCurrentTime(timeStart - timeElapsed);//, fps);
	gui->SetFPS(GetTimer()->GetFPS());

	if (!gameDone)
		timeElapsed += deltaTime;
	else
		timeElapsed = timeStart;
}

void CWorld::FadeScreen()
{
	GLint width, height;
	CGUI::EnterOrtho(&width, &height);

	width /= 2;
	height /=2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	glColor4f(0.0, 0.0, 0.0, 0.5);
	glBegin(GL_QUADS);
	glVertex2i(-width, -height);
	glVertex2i( width, -height);
	glVertex2i( width,  height);
	glVertex2i(-width,  height);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_STENCIL_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	CGUI::LeaveOrtho();
}

void CWorld::SetScreen(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
}

void CWorld::Draw(CCamera *camera)
{
	terrain->Draw(camera);
	//if (camera->UsingTelescope())
	//{
	//	DrawTelescope();
	//}
	gui->Draw();

	if (gameDone)
	{
		FadeScreen();
		if (numOgros + numSods <=0)
			gui->DrawWinner();
		else
			gui->DrawLoser();
	}
}

void CWorld::OnPrepare()
{
	glClearColor(terrain->fogColor[0], terrain->fogColor[1], terrain->fogColor[2], terrain->fogColor[3]);

	terrain->Prepare();

	if ((numOgros + numSods <= 0) || (timeElapsed >= timeStart))
		gameDone = true;
}

// LoadWorld()
// desc: initializes the world
void CWorld::LoadWorld()
{
	int enemyIdx = 0;
	int rndInt = 0;

	numOgros = 0;
	numSods = 0;

	srand((unsigned int)time(NULL));
	
	rndInt = (rand() % (MAX_ENEMIES-4)) + 4;	// random # from 4 to MAX
	//rndInt = 20;
	numOgros = numSods = rndInt;
	numOgros = numSods = 20; // numOgros =1; numSods =0;// 20;

	// generate ogros
	for (enemyIdx = 0; enemyIdx < numOgros; enemyIdx++)
	{
		ogroEnemy = new COgroEnemy(GetMd2Model(Orgo));
		ogroEnemy->AttachTo(terrain);
		ogroEnemy->SetPlayer(player);
		ogroEnemy->SetAudioSystem(audioSystem);
		ogroEnemy->position.x = (float)(rand() % (int)(terrain->GetWidth() * terrain->GetMul()));
		ogroEnemy->position.y = 0.0f;
		ogroEnemy->position.z = (float)(rand() % (int)(terrain->GetWidth() * terrain->GetMul()));
	}

	// generate sods
	for (enemyIdx = 0; enemyIdx < numSods; enemyIdx++)
	{
		sodEnemy = new CSodEnemy(GetMd2Model(Sod));
		sodEnemy->AttachTo(terrain);
		sodEnemy->SetPlayer(player);
		sodEnemy->SetAudioSystem(audioSystem);
		sodEnemy->position.x = (float)(rand() % (int)(terrain->GetWidth() * terrain->GetMul()));
		sodEnemy->position.y = 0.0f;
		sodEnemy->position.z = (float)(rand() % (int)(terrain->GetWidth() * terrain->GetMul()));
	}
}

// CountObjectTypes
// desc: counts the number of objects passed as classID in the hierarchy
int CWorld::CountObjectTypes(const type_info &classID)
{
	int c = 0;
	CNode * pNode = CNode::header;

	while (pNode != NULL)
	{
		if (typeid(*pNode) == classID)
		{
			c++;
		}
		pNode = pNode->next;
	}
	return c;
}

void CWorld::LoadGameResource()
{
	struct ModelRes
	{
		char * modelFile;
		char * skinFile;
	};
	static ModelRes gameRes[] ={
		{"models\\sodf8\\tris.md2", "models\\sodf8\\abarlith.pcx"},	// Sod
		{"models\\ogro\\tris.md2", "models\\ogro\\ogrobase.pcx"},	// Orgo
		{"models\\rocketair.md2", "models\\rocket.pcx"},	// Rocket
	};
	FreeGameResource();
	for (int i = 0; i< (Rocket - Sod + 1); i++)
	{
		m_models[i] = new CMD2Model(gameRes[i].modelFile, gameRes[i].skinFile);
	}
}
void CWorld::FreeGameResource()
{
	for (int i = 0; i< (Rocket - Sod + 1); i++)
	{
		if (m_models[i] != NULL)
		{
			delete m_models[i];
			m_models[i] = NULL;
		}
	}
}

//void CWorld::SwitchWeapon()
//{
//	if (m_weapon < wGun)
//	{
//		m_weapon ++;
//	}
//	else
//	{
//		m_weapon = wRocket;
//	}
//}

//void CWorld::DrawTelescope()
//{
//	GLint width, height;
//	CGUI::EnterOrtho(&width, &height);
//
//	GLfloat r = (width < height) ? (GLfloat)width : (GLfloat)height;
//	r /= 2.0f;
//
//	glClear(GL_STENCIL_BUFFER_BIT);
//
//	glEnable(GL_STENCIL_TEST);
//	glStencilFunc(GL_NEVER, 1, 1);
//	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
//
//
//	glBegin(GL_POLYGON);
//	for (float angle = 0.0f; angle < 3.1415926f * 2; angle += (3.1415926f / 30.0f) )
//	{
//		glVertex2f((GLfloat)sin(angle) * r, (GLfloat)cos(angle) * r);
//	}
//	glEnd();
//
//	glStencilFunc(GL_EQUAL, 0, 1);
//	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);
//
//	glColor3f(0,0,0);
//	glBegin(GL_QUADS);
//	glVertex3f((GLfloat)-width/2, (GLfloat)-height/2, -1);
//	glVertex3f((GLfloat)width/2, (GLfloat)-height/2, -1);
//	glVertex3f((GLfloat)width/2, (GLfloat)height/2, -1);
//	glVertex3f((GLfloat)-width/2, (GLfloat)height/2, -1);
//	glEnd();
//
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glEnable(GL_DEPTH_TEST);
//
//	glDisable(GL_STENCIL_TEST);
//
//	CGUI::LeaveOrtho();
//}