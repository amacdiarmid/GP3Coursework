#include "missileComponent.h"

missileComponent::missileComponent(GameObject *tempOwner, AudioClip* TempExp, AudioClip* TempFire)
{
	type = "missile component";
	owner = tempOwner;
	explosion = TempExp;
	fire = TempFire;
	fire->Play(0.2, owner->getWorldPos());
	spawnTime = SDL_GetTicks();
	fire->SetOwner(owner);
}

missileComponent::~missileComponent()
{
}

void missileComponent::update(mat4 MVPMat)
{
	//if the missile has been in flight for more than the life span destroy it. 
	if (SDL_GetTicks() - spawnTime >= LifeSpawn)
	{
		owner->setDestroy(true);
	}
}

bool missileComponent::collideWithObject(GameObject* target)
{
	//if the objects reference are already not taged as destroy 
	if (target != owner->getParent() && !owner->getParent()->getDestroy())
	{
		cout << "destroy " + owner->getName() << endl;
	
		//play audio
		explosion->Play(0.1, owner->getWorldPos());
		fire->setLocation(owner->getWorldPos());
		fire->Stop();
	
		//playParticles
	
		//destroyTarget
		owner->setDestroy(true);
		target->setDestroy(true);

		return true;
	}
	
}

