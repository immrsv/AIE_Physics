#include "PhysicsApplication.h"

#include <iostream>

#include "Plane.h"
#include "Circle.h"
#include "Box.h"
//#include "Spring.h"

using namespace glm;

PhysicsApplication* PhysicsApplication::sm_pInstance = 0;

bool PhysicsApplication::startup()
{
	glfwInit();

	window = glfwCreateWindow(1280, 720,
		"Computer Graphics",
		nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);


	glClearColor(0.0f, 0.0f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	Gizmos::create(65535u, 65535u, 65535u, 65535u);

	camera.radius = 1;
	
	resetScene();

	return true;
}

void PhysicsApplication::shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);


	glfwTerminate();
}

float divisor = 60.0f;

bool PhysicsApplication::update()
{
	camera.update(window);

	if (glfwGetKey(window, GLFW_KEY_UP)) {
		divisor = std::fmaxf(30.0f, divisor * 0.95f);
		std::cout << "Time Step @ 1.0 / " << divisor << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		divisor = std::fminf(1000.0f, divisor * 1.05f);
		std::cout << "Time Step @ 1.0 / " << divisor << std::endl;
	}

	float dt = 1.0f / divisor;
	
	if (glfwGetKey(window, GLFW_KEY_1))
		MakeScene1();
	if (glfwGetKey(window, GLFW_KEY_2))
		MakeScene2();
	if (glfwGetKey(window, GLFW_KEY_3))
		MakeScene3();
	if (glfwGetKey(window, GLFW_KEY_4))
		MakeScene4();
	if (glfwGetKey(window, GLFW_KEY_5))
		MakeScene5();
	if (glfwGetKey(window, GLFW_KEY_6))
		MakeScene6();

	if (glfwGetKey(window, GLFW_KEY_G))
		m_bShowGrid = !m_bShowGrid;

	if (glfwGetKey(window, GLFW_KEY_R))
		resetScene();

	// check for mousedown
	bool mouseDown = glfwGetMouseButton(window, 0);
	double x0, y0;

	glfwGetCursorPos(window, &x0, &y0);

	mat4 view = camera.getView();
	mat4 projection = camera.getProjection();

	glm::vec3 windowCoordinates = glm::vec3(x0, y0, 0);
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, 1280, 720);
	glm::vec3 worldCoordinates = glm::unProject(windowCoordinates, view, projection, viewport);

	m_v2MouseCurrent = vec2(worldCoordinates[0] * camera.getDistance(), worldCoordinates[1] * (-camera.getDistance()));

	if (mouseDown != m_bIsMouseDown)
	{
		if (mouseDown)
		{
			m_v2MouseStart = m_v2MouseCurrent;
		}
		else {
			for (auto obj : m_Objects) {

				if (obj->isInside(m_v2MouseStart)) {
					RigidBody* rb = static_cast<RigidBody*>(obj);
					rb->m_bIsAwake = true;
					vec2 posn = m_v2MouseStart - rb->m_v2Position;
					if (glm::length(posn) < 0.25f) posn = vec2();

					rb->applyForce(2.0f*(m_v2MouseStart - m_v2MouseCurrent), posn);
				}
			}
		}
		m_bIsMouseDown = mouseDown;
	}

	bool objectiveComplete = false;
	// Update objects
	for (auto obj : m_Objects){

		RigidBody* rb = dynamic_cast<RigidBody*>(obj);

		
		// Collision
		for (auto candidate : m_Objects ){
			if (candidate == obj) break; // Don't check self!
			if (obj->checkCollision(candidate)) {
				if ((obj->tag == 1 && candidate->tag == 2) || (obj->tag == 2 && candidate->tag == 1)) // Player hit objective
					objectiveComplete = true;
			}
		}

		// Motion
		obj->update(dt);

		// Gravity
		if (rb) rb->applyForce(dt * rb->m_fMass * PhysicsObject::sm_v2Gravity);
	}

	if (objectiveComplete) {
		m_iActiveScene++;
		resetScene();
	}

	return (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
}

void PhysicsApplication::draw()
{
	mat4 view = camera.getView();
	mat4 projection = camera.getProjection();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Gizmos::clear();

	Gizmos::addTransform(glm::mat4(1));
	vec4 orange(1, 0.7f, 0.2f, 0.5f);
	vec4 red(1, 0, 0, 0.5f);
	vec4 white(1, 1, 1, 0.5f);
	vec4 grey(0.5f);
	vec4 black(0, 0, 0, 1);

	if (m_bIsMouseDown) {
		Gizmos::add2DLine(m_v2MouseStart, m_v2MouseCurrent, white);
	}

	if (m_bShowGrid) {
		for (int i = 0; i < 21; ++i) {
			Gizmos::add2DLine(vec2(-10 + i, 10), vec2(-10 + i, -10), i == 10 ? orange : grey);
			Gizmos::add2DLine(vec2(10, -10 + i), vec2(-10, -10 + i), i == 10 ? orange : grey);
		}
	}

	for (auto obj : m_Objects) {
		obj->draw();
	}

	Gizmos::draw2D(projection * view);

	glfwSwapBuffers(window);
	glfwPollEvents();

	day++;
}


void PhysicsApplication::resetScene() {
	switch (m_iActiveScene)
	{
	case 1: default: MakeScene1(); break;
	case 2: MakeScene2(); break;
	case 3: MakeScene3(); break;
	case 4: MakeScene4(); break;
	case 5: MakeScene5(); break;
	case 6:	MakeScene6(); break;
	}
}

void PhysicsApplication::MakeScene1() {
	m_iActiveScene = 1;

	PhysicsObject::sm_v2Gravity = glm::vec2(0, 0);
	PhysicsObject::sm_fCoeffRestitution = 1.0f;

	m_Objects.clear();

	PhysicsObject* obj;

	m_Objects.push_back(obj = new Box(vec2(-9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2( 9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(7, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-1,-7), vec2(0, 0), vec2(8.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0, 7), vec2(0, 0), vec2(9.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(8, -7), vec2(0, 0), vec2(1, 0.05)));
	obj->m_v4Color = vec4(0, 1, 0, 1);
	((RigidBody*)obj)->m_bIsKinematic = true;
	((RigidBody*)obj)->tag = 2; // Objective


	m_Objects.push_back(obj = new Circle(vec2(-7.5, -5.5), vec2(), 0.5f));
	obj->m_v4Color = vec4(0.7, 0.7, 0.7, 1);
	((RigidBody*)obj)->m_bIsAwake = false;
	((RigidBody*)obj)->tag = 1; // Player

}

void PhysicsApplication::MakeScene2() {
	m_iActiveScene = 2;

	PhysicsObject::sm_v2Gravity = glm::vec2(0, -2);
	PhysicsObject::sm_fCoeffRestitution = 0.8f;

	m_Objects.clear();

	PhysicsObject* obj;

	m_Objects.push_back(obj = new Box(vec2(-9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-1, -7), vec2(0, 0), vec2(8.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(7, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0, 7), vec2(0, 0), vec2(9.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(8, -7), vec2(0, 0), vec2(1, 0.05)));
	obj->m_v4Color = vec4(0, 1, 0, 1);
	((RigidBody*)obj)->m_bIsKinematic = true;
	((RigidBody*)obj)->tag = 2; // Objective

	m_Objects.push_back(obj = new Circle(vec2(-7.5, -5.5), vec2(), 0.5f));
	obj->m_v4Color = vec4(0.7, 0.7, 0.7, 1);
	((RigidBody*)obj)->m_bIsAwake = false;
	((RigidBody*)obj)->tag = 1; // Player

	m_Objects.push_back(obj = new Box(vec2(-4, 2), vec2(0, 0), vec2(0.5)));
	obj->m_fRotation = 3.1415f / 4.0f;
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(4, 2), vec2(0, 0), vec2(0.5)));
	obj->m_fRotation = 3.1415f / 3.0f;
	((RigidBody*)obj)->m_bIsKinematic = true;
}

void PhysicsApplication::MakeScene3() {
	m_iActiveScene = 3;

	PhysicsObject::sm_v2Gravity = glm::vec2(0, -2);
	PhysicsObject::sm_fCoeffRestitution = 1.1f;

	m_Objects.clear();

	PhysicsObject* obj;

	m_Objects.push_back(obj = new Box(vec2(-9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(9, 4.5), vec2(0, 0), vec2(0.05, 2.5)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(9, -4.5), vec2(0, 0), vec2(0.05, 2.5)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(4.5, 0), vec2(0, 0), vec2(0.05, 2.5)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0, -7), vec2(0, 0), vec2(9.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0, 7), vec2(0, 0), vec2(9.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(10, 0), vec2(0, 0), vec2(0.05, 2)));
	obj->m_v4Color = vec4(0, 1, 0, 1);
	((RigidBody*)obj)->m_bIsKinematic = true;
	((RigidBody*)obj)->tag = 2; // Objective

	m_Objects.push_back(obj = new Circle(vec2(-7.5, -5.5), vec2(), 0.5f));
	obj->m_v4Color = vec4(0.7, 0.7, 0.7, 1);
	((RigidBody*)obj)->m_bIsAwake = false;
	((RigidBody*)obj)->tag = 1; // Player
}


void PhysicsApplication::MakeScene4() {
	m_iActiveScene = 4;

	PhysicsObject::sm_v2Gravity = glm::vec2(0, -2);
	PhysicsObject::sm_fCoeffRestitution = 0.8f;

	m_Objects.clear();

	PhysicsObject* obj;

	m_Objects.push_back(obj = new Box(vec2(-9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(7, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-6, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0.5, -5), vec2(0, 0), vec2(6.5, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-7.5, -7), vec2(0, 0), vec2(1.5, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0, 7), vec2(0, 0), vec2(9.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(8, -7), vec2(0, 0), vec2(1, 0.05)));
	obj->m_v4Color = vec4(0, 1, 0, 1);
	((RigidBody*)obj)->m_bIsKinematic = true;
	((RigidBody*)obj)->tag = 2; // Objective

	m_Objects.push_back(obj = new Circle(vec2(-7.5, -5.5), vec2(), 0.5f));
	obj->m_v4Color = vec4(0.7, 0.7, 0.7, 1);
	((RigidBody*)obj)->m_bIsAwake = false;
	((RigidBody*)obj)->tag = 1; // Player

}

void PhysicsApplication::MakeScene5() {
	m_iActiveScene = 5;

	PhysicsObject::sm_v2Gravity = glm::vec2(0, -2);
	PhysicsObject::sm_fCoeffRestitution = 0.8f;

	m_Objects.clear();

	PhysicsObject* obj;

	m_Objects.push_back(obj = new Box(vec2(-9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(7, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-6, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0.5, -5), vec2(0, 0), vec2(6.5, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-7.5, -7), vec2(0, 0), vec2(1.5, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0, 7), vec2(0, 0), vec2(9.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(8, -7), vec2(0, 0), vec2(1, 0.05)));
	obj->m_v4Color = vec4(0, 1, 0, 1);
	((RigidBody*)obj)->m_bIsKinematic = true;
	((RigidBody*)obj)->tag = 2; // Objective

	m_Objects.push_back(obj = new Circle(vec2(-7.5, -5.5), vec2(), 0.5f));
	obj->m_v4Color = vec4(0.7, 0.7, 0.7, 1);
	((RigidBody*)obj)->m_bIsAwake = false;
	((RigidBody*)obj)->tag = 1; // Player

	m_Objects.push_back(obj = new Circle(vec2(), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(1,0), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(-1,0), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(0,1), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(0,-1), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;

}


void PhysicsApplication::MakeScene6() {
	m_iActiveScene = 6;

	PhysicsObject::sm_v2Gravity = glm::vec2(0, -2);
	PhysicsObject::sm_fCoeffRestitution = 0.8f;

	m_Objects.clear();

	PhysicsObject* obj;

	m_Objects.push_back(obj = new Box(vec2(-9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(9, 0), vec2(0, 0), vec2(0.05, 7.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(7, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-6, -6), vec2(0, 0), vec2(0.05, 1.0)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0.5, -5), vec2(0, 0), vec2(6.5, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(-7.5, -7), vec2(0, 0), vec2(1.5, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(0, 7), vec2(0, 0), vec2(9.0, 0.05)));
	((RigidBody*)obj)->m_bIsKinematic = true;

	m_Objects.push_back(obj = new Box(vec2(8, -7), vec2(0, 0), vec2(1, 0.05)));
	obj->m_v4Color = vec4(0, 1, 0, 1);
	((RigidBody*)obj)->m_bIsKinematic = true;
	((RigidBody*)obj)->tag = 2; // Objective

	m_Objects.push_back(obj = new Box(vec2(-7.5, -5.5), vec2(), vec2(0.5f)));
	obj->m_v4Color = vec4(0.7, 0.7, 0.7, 1);
	((RigidBody*)obj)->m_bIsAwake = false;
	((RigidBody*)obj)->tag = 1; // Player

	m_Objects.push_back(obj = new Circle(vec2(), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(1, 0), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(-1, 0), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(0, 1), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;
	m_Objects.push_back(obj = new Circle(vec2(0, -1), vec2(), 0.5f));
	((RigidBody*)obj)->m_bIsAwake = false;

}