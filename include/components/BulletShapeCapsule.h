#pragma once
#include "../IBulletShape.h"
#include "Sigma.h"

namespace Sigma{
	class Mesh;
	class BulletShapeCapsule : public IBulletShape {
	public:
		SET_COMPONENT_TYPENAME("BulletShapeCapsule");
		BulletShapeCapsule(const id_t entityID = 0) : IBulletShape(entityID) { }
		~BulletShapeCapsule() {
			if (this->motionState) {
				delete this->motionState;
			}
		}

		void SetCapsuleSize(float radius, float height) {
			this->radius = radius;
			this->height = height;
		}

		void InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz);
	private:
		float radius;
		float height;
		btMotionState* motionState;
	};
}
