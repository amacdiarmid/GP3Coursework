#ifndef _SpaceScene_H
#define _SpaceScene_H

#include "Scene.h"
#include "BulletPhys.h"
#include "physicsComponent.h"
#include "GameInputComponent.h"
#include "GamePlayerController.h"
#include "OpenAL.h"
#include "AudioClip.h"
#include "AIComponent.h"
#include "AsteroidComponent.h"


struct Light {
	glm::vec3 position;
	glm::vec3 intensities; //a.k.a. the color of the light
	float attenuation;
	float ambientCoefficient;
};

class SpaceScene : public IkeyboardListener, public Scene 
{
public:
	SpaceScene();
	SpaceScene(string name);
	~SpaceScene();

	void update() override;
	void render() override;
	void createScene() override;
	void UpdateLightPerspMVP();
	void destroyScene() override;
	void SceneLoop() override;

	void setUpShaders();
	void setUpMeshes();
	void setUpTextures();
	void setUpCubemap();
	void setUpAudio();
	void setUpLights();


	void spawnAsteroidClusters();
	void spawnAsteroids(vec3 location);
	void spawnShips();

	void OrbitCamera();

	//debug stuff
	GameObject *getGameObject(string command) override;
	GameObject *getWorldObject() override;
	Object *getObject(string command) override;
	Texture *getTexture(string command) override;
	Shader *getShader(string command) override;
	string getName() override;

	//input stuff
	void onKeyDown(SDL_Keycode key) override;
	void onkeyUp(SDL_Keycode key) override;
	void mouseMove(SDL_MouseMotionEvent motion) override;
	void mouseDown(SDL_MouseButtonEvent button) override {};
	void mouseUp(SDL_MouseButtonEvent button)override {};
	void joyMove(SDL_ControllerAxisEvent motion) override {};
	void joyButtonDown(SDL_ControllerButtonEvent button) override {};
	void joyButtonUp(SDL_ControllerButtonEvent button) override;

private:
	//ShadowMapping 1st pass
	GLuint depthTexture;
	GLuint depthMVPLocation;
	GLuint shadowFBO;
	mat4 depthMVP;
	GLuint quadTexture;
	GLuint quadVBO;
	//ShadowMapping 2nd pass
	Shader *shadowMapProgram;
	GLuint textureSamplerLocation;
	GLuint MVPLocation;
	GLuint viewLocation;
	GLuint modelLocation;
	GLuint depthBiasLocation;
	GLuint shadowMapLocation;
	GLuint lightLocation;
	vec3 lightInvDir = glm::vec3(-50, -50, 10); //TODO Calculate based on light direction
	mat4 depthBias;
	glm::mat4 ModelMatrix = glm::mat4(1.0);

	//PostProcessing
	GLuint FBOTexture;
	const int FRAME_BUFFER_WIDTH = 640;
	const int FRAME_BUFFER_HEIGHT = 480;
	GLuint FBODepthBuffer;
	GLuint frameBufferObject = 0;
	GLuint fullScreenVAO;
	GLuint fullScreenVBO;
	GLuint fullScreenShaderProgram;
	unsigned int lastTicks, currentTicks;
	float elapsedTime;
	float totalTime;
	vec2 screenResolution = vec2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	GLuint lightId;

	float materialShininess = 100;
	vec3 materialSpecularColor = vec3(0.3f, 0.3f, 0.3f);
	
	Light gLight;

	GLenum 	err = GL_NO_ERROR;
	GLuint textureLocationDiffuse;
	GLuint textureLocationSpecular;

	GLuint diffuseMap;

	GLuint materialShininessLoc;
	GLuint materialSPecularLoc;
	GLuint gLightPosLoc;
	GLuint gLightIntensitiesLoc;
	GLuint gLightAttenuationLoc;
	GLuint gLightAmbientCoeLoc;
	GLuint cameraPosLoc;

	vec3 lightDirection = vec3(0.0f, 0.0f, 1.0f);
	vec4 ambientMaterialColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	vec4 ambientLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 diffuseMaterialColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	vec4 diffuseLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 cameraPosition = vec3(15.0f, 50.0f, 50.0f);
	vec4 specularMaterialColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	GLfloat specularPower = 100;
	vec4 specularLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	CubeTexture* skyMaterial;

	GLfloat factor = 0.5f;
	GLboolean sepia = false;
	//woo we're the best

	//bullet physics 
	BulletPhys *bulPhys;

	//physic shape ID
	int AsteroidSphereID;
	int missileBoxID;
	int shipBoxID;

	int startingAsteroidCount = 100;
	int curAsteroidCount = 0;
	int TotalAsteroidMeshCount = 0;
	int TotalAsteroidTextureCount = 0;
	int asteroidMass = 100;
	int AsteroidSpawnBoxSize = 1000;

	int startingEnemyCount = 5;
	int curEnemyCount = 0;
	int enemyMass = 1000;

	//audio
	OpenAL* audio;
	map<string, ALuint> Sounds;
	AudioClip* BackgroundAudio;

	list<GameObject*> Asteroids;
	list<GameObject*> Ships;

	GameInputComponent *inputComp;
	vec3 playerPos;
	GameObject* playerObj;
	GameObject* levelNode;

	//super spooky 3D array for spawning clusters of asteroids
	//was initially used for infinite world generation but didnt work
	vector<vector<vector<vec3>>> positionNodes;

	int spawnTimerForEnenyWaves = 30000;
	int lastWaveSpawnTimer;
};

#endif