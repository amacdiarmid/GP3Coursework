#pragma once

#include "Components\BaseComponent.h"
#include "GameObject.h"

class AsteroidComponent : public Component
{
public:
	AsteroidComponent(GameObject *tempOwner, GameObject* TempPlayer);
	~AsteroidComponent();

	void update(mat4 MVPMat) override;

private:
	int destroyDistance = 2000;
	GameObject* player;
};
