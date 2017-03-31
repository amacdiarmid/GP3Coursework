#include "AsteroidComponent.h"

AsteroidComponent::AsteroidComponent(GameObject * tempOwner, GameObject * TempPlayer)
{
	type = "asteroid component";
	owner = tempOwner;
	player = TempPlayer;
}

AsteroidComponent::~AsteroidComponent()
{

}

void AsteroidComponent::update(mat4 MVPMat)
{
	float TempLen = glm::distance(player->getWorldPos(), owner->getWorldPos());
	if (TempLen > destroyDistance) 
	{ 
		vec3 curPos = owner->getWorldPos();
		vec3 lookDir = owner->getCurScene()->getInput()->getlookAtPoint();
		vec3 newPos = curPos + vec3(lookDir.x * 1000, lookDir.y * 1000, lookDir.z * 1000);
		owner->setPosition(newPos);
	}
}
