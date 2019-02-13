#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 position = { 0,0,0 };
	glm::vec3 viewDirection = { 0.f, 0.f, -1.f };
	glm::vec3 upPositipon = { 0, 1, 0 };

public:
	glm::vec2 oldMousePosition;
	float rSpeed = 0.5f;
	float mSpeed = 0.05f;
	
	bool flyCamera = 1;

	struct
	{
		float angle; ///angle of the view projecton in angles not radians
		float *width; /// pointer to the idth of the screen
		float *height; /// pointer to the height of the screen
		float closePlane;
		float farPlane;
	}projectionData;

	Camera();
	Camera(float angle, float *width, float *height, float closePlane, float farPlane);


	glm::mat4 getObjectToWorld(); ///gets the object to world for the camera
	glm::mat4 getProjectionViewMatrix(); /// gets the full view transform

	void mouseUpdate(const glm::vec2 &pos);
	void move(const glm::vec3 &move);
	void moveUp(float speed);
	void moveDown(float speed);
	void moveLeft(float speed);
	void moveRight(float speed);
	void moveFront(float speed);
	void moveBack(float speed);


};
