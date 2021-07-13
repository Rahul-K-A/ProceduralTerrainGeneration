#pragma once
#include <iostream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW/glfw3.h>
#include <glm\gtc\matrix_transform.hpp>


class Camera
{
public:
	//Constructor
	Camera(glm::vec3 InitialPosition, GLfloat InitialYaw, GLfloat InitialPitch, GLfloat InitialMovementSpeed, GLfloat InitialTurnSpeed);
	//Updates position of camera every frame
	void Update();
	//Takes array of all keys from a MyWindow instance and updates the camera position every frame
	void KeyControl(bool* KeyArray, double DeltaTime);
	//Takes array of all keys from a MyWindow instance and updates the camera orientation every frame
	void MouseControl(double MouseX, double MouseY, double DeltaTime);
	//Calculates the view from the camera for the given position and orientation
	glm::mat4 CalculateCameraMatrix();
	//Returns position of camera in world space
	glm::vec3 GetCameraPosition();
	//Returns direction camera is facing
	glm::vec3 GetCameraDirection();
	//Returns boolean for spotlight attached to camera
	bool GetFlashLightStatus();

private:
	//Postion vector
	glm::vec3 Position;
	//Vector in the direction to which the ccamera points
	glm::vec3 Front;
	//Vector perpendicular to  the camera
	glm::vec3 Up;
	//Vector which points to the right of the camera
	glm::vec3 Right;
	//Universal Y axis of Laptop  (Remember coordinate system changes for displays so our Z axis is actually Y axis of the display)
	const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//Rotation about Y axis of the display
	GLdouble Yaw;
	//Rotation about Z axis of the display axis
	GLdouble Pitch;
	//Movement speed rate
	GLdouble MovementSpeed;
	//Turn speed rate
	GLdouble TurnSpeed;
	//Spot Light Status boolean
	bool IsSpotLightOn;


};
