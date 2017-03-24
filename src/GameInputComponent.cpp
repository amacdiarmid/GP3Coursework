#include "GameInputComponent.h"



GameInputComponent::GameInputComponent(GameObject *tempOwner)
{
	type = "Game input component";
	owner = tempOwner;
	keyboardListeners.push_back(this);
	playerCon = (GamePlayerController*)owner->getInput();
	missileCount = 0;
	FireForce = 5000;
}


GameInputComponent::~GameInputComponent()
{
}

void GameInputComponent::onKeyDown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_w:
		owner->getInput()->moveForward();
		break;
	case SDLK_s:
		owner->getInput()->moveBackward();
		break;
	case SDLK_a:
		owner->getInput()->strafeLeft();
		break;
	case SDLK_d:
		owner->getInput()->strafeRight();
		break;
	}
	owner->setPosition(owner->getInput()->getWorldPoint());
}

void GameInputComponent::onkeyUp(SDL_Keycode key)
{
}

void GameInputComponent::mouseMove(SDL_MouseMotionEvent motion)
{
	owner->getInput()->mouseMovment(vec2(motion.x, motion.y));
	SDL_WarpMouseInWindow(getWin(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void GameInputComponent::mouseDown(SDL_MouseButtonEvent button)
{
	if (button.button == SDL_BUTTON_LEFT)
	{
		playerCon->FirePrimWeapon();

		GameObject* tempMissile;
		string TempName = "missile" + to_string(missileCount);


		tempMissile = new GameObject(TempName, owner, missileObject, missileTexture, missileShader);
		owner->addChild(tempMissile);	//creating object
		
		missileCount++;
		btVector3 FirePosition = btVector3(owner->getWorldPos().x, owner->getWorldPos().y, owner->getWorldPos().z);
		//cout << "position " << owner->getWorldPos().x << " " << owner->getWorldPos().y << " " << owner->getWorldPos().z << endl;
		btRigidBody* missile1 = bulPhys->CreatePhysBox(FirePosition, 1, missileShapeID);
		missile1->setCollisionFlags(missile1->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);



		//TODO set the firce direction forward

		btVector3 btForce = btVector3(playerCon->getlookAtPoint().x * FireForce, playerCon->getlookAtPoint().y * FireForce, playerCon->getlookAtPoint().z * FireForce);
		missile1->applyForce(btForce, FirePosition);
		missile1->setAngularVelocity(btForce);
		missile1->setCollisionFlags(missile1->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

		tempMissile->addComponent(new Renderer(owner->getChild(TempName)));	//adding render comp

		physicsComponent* phys = new physicsComponent(owner->getChild(TempName), missile1, bulPhys);

		tempMissile->addComponent(phys); //adding physics comp
		tempMissile->addComponent(new missileComponent(owner->getChild(TempName), ExpAudio, FireAudio));	//adding the missile comp with all the missile related code
		tempMissile->setPosition(vec3(0,0,0));	//changing postiion
		tempMissile->setRotation(playerCon->getlookAtPoint());	//change rotaion
		tempMissile->setScale(vec3(1, 1, 1));	//change scele
		tempMissile->setForceRender(true);
	}
}

void GameInputComponent::mouseUp(SDL_MouseButtonEvent button)
{
}
