#pragma once

#include <conio.h>
#include <gl_core_4_4.h>
#include <glfw-3.1.2\include\GLFW\glfw3.h>
#include <aie/Gizmos.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <list>

#include "Application.h"
#include "Camera.h"
#include "PhysicsObject.h"
#include "RigidBody.h"


class PhysicsApplication : public Application
{
public:
	static PhysicsApplication* sm_pInstance;
	PhysicsApplication() { sm_pInstance = this; }

	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();

	int day = 0;

	Camera camera;

	GLuint shaderID;

	std::list<PhysicsObject*> m_Objects;

	void resetScene();
	void MakeScene1();
	void MakeScene2();
	void MakeScene3();
	void MakeScene4();
	void MakeScene5();
	void MakeScene6();

private:
	vec2 m_v2MouseStart;
	vec2 m_v2MouseCurrent;
	bool m_bIsMouseDown;

	bool m_bShowGrid = true;

	int m_iActiveScene;
};
