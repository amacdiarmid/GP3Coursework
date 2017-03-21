#include "missileComponent.h"

missileComponent::missileComponent(GameObject *tempOwner, btRigidBody* tempRigidBody, btDiscreteDynamicsWorld* TdynamicsWorld)
{
	owner = tempOwner;
	rigidBody = tempRigidBody;
	dynamicsWorld = TdynamicsWorld;
}

missileComponent::~missileComponent()
{
}

void missileComponent::update(mat4 MVPMat)
{
	SpaceScene* curScene = (SpaceScene*)owner->getCurScene();
	std::list<physicsComponent*> physList = curScene->getPhysComps();

	//rigidBody->

	for each (physicsComponent* i in physList)
	{
		if (rigidBody->checkCollideWith(i->getRB()))
		{
			collideWithObject(i->getOwner());
		}
	}
	
}

bool missileComponent::collideWithObject(GameObject* target)
{
	//playAudio
	//playParticles
	//destroyobject
	//destroyTarget
	cout << "destroy " + owner->getName() << endl;
	return true;
}

