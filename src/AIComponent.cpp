#include "AIComponent.h"

AIComponent::AIComponent(GameObject * tempOwner, physicsComponent * TempPhys, GameObject* TempTarget, int* totalShips)
{
	type = "AI component";
	owner = tempOwner;
	phys = TempPhys->getRB();
	target = TempTarget;
	refToTotalShips = totalShips;
}

AIComponent::~AIComponent()
{

}

void AIComponent::update(mat4 MVPMat)
{
	//get the location of the ship and ser the direction it needs to move in
	vec3 pos = owner->getWorldPos();
	vec3 dir = (target->getWorldPos() - owner->getWorldPos()) * Speed;

	
	//get the distance and if the distance is less than the range fire missile if it can
	float dis = distance(target->getWorldPos(), owner->getWorldPos());
	if (dis < fireMissileDistance)
	{
		if (CanFire)
		{
			CanFire = false;
			TickOnFire = SDL_GetTicks();
			FireMissile();
		}
	}
	else
	{
		//apply shi force
		phys->applyForce(btVector3(dir.x, dir.y, dir.z), btVector3(pos.x, pos.y, pos.z));
	}
	//see if the ship can fire agian
	if (SDL_GetTicks() - TickOnFire >= missileFireRate && !CanFire)
	{
		CanFire = true;
	}
}

void AIComponent::FireMissile()
{
	GameObject* tempMissile;
	string TempName = owner->getName() + "missile" + to_string(missileCount);


	tempMissile = new GameObject(TempName, owner, missileObject, missileTexture, missileShader);
	owner->addChild(tempMissile);	//creating object

	missileCount++;
	btVector3 FirePosition = btVector3(owner->getWorldPos().x, owner->getWorldPos().y, owner->getWorldPos().z);
	//cout << "position " << owner->getWorldPos().x << " " << owner->getWorldPos().y << " " << owner->getWorldPos().z << endl;
	btRigidBody* missile1 = bulPhys->CreatePhysBox(FirePosition, missileMass, missileShapeID, COL_ENEMYMISSILE);

	//TODO set the firce direction forward


	vec3 vec3Force = (target->getWorldPos() - owner->getWorldPos()) * (float)FireForce;
	btVector3 btForce = btVector3(vec3Force.x, vec3Force.y, vec3Force.z);
	missile1->applyForce(btForce, FirePosition);
	missile1->setAngularVelocity(btForce);

	tempMissile->addComponent(new Renderer(owner->getChild(TempName)));	//adding render comp

	physicsComponent* phys = new physicsComponent(owner->getChild(TempName), missile1, bulPhys);

	tempMissile->addComponent(phys); //adding physics comp
	tempMissile->addComponent(new missileComponent(owner->getChild(TempName), ExpAudio, FireAudio));	//adding the missile comp with all the missile related code
	tempMissile->setPosition(vec3(0, 0, 0));	//changing postiion
	tempMissile->setRotation(vec3(0, 0, 0));	//change rotaion
	tempMissile->setScale(vec3(1, 1, 1));	//change scele
	tempMissile->setForceRender(true);
}
