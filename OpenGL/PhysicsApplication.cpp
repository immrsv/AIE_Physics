#include "PhysicsApplication.h"

#include <iostream>

#include "Plane.h"
#include "Circle.h"
#include "Box.h"
#include "Spring.h"

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
	
	MakeScene1();

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

	for (auto obj : m_Objects){

		RigidBody* rb = dynamic_cast<RigidBody*>(obj);


		// Gravity
		if (rb) rb->applyForce(dt * rb->m_fMass * PhysicsObject::sm_v2Gravity);

		// Collision
		for (auto candidate : m_Objects ){
			if (candidate == obj) break; // Don't check self!
			obj->checkCollision(candidate);
		}

		// Motion
		obj->update(dt);
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
	vec4 orange(1, 0.7f, 0.2f, 1.0f);
	vec4 red(1, 0, 0, 1);
	vec4 white(1);
	vec4 black(0, 0, 0, 1);



	for (int i = 0; i < 21; ++i) {
		Gizmos::add2DLine(vec2(-10 + i, 10), vec2(-10 + i, -10), i == 10 ? orange : white);
		Gizmos::add2DLine(vec2(10, -10 + i), vec2(-10, -10 + i), i == 10 ? orange : white);
	}

	// test rendering code
	//Gizmos::add2DCircle(vec2(0, -4), 1, 32, red);
	//Gizmos::add2DAABBFilled(vec2(0, 1), vec2(1, 3), red);

	for (auto obj : m_Objects) {
		obj->draw();
	}

	Gizmos::draw2D(projection * view);

	glfwSwapBuffers(window);
	glfwPollEvents();

	day++;
}

void PhysicsApplication::MakeScene1() {

	PhysicsObject::sm_v2Gravity = glm::vec2(0, 0);
	PhysicsObject::sm_fCoeffRestitution = 1.0f;

	m_Objects.clear();

	RigidBody* obj = new Box(vec2(0, 0), vec2(0, 0));
	obj->m_fAngularVelocity = 1.0f;

	m_Objects.push_back(obj);

	obj = new Circle(vec2(-4, 1), vec2(1, 0), 1);
	m_Objects.push_front(obj);
}

void PhysicsApplication::MakeScene2() {
	PhysicsObject::sm_v2Gravity = glm::vec2(0, 0);
	PhysicsObject::sm_fCoeffRestitution = 1.0f;

	m_Objects.clear();

	RigidBody* obj = new Box(vec2(0, 0), vec2(0, 0));
	obj->m_fRotation = 1.57f;

	m_Objects.push_back(obj);

	obj = new Circle(vec2(-4, 1), vec2(1, 0), 0.5);
	m_Objects.push_front(obj);
}

void PhysicsApplication::MakeScene3() {
	PhysicsObject::sm_v2Gravity = glm::vec2(0, -5);
	PhysicsObject::sm_fCoeffRestitution = 0.5f;

	m_Objects.clear();

	m_Objects.push_back(new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 1));
	m_Objects.push_back(new Plane(glm::vec2(-8.5, 0), glm::vec2(1, 0)));
	m_Objects.push_back(new Plane(glm::vec2(8.5, 0), glm::vec2(-1, 0)));
	m_Objects.push_back(new Plane(glm::vec2(0, -6.5), glm::vec2(0, 1)));
	m_Objects.push_back(new Plane(glm::vec2(0, 6.5), glm::vec2(0, -1)));

	Box* b = new Box(vec2(3, 3), vec2(0, 0));
	b->m_fRotation = 1.0f;

	m_Objects.push_back(b);
}


void PhysicsApplication::MakeScene4() {

	PhysicsObject::sm_v2Gravity = glm::vec2(0, 0);
	PhysicsObject::sm_fCoeffRestitution = 1.0f;

	m_Objects.clear();


	m_Objects.push_back(new Plane(glm::vec2(-6.5, 0), glm::vec2(1, 0)));
	m_Objects.push_back(new Plane(glm::vec2(6.5, 0), glm::vec2(-1, 0)));
	m_Objects.push_back(new Plane(glm::vec2(0, -6.5), glm::vec2(0, 1)));
	m_Objects.push_back(new Plane(glm::vec2(0, 6.5), glm::vec2(0, -1)));

	RigidBody* obj;
	m_Objects.push_back(obj = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 1));
	obj->tag = 1;

	m_Objects.push_back(obj = new Circle(glm::vec2(-4, -6), glm::vec2(1, 1), 1));
	obj->tag = 2;
}

void PhysicsApplication::MakeScene5() {
	PhysicsObject::sm_v2Gravity = glm::vec2(0, 0);
	PhysicsObject::sm_fCoeffRestitution = 1.0f;

	m_Objects.clear();


	m_Objects.push_back(new Plane(glm::vec2(-6.5, 0), glm::vec2(1, 0)));
	m_Objects.push_back(new Plane(glm::vec2(6.5, 0), glm::vec2(-1, 0)));
	m_Objects.push_back(new Plane(glm::vec2(0, -6.5), glm::vec2(0, 1)));
	m_Objects.push_back(new Plane(glm::vec2(0, 6.5), glm::vec2(0, -1)));


	m_Objects.push_back(new Box(glm::vec2(0, -2), glm::vec2(0, 0), vec2(1, 1)));

	RigidBody* obj = new Box(glm::vec2(-4, -2.75), glm::vec2(2, 0), vec2(1,1));
	obj->m_fRotation = 3.14f / 4.0f;

	m_Objects.push_back(obj);

}


void PhysicsApplication::MakeScene6() {

	PhysicsObject::sm_v2Gravity = glm::vec2(0, 0);
	PhysicsObject::sm_fCoeffRestitution = 1.0f;

	m_Objects.clear();

	RigidBody *obj1, *obj2;

	m_Objects.push_back(obj1 = new Circle(vec2(-2, 0.5), vec2(0, 0), 1));
	m_Objects.push_back(obj2 = new Circle(vec2( 2, 0.5), vec2(0, 0), 1));
	m_Objects.push_back(new Spring(obj1, vec2(0.5), obj2, vec2(0.5), 3, 1));

}