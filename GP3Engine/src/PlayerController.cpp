#include "PlayerController.h"

PlayerController::PlayerController()
{
	mouseMovment(vec2(0, 0));
}

void PlayerController::moveForward()
{
	position += direction * speed;
	if (!debug)
	{ 
		position.y = fixedHeight; 
		if (glm::distance(position,center) > maxDistance)
		{
			position -= direction * speed;
		}
		//cout << "distance " << glm::distance(position, center) << endl;
	}
	//cout << "w key " << endl;
}

void PlayerController::moveBackward()
{
	position -= direction * speed;
	if (!debug)
	{
		position.y = fixedHeight;
		if (glm::distance(position, center) > maxDistance)
		{
			position += direction * speed;
		}
		//cout << "distance " << glm::distance(position, center) << endl;
	}	//cout << "s key " << endl;
}

void PlayerController::strafeRight()
{
	position += right * speed;
	if (!debug)
	{
		position.y = fixedHeight;
		if (glm::distance(position, center) > maxDistance)
		{
			position -= right * speed;
		}
		//cout << "distance " << glm::distance(position, center) << endl;
	}	//cout << "s key " << endl;
}

void PlayerController::strafeLeft()
{
	position -= right * speed;
	if (!debug)
	{
		position.y = fixedHeight;
		if (glm::distance(position, center) > maxDistance)
		{
			position += right * speed;
		}
		//cout << "distance " << glm::distance(position, center) << endl;
	}	//cout << "s key " << endl;
}

void PlayerController::mouseMovment(vec2 mousePos)
{
	//cout << "hor angle " << mousePos.x << " vert angle " << mousePos.y << endl;

	horAngle += mouseSpeed * float(SCREEN_WIDTH / 2 - mousePos.x);
	if (degrees(horAngle) > 360.0f) { horAngle = radians(0.0f); }
	else if (degrees(horAngle) < 0.0f){ horAngle = radians(360.0f); }
	VertAngle += mouseSpeed * float(SCREEN_HEIGHT / 2 - mousePos.y);
	if (degrees(VertAngle) >= 90.0f) { VertAngle = radians(89.9f); }
	else if (degrees(VertAngle) <= -90.0f) { VertAngle = radians (- 89.9f);}
	//cout << "hor angle " << degrees(horAngle) << " vert angle " << degrees(VertAngle) << endl;

	direction = vec3(cos(VertAngle)*sin(horAngle), sin(VertAngle), cos(VertAngle)*cos(horAngle));

	right = vec3(sin(horAngle - 3.14 / 2), 0, cos(horAngle - 3.14 / 2));
	upPoint = vec3(0, 1, 0);
	//upPoint = cross(direction, right);
}

void PlayerController::Update()
{
	projMatrix = perspective(FOV, getRatio(), NEAR_CLIPPINGPLANE, FAR_CLIPPINGPLANE);
	viewMatrix = lookAt(position, position + direction, upPoint);
	MVPMatrix = projMatrix*viewMatrix;
	//std::cout << (float)position.x << " " << (float)position.y << " " << (float)position.z << "\n" << (float)direction.x << " " << (float)direction.y << " " << (float)direction.z << " " << endl;
}
