#include "SpaceScene.h"
#include "Editor.h"
#include "OpenGL.h"

SpaceScene::SpaceScene()
{
	debugMode = false;
	worldObject = new GameObject("world Object");
}

SpaceScene::SpaceScene(string tempName)
{
	debugMode = false;
	name = tempName;
	worldObject = new GameObject("world Object");
}

SpaceScene::~SpaceScene()
{
}

void SpaceScene::render()
{
	CHECK_GL_ERROR();
	//set the clear colour background 
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	CHECK_GL_ERROR();

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	CHECK_GL_ERROR();
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	CHECK_GL_ERROR();

	//clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//blend alpha channel
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//backface culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);


	glUseProgram(shaders["main"]->getShader());
	//update();

	CHECK_GL_ERROR();
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &input->getMVPmatrix()[0][0]);
	CHECK_GL_ERROR();
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &ModelMatrix[0][0]);
	CHECK_GL_ERROR();

	glUniform3f(lightDirLoc, input->getlookAtPoint().x, input->getlookAtPoint().y, input->getlookAtPoint().z);
	CHECK_GL_ERROR();
	glUniform3f(lightPosLoc, playerObj->getWorldPos().x, playerObj->getWorldPos().y, playerObj->getWorldPos().z);
	CHECK_GL_ERROR();
	glUniform3f(ambientMatLoc, ambientMaterialColor.x, ambientMaterialColor.y, ambientMaterialColor.z);
	CHECK_GL_ERROR();
	glUniform3f(ambientLightLoc, ambientLightColor.x, ambientLightColor.y, ambientLightColor.z);
	CHECK_GL_ERROR();
	glUniform3f(diffuseMatLoc, diffuseMaterialColor.x, diffuseMaterialColor.y, diffuseMaterialColor.z);
	CHECK_GL_ERROR();
	glUniform3f(diffuseLightLoc, diffuseLightColor.x, diffuseLightColor.x, diffuseLightColor.x);
	CHECK_GL_ERROR();
	glUniform3f(SpecMatLoc, specularMaterialColor.x, specularMaterialColor.y, specularMaterialColor.y);
	CHECK_GL_ERROR();
	glUniform1f(specPowLoc, specularPower);
	CHECK_GL_ERROR();
	glUniform3f(specLightLoc, specularLightColor.x, specularLightColor.y, specularLightColor.z);
	CHECK_GL_ERROR();
	glUniform1f(matShininess, materialShininess);
	CHECK_GL_ERROR();

	//glUniform1i(textureSamplerLocation, 0);
	CHECK_GL_ERROR();

	glDepthMask(GL_TRUE);
	CHECK_GL_ERROR();

	worldObject->render(fustrum);

	CHECK_GL_ERROR();
}

void SpaceScene::update()
{
	GLenum err = GL_NO_ERROR;
	//update MVP
	input->Update();
	
	//update scene graph
	worldObject->update(input->getMVPmatrix());

	CHECK_GL_ERROR();

	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	//update old postprocessing light. this code should have been removed 
	UpdateLightPerspMVP();
	depthBias = biasMatrix * depthMVP;

	//if the time between last frame is more than 30 seconds spawn next wave
	if (SDL_GetTicks() - lastWaveSpawnTimer >= spawnTimerForEnenyWaves)
	{
		spawnShips();
	}

	//update light position should have been removed for build 
	gLight.position = glm::vec3(playerObj->getWorldPos().x, playerObj->getWorldPos().y, playerObj->getWorldPos().z);
	gLightPosLoc = glGetUniformLocation(shaders["main"]->getShader(), "light.position");
	CHECK_GL_ERROR();
}

void SpaceScene::createScene()
{
	cout << "creating space scene" << endl;

	//CreateFrameBuffer(); ENable for post processing
	glEnable(GL_DEPTH_TEST);
	CHECK_GL_ERROR();

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	CHECK_GL_ERROR();

	//editor = new Editor(this);
	debugMode = true;

	//create input
	input = new GamePlayerController();

	//bullet physics
	bulPhys = new BulletPhys();
	bulPhys->CreateGroundPlane();
	AsteroidSphereID = bulPhys->CreateSphereShape(20.0);
	missileBoxID = bulPhys->CreateBoxShape(btVector3(10, 10, 10));
	shipBoxID = bulPhys->CreateBoxShape(btVector3(15, 15, 15));


	setUpCubemap();
	setUpMeshes();
	setUpTextures();
	setUpShaders();
	setUpAudio();

	//create player/debug cam
	input = new GamePlayerController();
	fustrum = new Fustrum(input);
	fustrum->setUpCamera();
	fustrum->updateCamera();
	worldObject->setScene(this);

	//loadScene(worldObject, name, this);

	//current object for easy parent
	GameObject *tempObj;

	//scene node
	worldObject->addChild(new GameObject("SpaceNode", worldObject));	//creating node
	tempObj = worldObject->getChild("SpaceNode"); //setting temp object for easy access
	tempObj->setPosition(vec3(0, 0, 0));
	tempObj->setActive(true);
	levelNode = tempObj;
	
	//player
	playerObj = new GameObject("player", tempObj, input);
	tempObj->addChild(playerObj);
	inputComp = new GameInputComponent(tempObj->getChild("player"));
	AudioClip* explosion = new AudioClip(Sounds["Explosion"]);
	AudioClip* RockitFire = new AudioClip(Sounds["RocketFire"]);
	audio->AddmovingAudio(RockitFire);
	inputComp->assignMissile(objects["missile"], shaders["main"], textures["missile"], missileBoxID, bulPhys, explosion, RockitFire);
	playerObj->addComponent(inputComp);
	playerObj->setPosition(vec3(0, 0, 0));
	playerObj->setScale(vec3(0.5, 0.5, 0.5));
	playerObj->addComponent(new Renderer(tempObj->getChild("player")));	//adding render comp
	playerObj->setShader(shaders["main"]);
	playerObj->setTexture(textures["GreenShip"]);
	playerObj->setModel(objects["ship"]);
	playerObj->setForceRender(true);
	playerObj->setForceNoRender(true);
	
	

	// example object
	string testName = "test";
	tempObj->addChild(new GameObject(testName, tempObj, objects["TestTeapot"], textures["TestSun"], shaders["main"]));	//creating object
	tempObj->getChild(testName)->addComponent(new Renderer(tempObj->getChild(testName)));	//adding render comp
	//physicsComponent* phys = new physicsComponent(tempObj->getChild(testName), bulPhys->CreatePhysBox(btVector3(0, 0, 0), 1, AsteroidSphereID, COL_AST), bulPhys);
	//tempObj->getChild(testName)->addComponent(phys); //adding physics comp
	tempObj->getChild(testName)->setPosition(vec3(0, 0, 0));	//changing postiion
	tempObj->getChild(testName)->setRotation(vec3(0, 0, 0));	//change rotaion
	tempObj->getChild(testName)->setScale(vec3(1, 1, 1));	//change scele
	tempObj->getChild(testName)->setForceRender(true);

	spawnAsteroidClusters();
	spawnShips();

	//set skybox
	//worldObject->addChild(new GameObject("skybox", worldObject, objects["cubeMesh"], skyMaterial, shaders["main"]));
	//worldObject->getChild("skybox")->addComponent(RENDER_COMPONENT);
	//worldObject->getChild("skybox")->setForceRender(true);
	//worldObject->getChild("skybox")->setScale(vec3(20, 20, 20));	//change scele

	cout << "world: " << worldObject->getName() << " components: ";
	worldObject->printComponents();
	worldObject->printChildern();

	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		//Process/log the error.
		cout << "error in creating audio " << err << endl;
	}

	//unuzed light parameters 
	materialShininess = 100;
	gLight.intensities = glm::vec3(1.0f, 1.0f, 1.0f); //white
	gLight.position = glm::vec3(0, 0, 0);
	gLight.attenuation = 1.0f;
	gLight.ambientCoefficient = 0.305f;

	//set shader
	GLuint shaderID = shaders["main"]->getShader();

	//get Shader locations
	lightDirLoc = glGetUniformLocation(shaderID, "lig.direction");
	CHECK_GL_ERROR();
	lightPosLoc = glGetUniformLocation(shaderID, "lig.position");
	CHECK_GL_ERROR();
	ambientMatLoc = glGetUniformLocation(shaderID, "mat.ambientCol");
	CHECK_GL_ERROR();
	ambientLightLoc = glGetUniformLocation(shaderID, "lig.ambientCol");
	CHECK_GL_ERROR();
	diffuseMatLoc = glGetUniformLocation(shaderID, "mat.diffuseCol");
	CHECK_GL_ERROR();
	diffuseLightLoc = glGetUniformLocation(shaderID, "lig.diffuseCol");
	CHECK_GL_ERROR();
	SpecMatLoc = glGetUniformLocation(shaderID, "mat.specularCol");
	CHECK_GL_ERROR();
	specPowLoc = glGetUniformLocation(shaderID, "lig.SpecularPow");
	CHECK_GL_ERROR();
	specLightLoc = glGetUniformLocation(shaderID, "lig.specularCol");
	CHECK_GL_ERROR();
	matShininess = glGetUniformLocation(shaderID, "mat.shininess");
	CHECK_GL_ERROR();

	MVPLocation = glGetUniformLocation(shaderID, "MVP");

	CHECK_GL_ERROR();
}

void SpaceScene::UpdateLightPerspMVP()
{
	// Compute the MVP matrix from the light's point of view
	mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	mat4 depthModelMatrix = glm::mat4(1.0);
	depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

}

void SpaceScene::destroyScene()
{
	for (auto const& x : objects)
	{
		x.second->cleanUp();
	}
	
	for (auto const& x : textures)
	{
		x.second->cleanUp();
	}
	for (auto const& x :shaders)
	{
		x.second->cleanUp();
	}
	delete BackgroundAudio;
	delete audio;
}

void SpaceScene::SceneLoop()
{
	windowLoop();
	audio->updateAudio(playerObj->getWorldPos());
	bulPhys->updatePhysics();
	update();
	render();
}

void SpaceScene::setUpShaders()
{
	//create shaders
	Shader * currentShader = new Shader("main");
	currentShader->attatchVertexShader("/final Shaders/textureSpecularVS.glsl");
	currentShader->attatchFragmentShader("/final Shaders/textureSpecularFS.glsl");
	currentShader->createShader();
	shaders.insert(pair<string, Shader*>("main", currentShader));

	shaders.insert(pair<string, Shader*>("toonMaterial", new Shader("toonMaterial")));
	shaders["toonMaterial"]->attatchVertexShader("/specularVS.glsl");
	shaders["toonMaterial"]->attatchFragmentShader("/toonSpecularFS.glsl");
	shaders["toonMaterial"]->createShader();

	shaders.insert(pair<string, Shader*>("sky", new Shader("sky")));
	shaders["sky"]->attatchVertexShader("/skyboxVS.glsl");
	shaders["sky"]->attatchFragmentShader("/skyboxFS.glsl");
	shaders["sky"]->createShader();
}

void SpaceScene::setUpMeshes()
{
	//create objects
	objects.insert(pair<string, Object*>("TestWalker", new Object("TestWalker")));
	objects["TestWalker"]->createBuffer("/heroWalker.FBX");

	objects.insert(pair<string, Object*>("TestTeapot", new Object("TestTeapot")));
	objects["TestTeapot"]->createBuffer("/utah-teapot.FBX");

	for (size_t i = 1; i < 6; i++)
	{
		string name = "asteroid" + to_string(i);

		objects.insert(pair<string, Object*>(name, new Object(name)));
		objects[name]->createBuffer("/" + name + ".FBX");

		TotalAsteroidMeshCount++;
	}

	objects.insert(pair<string, Object*>("missile", new Object("missile")));
	objects["missile"]->createBuffer("/AGM-114HellFire.FBX");

	objects.insert(pair<string, Object*>("ship", new Object("ship")));
	objects["ship"]->createBuffer("/Ender Battlecruiser.FBX");

	//for cubemap
	//objects.insert(pair<string, Object*>("cubeMesh", new Cube("cubeMesh")));
	//objects["cubeMesh"]->createBuffer();
}

void SpaceScene::setUpTextures()
{
	//create textures
	textures.insert(pair<string, Texture*>("TestSun", new Texture("TestSun")));
	textures["TestSun"]->createTexture("/SunTexture.png");

	textures.insert(pair<string, Texture*>("TestWalker", new Texture("TestWalker")));
	textures["TestWalker"]->createTexture("/unsung-map.jpg");

	//loop loading in all the asteroid textures
	for (size_t i = 1; i < 16; i++)
	{
		string name = "AM" + to_string(i);

		textures.insert(pair<string, Texture*>(name, new Texture(name)));
		textures[name]->createTexture("/" + name + ".jpg");

		TotalAsteroidTextureCount++;
	}

	textures.insert(pair<string, Texture*>("missile", new Texture("missile")));
	textures["missile"]->createTexture("/HellFireTexture.png");

	textures.insert(pair<string, Texture*>("GreenShip", new Texture("GreenShip")));
	textures["GreenShip"]->createTexture("/bc-grey-green.png");

	textures.insert(pair<string, Texture*>("RedShip", new Texture("RedShip")));
	textures["RedShip"]->createTexture("/bc-grey-red.png");

}

void SpaceScene::setUpCubemap()
{
	//create cubemap texture
	skyMaterial = new CubeTexture("skybox");
	string skyBoxFront = ASSET_PATH + TEXTURE_PATH + "/skybox/Space_front.png";
	string skyBoxBack = ASSET_PATH + TEXTURE_PATH + "/skybox/Space_back.png";
	string skyBoxLeft = ASSET_PATH + TEXTURE_PATH + "/skybox/Space_left.png";
	string skyBoxRight = ASSET_PATH + TEXTURE_PATH + "/skybox/Space_right.png";
	string skyBoxTop = ASSET_PATH + TEXTURE_PATH + "/skybox/Space_top.png";
	string skyBoxBottom = ASSET_PATH + TEXTURE_PATH + "/skybox/Space_bottom.png";
	skyMaterial->loadSkyBoxTextures(skyBoxFront, skyBoxBack, skyBoxLeft, skyBoxRight, skyBoxTop, skyBoxBottom);
}

void SpaceScene::setUpAudio()
{
	//audio
	ALenum  error;
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}
	audio = new OpenAL();
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

	//background 
	Sounds.insert(pair<string, ALuint>("backgroundAudio", audio->CreateBuffer("/SpacialHarvest.wav")));

	//ship 
	Sounds.insert(pair<string, ALuint>("ShipAudio", audio->CreateBuffer("/sergeniousShip.wav")));

	//rocket 
	Sounds.insert(pair<string, ALuint>("RocketFire", audio->CreateBuffer("/rocket-shoot.wav")));

	//explosion 
	Sounds.insert(pair<string, ALuint>("Explosion", audio->CreateBuffer("/explosion-01.wav")));
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

	BackgroundAudio = new AudioClip(Sounds["backgroundAudio"]);
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

	BackgroundAudio->Play(1.0f, vec3(0, 0, 0));
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}

	BackgroundAudio->setLooping(true);
	if ((error = alutGetError()) != AL_NO_ERROR)
	{
		cout << alutGetErrorString(error) << endl;
	}
}

void SpaceScene::spawnAsteroidClusters()
{
	//set up this 3D array to store asteroid cluster location
	positionNodes.resize(3);
	for (int i = 0; i < 3; ++i) {
		positionNodes[i].resize(3);

		for (int j = 0; j < 3; ++j)
			positionNodes[i][j].resize(3);
	}

	//spawn asteroid clusters in these locations 
	//center
	positionNodes[1][1][1] = vec3(0, 0, 0);
	spawnAsteroids(positionNodes[1][1][1]);

	//forward
	positionNodes[2][1][1] = vec3(AsteroidSpawnBoxSize, 0, 0);
	spawnAsteroids(positionNodes[2][1][1]);
	//backwards
	positionNodes[0][1][1] = vec3(-AsteroidSpawnBoxSize, 0, 0);
	spawnAsteroids(positionNodes[0][1][1]);
	//up
	positionNodes[1][2][1] = vec3(0, AsteroidSpawnBoxSize, 0);
	spawnAsteroids(positionNodes[1][2][1]);
	//down
	positionNodes[1][0][1] = vec3(0, -AsteroidSpawnBoxSize, 0);
	spawnAsteroids(positionNodes[1][0][1]);
	//right
	positionNodes[1][1][2] = vec3(0, 0, AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[1][1][2]);
	//left
	positionNodes[1][1][0] = vec3(0, 0, -AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[1][1][0]);

	//forwards up
	positionNodes[2][2][1] = vec3(AsteroidSpawnBoxSize, AsteroidSpawnBoxSize, 0);
	spawnAsteroids(positionNodes[2][2][1]);
	//forwards up right
	positionNodes[2][2][2] = vec3(AsteroidSpawnBoxSize, AsteroidSpawnBoxSize, AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[2][2][2]);
	//forwards up left
	positionNodes[2][2][0] = vec3(AsteroidSpawnBoxSize, AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[2][2][0]);
	//forwards down
	positionNodes[2][0][1] = vec3(AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize, 0);
	spawnAsteroids(positionNodes[2][0][1]);
	//forwards down right
	positionNodes[2][0][2] = vec3(AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize, AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[2][0][2]);
	//forwards down left
	positionNodes[2][0][0] = vec3(AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[2][0][0]);

	//backwards up
	positionNodes[0][2][1] = vec3(-AsteroidSpawnBoxSize, AsteroidSpawnBoxSize, 0);
	spawnAsteroids(positionNodes[0][2][1]);
	//backwards up right
	positionNodes[0][2][2] = vec3(-AsteroidSpawnBoxSize, AsteroidSpawnBoxSize, AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[0][2][2]);
	//backwards up left
	positionNodes[0][2][0] = vec3(-AsteroidSpawnBoxSize, AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[0][2][0]);
	//backwards down
	positionNodes[0][0][1] = vec3(-AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize, 0);
	spawnAsteroids(positionNodes[0][0][1]);
	//backwards down right
	positionNodes[0][0][2] = vec3(-AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize, AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[0][0][2]);
	//backwards down left
	positionNodes[0][0][0] = vec3(-AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize, -AsteroidSpawnBoxSize);
	spawnAsteroids(positionNodes[0][0][0]);
}

void SpaceScene::spawnAsteroids(vec3 location)
{
	//spawn asteroids in random locations maybe do a check to see if they are inside each other but maybe not
	srand(time(NULL));

	//spawn the number of asteroids for the value in the .h file 
	for (int i = 0; i < startingAsteroidCount; i++)
	{
		//name th asteroid 
		string name = "ast" + to_string(curAsteroidCount);

		//random position
		vec3 startingPos = vec3(rand() % AsteroidSpawnBoxSize - (AsteroidSpawnBoxSize/2), rand() % AsteroidSpawnBoxSize - (AsteroidSpawnBoxSize / 2), rand() % AsteroidSpawnBoxSize - (AsteroidSpawnBoxSize / 2));

		//offset this positon so it is inside the cluster
		vec3 spawnPos = vec3(startingPos.x + location.x, startingPos.y + location.y, startingPos.z + location.z);

		//create game object with random mesh and random texture
		levelNode->addChild(new GameObject(name, levelNode, objects["asteroid" + to_string(rand() % TotalAsteroidMeshCount + 1)], textures["AM" + to_string(rand() % TotalAsteroidTextureCount + 1)], shaders["main"]));	//creating object
		levelNode->getChild(name)->addComponent(new Renderer(levelNode->getChild(name)));	//adding render comp
		//add pbysics component
		physicsComponent* phys = new physicsComponent(levelNode->getChild(name), bulPhys->CreatePhysBox(btVector3(spawnPos.x, spawnPos.y, spawnPos.z), asteroidMass, AsteroidSphereID, COL_AST), bulPhys);
		levelNode->getChild(name)->addComponent(phys); //adding physics comp
		levelNode->getChild(name)->setPosition(spawnPos);	//changing postiion
		levelNode->getChild(name)->setRotation(vec3(0, 0, 0));	//change rotaion
		levelNode->getChild(name)->setScale(vec3(1, 1, 1));	//change scele
		levelNode->getChild(name)->setForceRender(true);

		//incrament the total number of asteroids so there arent 2 objects the same name
		curAsteroidCount++;

		//ad asteroid componet
		AsteroidComponent* AstComp = new AsteroidComponent(levelNode->getChild(name), playerObj);
		levelNode->getChild(name)->addComponent(AstComp);
		//add to asteroid list
		Asteroids.push_front(levelNode->getChild(name));
	}
}

void SpaceScene::spawnShips()
{
	lastWaveSpawnTimer = SDL_GetTicks();

	Ships.clear();
	for (int i = 0; i < startingEnemyCount; i++)
	{
		//name 
		string name = "ene" + to_string(curEnemyCount);

		//random starting location
		vec3 startingPos = vec3(rand() % (2000 - 1000) - 1000, rand() % (2000 - 1000) - 1000, rand() % (2000 - 1000) - 1000);

		//create object
		levelNode->addChild(new GameObject(name, levelNode, objects["ship"], textures["RedShip"], shaders["main"]));	//creating object
		levelNode->getChild(name)->addComponent(new Renderer(levelNode->getChild(name)));	//adding render comp

		//create phycics and add
		btRigidBody* body = bulPhys->CreatePhysBox(btVector3(startingPos.x, startingPos.y, startingPos.z), enemyMass, AsteroidSphereID, COL_ENEMY);
		physicsComponent* phys = new physicsComponent(levelNode->getChild(name), body, bulPhys);
		levelNode->getChild(name)->addComponent(phys); //adding physics comp
		levelNode->getChild(name)->setPosition(startingPos);	//changing postiion
		levelNode->getChild(name)->setRotation(vec3(0, 0, 0));	//change rotaion
		levelNode->getChild(name)->setScale(vec3(0.2, 0.2, 0.2));	//change scele
		levelNode->getChild(name)->setForceRender(true);
		//create AI
		AIComponent* AIComp = new AIComponent(levelNode->getChild(name), phys, playerObj, &curEnemyCount);
		levelNode->getChild(name)->addComponent(AIComp);

		//create audio clips 
		AudioClip* explosion = new AudioClip(Sounds["Explosion"]);
		AudioClip* RockitFire = new AudioClip(Sounds["RocketFire"]);
		audio->AddmovingAudio(RockitFire);
		AIComp->assignMissile(objects["missile"], shaders["main"], textures["missile"], missileBoxID, bulPhys, explosion, RockitFire);

		curEnemyCount++;

		Ships.push_front(levelNode->getChild(name));
	}
}


//turns out it is very difficult to move the camera and not break everything inside the engine 
//camera will snap to nearest asteroid but the ship will move with the camera 
//pressing the button agian will move the camera back to where it was but 
void SpaceScene::OrbitCamera()
{
	//if the camera is not already fixed fix them or else un fix their position
	if (!inputComp->getFixedCam())
	{
		float MinDis = 999999999;
		GameObject* curMinAst;

		//find nearest asteroid
		playerPos = playerObj->getWorldPos();
		for each (GameObject* var in Asteroids)
		{
			if (var)
			{
				vec3 ast = var->getWorldPos();

				float TempLen = glm::distance(playerPos, ast);

				if (TempLen < MinDis && TempLen > 100)
				{
					MinDis = TempLen;
					curMinAst = var;
					cout << TempLen << endl;
				}
			}	
		}

		//set the flad to disable inout 
		inputComp->setFixedCam(true);
		curMinAst->setForceNoRender(true);
		playerObj->setForceNoRender(false);

		//set the new camera position
		input->setWorldPoint(curMinAst->getWorldPos());
		//set the ship to the new position
		playerObj->setPosition(playerPos - curMinAst->getWorldPos());
	}
	else
	{
		//set flags to allow movement 
		inputComp->setFixedCam(false);
		playerObj->setForceNoRender(true);
		for each (GameObject* var in Asteroids)
		{
			var->setForceNoRender(false);
		}

		//set the new camera position
		input->setWorldPoint(playerPos);
		//set the ship to the new position
		playerObj->setPosition(playerPos);
	}
	
}

GameObject *SpaceScene::getGameObject(string command)
{
	GameObject * tempGameObject;
	tempGameObject = worldObject->findChild(command);
	if (tempGameObject != NULL)
	{
		cout << "game object assigned " << tempGameObject->getName() << endl;
	}
	else
	{
		cout << "game object assigned NULL" << endl;
	}
	return tempGameObject;
}

GameObject *SpaceScene::getWorldObject()
{
	return worldObject;
}

Object *SpaceScene::getObject(string command)
{
	Object * tempObject;
	tempObject = objects[command];
	if (tempObject != NULL)
	{
		cout << "mesh assigned " << tempObject->getName() << endl;
	}
	else
	{
		cout << "mesh assigned NULL" << endl;
	}
	return tempObject;
}

Texture *SpaceScene::getTexture(string command)
{
	Texture * tempTexture;
	tempTexture = textures[command];
	if (tempTexture != NULL)
	{
		cout << "texture assigned " << tempTexture->getName() << endl;
	}
	else
	{
		cout << "texture assigned NULL" << endl;
	}
	return tempTexture;
}

Shader *SpaceScene::getShader(string command)
{
	Shader * tempShader;
	tempShader = shaders[command];
	if (tempShader != NULL)
	{
		cout << "shader assigned " << tempShader->getName() << endl;
	}
	else
	{
		cout << "shader assigned NULL" << endl;
	}
	return tempShader;
}

void SpaceScene::onKeyDown(SDL_Keycode key)
{
	//cout << "Key down " << key << endl;
	fustrum->updateCamera();
	switch (key)
	{
	case SDLK_p:
		if (debugMode)
		{
			cout << "debug mode off" << endl;
			debugMode = false;
		}
		else
		{
			cout << "debug mode on" << endl;
			debugMode = true;
		}
		input->setDebug(debugMode);
		break;
	case SDLK_ESCAPE:
		GameRunning = false;
		break;
	case SDLK_m:
		audio->toggleMute();
		break;
	case SDLK_e:
		OrbitCamera();
		break;
	default:
		break;
	}
}

void SpaceScene::onkeyUp(SDL_Keycode key)
{
	//cout << "Key up " << key << endl;
}

void SpaceScene::mouseMove(SDL_MouseMotionEvent motion)
{

}

void SpaceScene::joyButtonUp(SDL_ControllerButtonEvent button)
{
	switch (button.button)
	{
	case SDL_CONTROLLER_BUTTON_BACK:
		audio->toggleMute();
		break;
	case SDL_CONTROLLER_BUTTON_START:
		GameRunning = false;
		break;
	case SDL_CONTROLLER_BUTTON_Y:
		OrbitCamera();
		break;
	default:
		break;
	}
}

string SpaceScene::getName()
{
	return name;
}


