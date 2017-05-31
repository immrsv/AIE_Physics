
#include <aie\Gizmos.h>

#include "Plane.h"


Plane::Plane(glm::vec2 posn, glm::vec2 normal)
	:PhysicsObject(PhysicsObjectType::PLANE, posn), m_v2Normal(normal),  m_v2Parallel(normal.y, -normal.x)
{
}

Plane::~Plane()
{
}


void Plane::draw() {
	
	glm::vec2 start(m_v2Position + m_v2Parallel * glm::vec2(100, 100));
	glm::vec2 end(m_v2Position - m_v2Parallel * glm::vec2(100, 100));

	Gizmos::add2DLine(start, end, glm::vec4(1, 0, 0, 1));
}

void Plane::update(float deltaTime) {

}

void Plane::collideWithPlane(PhysicsObject*) {}
void Plane::collideWithCircle(PhysicsObject*) {}
void Plane::collideWithBox(PhysicsObject*) {}