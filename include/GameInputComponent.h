#pragma once
//#include "BulletPhys.h"
#include "Components\InputComponent.h"
#include "GamePlayerController.h"
#include "physicsComponent.h"
#include "missileComponent.h"


class GameInputComponent :public InputComponent
{
public:
	GameInputComponent(GameObject *tempOwner);
	~GameInputComponent();

	void onKeyDown(SDL_Keycode key) override;
	void onkeyUp(SDL_Keycode key) override;
	void mouseMove(SDL_MouseMotionEvent motion) override;
	void mouseDown(SDL_MouseButtonEvent button) override;
	void mouseUp(SDL_MouseButtonEvent button) override;

	void assignBulPhys(BulletPhys* tempBul) { bulPhys = tempBul; };
	void assignMissile(Object* tempObject, Shader* tempShader, Texture* tempTexture, int shapeID, BulletPhys* TempPhys) { missileObject = tempObject, missileShader = tempShader, missileTexture = tempTexture, missileShapeID = shapeID, bulPhys = TempPhys; };

private:
	GamePlayerController *playerCon;

	//missile 
	Object* missileObject;
	Shader* missileShader;
	Texture* missileTexture;
	int missileShapeID;

	BulletPhys* bulPhys;

	//missile cound 
	int missileCount;
};

