#include "rigid_body_btphysics.h"

#include "btphysics_private.h"

#include "physics_world_btphysics.h"

#define BT_MATRIX_SIZE 16

namespace h5
{
	rigid_body_btphysics::rigid_body_btphysics()
		: collision_object_btphysics()
	{
		//bt_motion_state	= new btDefaultMotionState();
		//bt_rigid_body	= new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0.0f, bt_motion_state, nullptr));
		//bt_rigid_body->setFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	rigid_body_btphysics::rigid_body_btphysics(const construct_info &info)
		: collision_object_btphysics()
		, cinfo(info)
	{
		//bt_motion_state = new btDefaultMotionState();
		//bt_rigid_body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(info.mass, bt_motion_state, bt_collision_shape, bt_local_inertia));
		//bt_rigid_body->setFlags(
			//info._collision_type == collision_type::statical ? btCollisionObject::CF_STATIC_OBJECT :
			//info._collision_type == collision_type::dynamical ? btCollisionObject::CF_KINEMATIC_OBJECT :
			//btCollisionObject::CF_NO_CONTACT_RESPONSE);

		/*if (info._shape_type == shape_type::sphere)
		{
			bt_collision_shape = new btSphereShape(info.radius);
		}
		else if (info._shape_type == shape_type::convex_hull)
		{
			btConvexHullShape *bt_convex_hull_shape = new btConvexHullShape();
			const size_t indices_count = info.indices_ptr->size();
			for (size_t i = 0; i < indices_count; ++i)
			{
				bt_convex_hull_shape->addPoint(
					btVector3(
						info.vertices_ptr->at(info.indices_ptr->at(i)).x,
						info.vertices_ptr->at(info.indices_ptr->at(i)).y,
						info.vertices_ptr->at(info.indices_ptr->at(i)).z
					)
				);
			}

			bt_collision_shape = bt_convex_hull_shape;
		}
		else if (info._shape_type == shape_type::triangle_mesh)
		{
			bt_triangle_mesh = new btTriangleMesh();
			
			const size_t indices_count = info.indices_ptr->size();
			size_t i = 0;
			while (i < indices_count)
			{
				btVector3 bt_vertex0 = btVector3(
					info.vertices_ptr->at(info.indices_ptr->at(i)).x,
					info.vertices_ptr->at(info.indices_ptr->at(i)).y,
					info.vertices_ptr->at(info.indices_ptr->at(i)).z);
				++i;

				btVector3 bt_vertex1 = btVector3(
					info.vertices_ptr->at(info.indices_ptr->at(i)).x,
					info.vertices_ptr->at(info.indices_ptr->at(i)).y,
					info.vertices_ptr->at(info.indices_ptr->at(i)).z);
				++i;

				btVector3 bt_vertex2 = btVector3(
					info.vertices_ptr->at(info.indices_ptr->at(i)).x,
					info.vertices_ptr->at(info.indices_ptr->at(i)).y,
					info.vertices_ptr->at(info.indices_ptr->at(i)).z);
				++i;

				bt_triangle_mesh->addTriangle(bt_vertex0, bt_vertex1, bt_vertex2);
			}

			bt_collision_shape = new btBvhTriangleMeshShape(bt_triangle_mesh, true);
		}

		btVector3 bt_local_inertia(0.0f, 0.0f, 0.0f);
		if (info._collision_type == collision_object_btphysics::collision_type::dynamical)
		{
			bt_collision_shape->calculateLocalInertia(info.mass, bt_local_inertia);
		}	

		bt_rigid_body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(info.mass, bt_motion_state, bt_collision_shape, bt_local_inertia));
		bt_rigid_body->setFlags(
			info._collision_type == collision_type::statical ? btCollisionObject::CF_STATIC_OBJECT :
			info._collision_type == collision_type::dynamical ? btCollisionObject::CF_KINEMATIC_OBJECT :			
			btCollisionObject::CF_NO_CONTACT_RESPONSE);*/
	}

	rigid_body_btphysics::~rigid_body_btphysics()
	{
		/*delete bt_rigid_body;
		delete bt_collision_shape;

		if (bt_triangle_mesh)
		{
			delete bt_triangle_mesh;
		}

		delete bt_motion_state;*/
	}

	void rigid_body_btphysics::set_mass(float mass)
	{
		bt_rigid_body->setMassProps(mass, btVector3(0.0f, 0.0f, 0.0f));
	}

	float rigid_body_btphysics::get_mass()
	{
		// todo: get normal mass
		return bt_rigid_body->getInvMass();
	}

	void rigid_body_btphysics::clear_forces()
	{
		bt_rigid_body->clearForces();
	}

	void rigid_body_btphysics::set_linear_velocity(const vector3 &v)
	{
		bt_rigid_body->setLinearVelocity(btVector3(v.x, v.y, v.z));
	}

	void rigid_body_btphysics::set_angular_velocity(const vector3 &v)
	{
		bt_rigid_body->setAngularVelocity(btVector3(v.x, v.y, v.z));
	}

	void rigid_body_btphysics::set_translation(const vector3 &v)
	{
		btTransform transform = bt_rigid_body->getWorldTransform();
		transform.setOrigin(btVector3(v.x, v.y, v.z));
		bt_rigid_body->setWorldTransform(transform);
		bt_motion_state->setWorldTransform(transform);
	}

	void rigid_body_btphysics::set_rotation(const quat &r)
	{
		btTransform transform = bt_rigid_body->getWorldTransform();
		transform.setRotation(btQuaternion(r.x, r.y, r.z, r.w));
		bt_rigid_body->setWorldTransform(transform);
		bt_motion_state->setWorldTransform(transform);		
	}

	matrix rigid_body_btphysics::get_transform_matrix() const
	{
		btScalar bp_matrix[BT_MATRIX_SIZE]{};
		bt_rigid_body->getWorldTransform().getOpenGLMatrix(bp_matrix);
		return matrix(
			bp_matrix[0], bp_matrix[1], bp_matrix[2], bp_matrix[3],
			bp_matrix[4], bp_matrix[5], bp_matrix[6], bp_matrix[7],
			bp_matrix[8], bp_matrix[9], bp_matrix[10], bp_matrix[11],
			bp_matrix[12], bp_matrix[13], bp_matrix[14], bp_matrix[15]
		);
	}

	vector3 rigid_body_btphysics::get_translation() const
	{
		btVector3 bt_translation = bt_rigid_body->getWorldTransform().getOrigin();
		return vector3(bt_translation.x(), bt_translation.y(), bt_translation.z());
	}

	quat rigid_body_btphysics::get_rotation() const
	{
		btQuaternion bt_rotation = bt_rigid_body->getWorldTransform().getRotation();
		return quat(bt_rotation.x(), bt_rotation.y(), bt_rotation.z(), bt_rotation.w());
	}

	void rigid_body_btphysics::on_insert_to_physics_world(physics_world_btphysics *ptr)
	{
		ptr->insert_rigid_body(this);
	}

	void rigid_body_btphysics::on_remove_from_physics_world(physics_world_btphysics *ptr)
	{
		ptr->remove_rigid_body(this);
	}
}
