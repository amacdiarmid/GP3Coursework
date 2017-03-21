#pragma once

#include "Common.h"
#include "Components\BaseComponent.h"
#include "Object.h"
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
#include "SpaceScene.h"

class missileComponent : public Component
{
public:
	missileComponent(GameObject *tempOwner, btRigidBody* tempRigidBody, btDiscreteDynamicsWorld* TdynamicsWorld);
	~missileComponent();

	void update(mat4 MVPMat) override;

	bool collideWithObject(GameObject* target);

private:
	btRigidBody* rigidBody;
	btDiscreteDynamicsWorld* dynamicsWorld;
};