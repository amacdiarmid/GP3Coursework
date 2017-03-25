#ifndef _PLAYER_CONTROLLER_H
#define _PLAYER_CONTROLLER_H

#include "Common.h"

class PlayerController
{
protected:
	//vectors
	vec3 position;
	vec3 direction;
	vec3 upPoint;
	vec3 right;

	//matrices
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 MVPMatrix;

	bool debug = false;

	float horAngle = 3.14f;
	float VertAngle = 0;
	float mouseSpeed = 0.005f;
	float speed = 3;

	float fixedHeight = 0;
	vec3 center = position;
	float maxDistance = 95;

	float controllerSensitivity = 0.1;

public:
	PlayerController();
	~PlayerController();
	
	//Camera LookAt
	virtual void moveForward();
	virtual void moveBackward();
	virtual void strafeRight();
	virtual void strafeLeft();
	void mouseMovment(vec2 mousePos);

	void Update();

	vec3 getWorldPoint(){ return position; };
	vec3 getlookAtPoint(){ return direction; };
	vec3 getUpPoint(){ return upPoint; };
	vec3 getDirection(){ return direction - position; };

	float getRatio(){ return SCREEN_WIDTH / SCREEN_HEIGHT; };
	float getFOV(){ return FOV; };
	float getNearPlaneDis(){ return NEAR_CLIPPINGPLANE; };
	float getFarPlaneDis(){ return FAR_CLIPPINGPLANE; };
	float getNearHeight(){ return 2 * tan(FOV / 2) * NEAR_CLIPPINGPLANE; };
	float getNearWidth(){ return getNearHeight() * getRatio(); };
	float getFarHeight(){ return 2 * tan(FOV / 2) * FAR_CLIPPINGPLANE; };
	float getFarWidth(){ return getFarHeight() * getRatio(); };

	mat4 GetViewMatrix(){ return viewMatrix; };
	mat4 getProjMatrix(){ return projMatrix; };
	mat4 getMVPmatrix(){ return MVPMatrix; };

	void setWorldPoint(vec3 tempPos){ position = tempPos; direction = tempPos + vec3(0, 0, -10); };
	void setDebug(bool tempBool){debug = tempBool; if (!debug){ position.y = fixedHeight; }};
};



#endif