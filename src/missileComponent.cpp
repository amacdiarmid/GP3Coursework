#include "missileComponent.h"

missileComponent::missileComponent(GameObject *tempOwner, AudioClip* TempExp, AudioClip* TempFire)
{
	type = "missile component";
	owner = tempOwner;
	explosion = TempExp;
	fire = TempFire;
	fire->Play();
}

missileComponent::~missileComponent()
{
}

void missileComponent::update(mat4 MVPMat)
{

}

bool missileComponent::collideWithObject(GameObject* target)
{
	cout << "destroy " + owner->getName() << endl;
	
	//play audio
	explosion->Play();
	fire->setLocation(owner->getWorldPos());
	fire->Stop();
	
	//playParticles
	
	//destroyTarget
	owner->setDestroy(true);
	target->setDestroy(true);

	return true;
}

