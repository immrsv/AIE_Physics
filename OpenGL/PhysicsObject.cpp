


#include "PhysicsObject.h"
#include <glm\glm\ext.hpp>


glm::vec2 PhysicsObject::sm_v2Gravity = glm::vec2();
float PhysicsObject::sm_fCoeffRestitution = 1.0;

PhysicsObject::PhysicsObject(PhysicsObjectType objectType, glm::vec2 posn)
	: m_iObjectType(objectType), m_v2Position(posn), m_fRotation(0), m_v4Color(1,0,0,1)
{
}

PhysicsObject::~PhysicsObject()
{
}


bool PhysicsObject::checkCollision(PhysicsObject* obj) {
	bool collision = false;
	switch (obj->m_iObjectType) {
	case PhysicsObjectType::PLANE:
		collision = collideWithPlane(obj);
		break;
	case PhysicsObjectType::CIRCLE:
		collision = collideWithCircle(obj);
		break;
	case PhysicsObjectType::TRIANGLE:
		//collision = collideWithTriangle(obj);
		break;
	case PhysicsObjectType::BOX:
		collision = collideWithBox(obj);
		break;
	}

	return collision;
}


void PhysicsObject::updateRotation() {
	cm_m4Rotation = glm::eulerAngleZ(m_fRotation);
	cm_v2LocalX = vec2(cm_m4Rotation * glm::vec4(1, 0, 0, 0));
	cm_v2LocalY = vec2(cm_m4Rotation * glm::vec4(0, 1, 0, 0));
}


glm::mat4& PhysicsObject::getRotation() {
	if (m_bIsRotationDirty) updateRotation();

	return cm_m4Rotation;
}

glm::vec2& PhysicsObject::getLocalX() {
	if (m_bIsRotationDirty) updateRotation();
	return cm_v2LocalX;
}

glm::vec2& PhysicsObject::getLocalY() {
	if (m_bIsRotationDirty) updateRotation();
	return cm_v2LocalY;
}