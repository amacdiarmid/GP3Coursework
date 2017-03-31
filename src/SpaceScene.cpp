
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
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniform1f(materialShininessLoc, materialShininess);
	glUniform3f(materialSPecularLoc, materialSpecularColor.x, materialSpecularColor.y, materialSpecularColor.z);
	glUniform3f(gLightPosLoc, gLight.position.x, gLight.position.y, gLight.position.z);
	glUniform3f(gLightIntensitiesLoc, gLight.intensities.x, gLight.intensities.y, gLight.intensities.z);
	glUniform1f(gLightAttenuationLoc, gLight.attenuation);
	glUniform1f(gLightAmbientCoeLoc, gLight.ambientCoefficient);
	glUniform3f(cameraPosLoc, input->getWorldPoint().x, input->getWorldPoint().y, input->getWorldPoint().z);
	CHECK_GL_ERROR();

	glUniform1i(textureSamplerLocation, 0);
	CHECK_GL_ERROR();

	glDepthMask(GL_TRUE);
	CHECK_GL_ERROR();

	worldObject->render(fustrum);

	CHECK_GL_ERROR();
}



void SpaceScene::update()
{
	GLenum err = GL_NO_ERROR;
	input->Update();

	bulPhys->updatePhysics();
	
	worldObject->update(input->getMVPmatrix());

	CHECK_GL_ERROR();

	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	UpdateLightPerspMVP();
	depthBias = biasMatrix * depthMVP;
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
	inputComp->assignMissile(objects["missile"], shaders["main"], textures["missile"], missileBoxID, bulPhys, Sounds["Explosion"], Sounds["RocketFire"]);
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

	materialShininess = 100;
	gLight.position = glm::vec3(0, 130, 0);
	gLight.intensities = glm::vec3(1.0f, 1.0f, 1.0f); //white
	gLight.attenuation = 1.0f;
	gLight.ambientCoefficient = 0.305f;

	GLuint shaderID = shaders["main"]->getShader();

	textureSamplerLocation = glGetUniformLocation(shaderID, "texture0");
	MVPLocation = glGetUniformLocation(shaderID, "MVP");
	materialShininessLoc = glGetUniformLocation(shaderID, "materialShininess");
	materialSPecularLoc = glGetUniformLocation(shaderID, "materialSpecularColor");
	gLightPosLoc = glGetUniformLocation(shaderID, "light.position");
	gLightIntensitiesLoc = glGetUniformLocation(shaderID, "light.intensities");
	gLightAttenuationLoc = glGetUniformLocation(shaderID, "light.attenuation");
	gLightAmbientCoeLoc = glGetUniformLocation(shaderID, "light.ambientCoefficient");
	cameraPosLoc = glGetUniformLocation(shaderID, "cameraPosition");
	//viewLocation = glGetUniformLocation(shaderID, "V");
	modelLocation = glGetUniformLocation(shaderID, "M");
	//depthBiasLocation = glGetUniformLocation(shaderID, "DepthBiasMVP");
	//shadowMapLocation = glGetUniformLocation(shaderID, "shadowMap");
	//lightLocation = glGetUniformLocation(shaderID, "LightInvDirection");
	//lightId = glGetUniformLocation(shaderID, "LightPosition_worldspace");

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
	for (auto const& x : Sounds)
	{
		delete x.second;
	}
	delete audio;
}

void SpaceScene::SceneLoop()
{
	windowLoop();
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

	audio = new OpenAL();
	
	AudioClip* tempAudio;

	//background 
	tempAudio = new AudioClip();
	tempAudio->CreateBuffer("/SpacialHarvest.wav");
	tempAudio->setLooping(TRUE);
	tempAudio->Play();
	Sounds.insert(pair<string, AudioClip*>("backgroundAudio", tempAudio));

	//ship 
	tempAudio = new AudioClip();
	tempAudio->CreateBuffer("/sergeniousShip.wav");
	tempAudio->setLooping(FALSE);
	Sounds.insert(pair<string, AudioClip*>("ShipAudio", tempAudio));

	//rocket 
	tempAudio = new AudioClip();
	tempAudio->CreateBuffer("/rocket-shoot.wav");
	tempAudio->setLooping(TRUE);
	Sounds.insert(pair<string, AudioClip*>("RocketFire", tempAudio));

	//explosion 
	tempAudio = new AudioClip();
	tempAudio->CreateBuffer("/explosion-01.wav");
	tempAudio->setLooping(FALSE);
	Sounds.insert(pair<string, AudioClip*>("Explosion", tempAudio));
}

void SpaceScene::spawnAsteroidClusters()
{


	//set up this 3D array 
	positionNodes.resize(3);
	for (int i = 0; i < 3; ++i) {
		positionNodes[i].resize(3);

		for (int j = 0; j < 3; ++j)
			positionNodes[i][j].resize(3);
	}

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

	for (int i = 0; i < startingAsteroidCount; i++)
	{
		string name = "ast" + to_string(curAsteroidCount);

		vec3 startingPos = vec3(rand() % AsteroidSpawnBoxSize - (AsteroidSpawnBoxSize/2), rand() % AsteroidSpawnBoxSize - (AsteroidSpawnBoxSize / 2), rand() % AsteroidSpawnBoxSize - (AsteroidSpawnBoxSize / 2));

		vec3 spawnPos = vec3(startingPos.x + location.x, startingPos.y + location.y, startingPos.z + location.z);

		levelNode->addChild(new GameObject(name, levelNode, objects["asteroid" + to_string(rand() % TotalAsteroidMeshCount + 1)], textures["AM" + to_string(rand() % TotalAsteroidTextureCount + 1)], shaders["main"]));	//creating object
		levelNode->getChild(name)->addComponent(new Renderer(levelNode->getChild(name)));	//adding render comp

		physicsComponent* phys = new physicsComponent(levelNode->getChild(name), bulPhys->CreatePhysBox(btVector3(spawnPos.x, spawnPos.y, spawnPos.z), asteroidMass, AsteroidSphereID, COL_AST), bulPhys);
		levelNode->getChild(name)->addComponent(phys); //adding physics comp
		levelNode->getChild(name)->setPosition(spawnPos);	//changing postiion
		levelNode->getChild(name)->setRotation(vec3(0, 0, 0));	//change rotaion
		levelNode->getChild(name)->setScale(vec3(1, 1, 1));	//change scele
		levelNode->getChild(name)->setForceRender(true);

		curAsteroidCount++;

		AsteroidComponent* AstComp = new AsteroidComponent(levelNode->getChild(name), playerObj);
		levelNode->getChild(name)->addComponent(AstComp);

		Asteroids.push_front(levelNode->getChild(name));
	}
}

void SpaceScene::spawnShips()
{
	Ships.clear();
	for (int i = 0; i < startingEnemyCount; i++)
	{
		string name = "ene" + to_string(curEnemyCount);

		vec3 startingPos = vec3(rand() % (2000 - 1000) - 1000, rand() % (2000 - 1000) - 1000, rand() % (2000 - 1000) - 1000);

		levelNode->addChild(new GameObject(name, levelNode, objects["ship"], textures["RedShip"], shaders["main"]));	//creating object
		levelNode->getChild(name)->addComponent(new Renderer(levelNode->getChild(name)));	//adding render comp

		btRigidBody* body = bulPhys->CreatePhysBox(btVector3(startingPos.x, startingPos.y, startingPos.z), enemyMass, AsteroidSphereID, COL_ENEMY);
		physicsComponent* phys = new physicsComponent(levelNode->getChild(name), body, bulPhys);
		levelNode->getChild(name)->addComponent(phys); //adding physics comp
		levelNode->getChild(name)->setPosition(startingPos);	//changing postiion
		levelNode->getChild(name)->setRotation(vec3(0, 0, 0));	//change rotaion
		levelNode->getChild(name)->setScale(vec3(0.2, 0.2, 0.2));	//change scele
		levelNode->getChild(name)->setForceRender(true);
		AIComponent* AIComp = new AIComponent(levelNode->getChild(name), phys, playerObj, &curEnemyCount);
		levelNode->getChild(name)->addComponent(AIComp);
		AIComp->assignMissile(objects["missile"], shaders["main"], textures["missile"], missileBoxID, bulPhys, Sounds["Explosion"], Sounds["RocketFire"]);

		curEnemyCount++;

		Ships.push_front(levelNode->getChild(name));
	}
}


//turns out it is very difficult to move the camera and not break everything inside the engine 
//camera will snap to nearest asteroid but the ship will move with the camera 
//pressing the button agian will move the camera back to where it was but 
void SpaceScene::OrbitCamera()
{
	if (!inputComp->getFixedCam())
	{
		float MinDis = 999999999;
		GameObject* curMinAst;

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


