#pragma once
//#include "BulletPhys.h"
#include "Components\InputComponent.h"
#include "GamePlayerController.h"
#include "physicsComponent.h"
#include "missileComponent.h"
#include "BulletPhys.h"
#include "AudioClip.h"


class GameInputComponent :public InputComponent
{
public:
	GameInputComponent(GameObject *tempOwner);
	~GameInputComponent();

	void update(mat4 MVPMat) override;

	void onKeyDown(SDL_Keycode key) override;
	void onkeyUp(SDL_Keycode key) override;
	void mouseMove(SDL_MouseMotionEvent motion) override;
	void mouseDown(SDL_MouseButtonEvent button) override;
	void mouseUp(SDL_MouseButtonEvent button) override;
	void joyMove(SDL_ControllerAxisEvent motion) override;
	void joyButtonDown(SDL_ControllerButtonEvent button) override;
	void joyButtonUp(SDL_ControllerButtonEvent button) override;

	void assignBulPhys(BulletPhys* tempBul) { bulPhys = tempBul; };
	void assignMissile(Object* tempObject, Shader* tempShader, Texture* tempTexture, int shapeID, BulletPhys* TempPhys, AudioClip* TempExp, AudioClip* TempFire) { missileObject = tempObject, missileShader = tempShader, missileTexture = tempTexture, missileShapeID = shapeID, bulPhys = TempPhys, FireAudio = TempFire, ExpAudio = TempExp; };

	void FireMissile();
	void controllerToMouseMovement(SDL_ControllerAxisEvent motion);

	bool getFixedCam() { return fixedCam; }
	void setFixedCam(bool TempBool) { fixedCam = TempBool; }

private:
	GamePlayerController *playerCon;

	//missile 
	Object* missileObject;
	Shader* missileShader;
	Texture* missileTexture;
	int missileShapeID;
	AudioClip* ExpAudio;
	AudioClip* FireAudio;

	BulletPhys* bulPhys;

	//missile cound 
	int missileCount;
	int FireForce;
	float missileFireRate = 3000;
	bool CanFire = true;
	int TickOnFire;
	int missileMass = 1;

	//controller
	int StickDeadZone;
	float curX;
	float curY;
	float controllerSensitivity;

	//input bool
	bool moveForward;
	bool moveBack;
	bool moveLeft;
	bool moveRight;
	bool Fire;
	bool ControllerAimX;
	bool ControllerAimY;

	bool fixedCam = false;
};

