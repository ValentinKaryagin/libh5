#pragma once

#include "commons.h"

#define BULLET_PHYSICS_SAFE_RELEASE(ptr) if (ptr) { delete ptr; ptr = nullptr; }

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
struct btDbvtBroadphase;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

struct btDefaultMotionState;
class btCollisionShape;
class btRigidBody;
class btTriangleMesh;
