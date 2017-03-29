#include "GameInputComponent.h"



GameInputComponent::GameInputComponent(GameObject *tempOwner)
{
	type = "Game input component";
	owner = tempOwner;
	keyboardListeners.push_back(this);
	playerCon = (GamePlayerController*)owner->getInput();
	missileCount = 0;
	FireForce = 5000;
	moveForward = false;
	moveBack = false;
	moveLeft = false;
	moveRight = false;
	Fire = false;
	ControllerAimX = false;
	ControllerAimY = false;
	StickDeadZone = 10000;
	curX = 0;
	curY = 0;
	controllerSensitivity = 10;
}


GameInputComponent::~GameInputComponent()
{
}

void GameInputComponent::update(mat4 MVPMat)
{
	if (!fixedCam)
	{
		if (moveForward)
		{
			owner->getInput()->moveForward();
		}
		if (moveBack)
		{
			owner->getInput()->moveBackward();
		}
		if (moveLeft)
		{
			owner->getInput()->strafeLeft();
		}
		if (moveRight)
		{
			owner->getInput()->strafeRight();
		}
		if (Fire && CanFire)
		{
			CanFire = false;
			TickOnFire = SDL_GetTicks();
			FireMissile();
		}
	}
	
	if (SDL_GetTicks() - TickOnFire >= missileFireRate)
	{
		CanFire = true;
	}


	owner->setPosition(owner->getInput()->getWorldPoint());
	
	if (ControllerAimX || ControllerAimY)
	{
		float TempX = SCREEN_WIDTH / 2;
		float TempY = SCREEN_HEIGHT / 2;

		TempX = TempX + (curX * controllerSensitivity);
		TempY = TempY + (curY * controllerSensitivity);

		cout << "X " << TempX << " y " << TempY << endl;

		owner->getInput()->mouseMovment(vec2(TempX, TempY));
	}
	
}

void GameInputComponent::onKeyDown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_w:
		moveForward = true;
		break;
	case SDLK_s:
		moveBack = true;
		break;
	case SDLK_a:
		moveLeft = true;
		break;
	case SDLK_d:
		moveRight = true;
		break;
	}
}

void GameInputComponent::onkeyUp(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_w:
		moveForward = false;
		break;
	case SDLK_s:
		moveBack = false;
		break;
	case SDLK_a:
		moveLeft = false;
		break;
	case SDLK_d:
		moveRight = false;
		break;
	}
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
		Fire = true;
	}
}

void GameInputComponent::mouseUp(SDL_MouseButtonEvent button)
{
	if (button.button == SDL_BUTTON_LEFT)
	{
		Fire = false;
	}
}

void GameInputComponent::joyMove(SDL_ControllerAxisEvent motion)
{
	switch (motion.axis)
	{
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		if (motion.value > 4) Fire = true;
		else Fire = false;
		break;
	case SDL_CONTROLLER_AXIS_LEFTX:
		if (motion.value > StickDeadZone) moveRight = true;
		else if (motion.value < -StickDeadZone) moveLeft = true;
		else
		{
			moveRight = false;
			moveLeft = false;
		}
		//cout << "left X value " << to_string(motion.value) << endl;
		break;
	case SDL_CONTROLLER_AXIS_LEFTY:
		if (motion.value < -StickDeadZone) moveForward = true;
		else if (motion.value > StickDeadZone) moveBack = true;
		else
		{
			moveForward = false;
			moveBack = false;
		}
		//cout << "left Y value " << to_string(motion.value) << endl;
		break;
	case SDL_CONTROLLER_AXIS_RIGHTX:
		if (motion.value > StickDeadZone || motion.value < -StickDeadZone)
		{
			curX = motion.value / 32767;
			ControllerAimX = true;
		}
		else
		{
			ControllerAimX = false;
		}
		//cout << "right X value " << to_string(curX) << endl;
		break;
	case SDL_CONTROLLER_AXIS_RIGHTY:
		if (motion.value > StickDeadZone || motion.value < -StickDeadZone)
		{
			curY = motion.value / 32767;
			ControllerAimY = true;
		}
		else
		{
			ControllerAimY = false;
		}
		//cout << "right Y value " << to_string(curY) << endl;
		break;
	default:
		break;
	}
}

void GameInputComponent::joyButtonDown(SDL_ControllerButtonEvent button)
{
	switch (button.button)
	{
	case SDL_CONTROLLER_BUTTON_DPAD_UP:
		moveForward = true;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		moveBack = true;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		moveLeft = true;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		moveRight = true;
		break;
	}
}

void GameInputComponent::joyButtonUp(SDL_ControllerButtonEvent button)
{
	switch (button.button)
	{
	case SDL_CONTROLLER_BUTTON_DPAD_UP:
		moveForward = false;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		moveBack = false;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		moveLeft = false;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		moveRight = false;
		break;
	}
}

void GameInputComponent::FireMissile()
{

	GameObject* tempMissile;
	string TempName = "missile" + to_string(missileCount);


	tempMissile = new GameObject(TempName, owner, missileObject, missileTexture, missileShader);
	owner->addChild(tempMissile);	//creating object

	missileCount++;
	btVector3 FirePosition = btVector3(owner->getWorldPos().x, owner->getWorldPos().y, owner->getWorldPos().z);
	//cout << "position " << owner->getWorldPos().x << " " << owner->getWorldPos().y << " " << owner->getWorldPos().z << endl;
	btRigidBody* missile1 = bulPhys->CreatePhysBox(FirePosition, missileMass, missileShapeID, COL_PLAYERMISSILE);

	//TODO set the firce direction forward

	btVector3 btForce = btVector3(playerCon->getlookAtPoint().x * FireForce, playerCon->getlookAtPoint().y * FireForce, playerCon->getlookAtPoint().z * FireForce);
	missile1->applyForce(btForce, FirePosition);
	missile1->setAngularVelocity(btForce);
	tempMissile->addComponent(new Renderer(owner->getChild(TempName)));	//adding render comp

	physicsComponent* phys = new physicsComponent(owner->getChild(TempName), missile1, bulPhys);

	tempMissile->addComponent(phys); //adding physics comp
	tempMissile->addComponent(new missileComponent(owner->getChild(TempName), ExpAudio, FireAudio));	//adding the missile comp with all the missile related code
	tempMissile->setPosition(vec3(0, 0, 0));	//changing postiion
	tempMissile->setRotation(playerCon->getlookAtPoint());	//change rotaion
	tempMissile->setScale(vec3(1, 1, 1));	//change scele
	tempMissile->setForceRender(true);
}

void GameInputComponent::controllerToMouseMovement(SDL_ControllerAxisEvent motion)
{

}

