
#include "default.h"

#include <btBulletDynamicsCommon.h>

#include "gfx.h"
#include "blendelf.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct elf_collision {
	ELF_OBJECT_HEADER;
	elf_actor *actor;
	elf_vec3f position;
	elf_vec3f normal;
	float depth;
};

struct elf_joint {
	ELF_OBJECT_HEADER;
	char *name;
	unsigned char joint_type;
	elf_actor *actor1;
	elf_actor *actor2;
	btTypedConstraint *constraint;
	elf_physics_world *world;
	float pivot[3];
	float axis[3];
};

struct elf_physics_tri_mesh {
	ELF_OBJECT_HEADER;
	btTriangleMesh *triMesh;
};

struct elf_physics_object {
	ELF_OBJECT_HEADER;
	int shape_type;
	float mass;
	btCollisionShape *shape;
	btCompoundShape *cshape;
	btRigidBody *body;
	btDefaultMotionState *motionState;
	elf_physics_tri_mesh *tri_mesh;
	elf_physics_world *world;
	elf_list *collisions;
	int collision_count;
	elf_actor *actor;
};

struct elf_physics_world {
	ELF_OBJECT_HEADER;
	btCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btBroadphaseInterface *broadphase;
	btConstraintSolver *solver;
	btDiscreteDynamicsWorld *world;
	float sync;
};

class MultipleRayResultCallback : public btCollisionWorld::RayResultCallback
{
public:
	MultipleRayResultCallback(const btVector3 &rayFromWorld, const btVector3 &rayToWorld, elf_list *list)
	: m_rayFromWorld(rayFromWorld), m_rayToWorld(rayToWorld), m_List(list) {}

	btVector3 m_rayFromWorld;
	btVector3 m_rayToWorld;
	btVector3 m_hitNormalWorld;
	btVector3 m_hitPointWorld;
	elf_list *m_List;

	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
	{
		elf_collision *collision;

		m_closestHitFraction = rayResult.m_hitFraction;
		m_collisionObject = rayResult.m_collisionObject;
		if (normalInWorldSpace)
		{
			m_hitNormalWorld = rayResult.m_hitNormalLocal;
		}
		else
		{
			m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
		}
		m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,rayResult.m_hitFraction);

		collision = elf_create_collision();

		collision->position.x = m_hitPointWorld.x();
		collision->position.y = m_hitPointWorld.y();
		collision->position.z = m_hitPointWorld.z();
		collision->normal.x = m_hitNormalWorld.x();
		collision->normal.y = m_hitNormalWorld.y();
		collision->normal.z = m_hitNormalWorld.z();
		collision->actor = ((elf_physics_object*)((btRigidBody*)m_collisionObject)->getUserPointer())->actor;
		elf_inc_ref((elf_object*)collision->actor);

		elf_append_to_list(m_List, (elf_object*)collision);

		return rayResult.m_hitFraction;
	}
};

elf_physics_world* elf_create_physics_world()
{
	elf_physics_world *world;

	world = (elf_physics_world*)malloc(sizeof(elf_physics_world));
	memset(world, 0x0, sizeof(elf_physics_world));
	world->type = ELF_PHYSICS_WORLD;

	world->collisionConfiguration = new btDefaultCollisionConfiguration();
	world->dispatcher = new btCollisionDispatcher(world->collisionConfiguration);
	world->broadphase = new btDbvtBroadphase();
	world->solver = new btSequentialImpulseConstraintSolver();
	world->world = new btDiscreteDynamicsWorld(world->dispatcher, world->broadphase, world->solver, world->collisionConfiguration);
	world->world->getDispatchInfo().m_enableSPU = true;
	world->world->setGravity(btVector3(0.0f, 0.0f, -9.81f));

	return world;
}

void elf_destroy_physics_world(elf_physics_world *world)
{
	delete world->world;
	delete world->solver;
	delete world->broadphase;
	delete world->dispatcher;
	delete world->collisionConfiguration;

	free(world);
}

void elf_update_physics_world(elf_physics_world *world, float time)
{
	int manifold_count;
	int contact_count;
	btPersistentManifold *manifold = 0;
	btManifoldPoint *point = 0;
	elf_physics_object *obj0;
	elf_physics_object *obj1;
	elf_collision *col0;
	elf_collision *col1;
	int i, j;

	world->world->stepSimulation(time, 4);

	manifold_count = world->dispatcher->getNumManifolds();
	contact_count = 0;

	for(i = 0; i < manifold_count; i++)
	{
		manifold = world->dispatcher->getManifoldByIndexInternal(i);
		contact_count = manifold->getNumContacts();
		if(!contact_count) continue;

		obj0 = (elf_physics_object*)((btRigidBody*)manifold->getBody0())->getUserPointer();
		obj1 = (elf_physics_object*)((btRigidBody*)manifold->getBody1())->getUserPointer();
		if(obj0 == obj1) continue;

		// add elf_clear_physic_object_collisions somewhere...

		while(elf_get_list_length(obj0->collisions) < obj0->collision_count+contact_count)
		{
			col0 = elf_create_collision();
			elf_append_to_list(obj0->collisions, (elf_object*)col0);
		}
		while(elf_get_list_length(obj1->collisions) < obj1->collision_count+contact_count)
		{
			col1 = elf_create_collision();
			elf_append_to_list(obj1->collisions, (elf_object*)col1);
		}

		obj0->collision_count += contact_count;
		obj1->collision_count += contact_count;

		for(j = 0, col0 = (elf_collision*)elf_begin_list(obj0->collisions),
			col1 = (elf_collision*)elf_begin_list(obj1->collisions); j < contact_count;
			col0 = (elf_collision*)elf_next_in_list(obj0->collisions),
			col1 = (elf_collision*)elf_next_in_list(obj1->collisions), j++)
		{
			point = &manifold->getContactPoint(j);

			if(col0->actor) elf_dec_ref((elf_object*)col0->actor);
			col0->actor = obj1->actor;
			elf_inc_ref((elf_object*)col0->actor);
			col0->position.x = point->m_positionWorldOnB.getX();
			col0->position.y = point->m_positionWorldOnB.getY();
			col0->position.z = point->m_positionWorldOnB.getZ();
			col0->normal.x = point->m_normalWorldOnB.getX();
			col0->normal.y = point->m_normalWorldOnB.getY();
			col0->normal.z = point->m_normalWorldOnB.getZ();
			col0->depth = point->m_distance1;

			if(col1->actor) elf_dec_ref((elf_object*)col1->actor);
			col1->actor = obj0->actor;
			elf_inc_ref((elf_object*)col1->actor);
			col1->position.x = point->m_positionWorldOnB.getX();
			col1->position.y = point->m_positionWorldOnB.getY();
			col1->position.z = point->m_positionWorldOnB.getZ();
			col1->normal.x = point->m_normalWorldOnB.getX();
			col1->normal.y = point->m_normalWorldOnB.getY();
			col1->normal.z = point->m_normalWorldOnB.getZ();
			col1->depth = point->m_distance1;
		}
	}
}

void elf_set_physics_world_gravity(elf_physics_world *world, float x, float y, float z)
{
	world->world->setGravity(btVector3(x, y, z));
}

elf_vec3f elf_get_physics_world_gravity(elf_physics_world *world)
{
	btVector3 grav;
	elf_vec3f result;

	grav = world->world->getGravity();

	result.x = grav.x();
	result.y = grav.y();
	result.z = grav.z();

	return result;
}

elf_collision* elf_get_ray_cast_result(elf_physics_world *world, float x, float y, float z, float dx, float dy, float dz)
{
	elf_collision *collision;

	btCollisionWorld::ClosestRayResultCallback rayResult(btVector3(x, y, z), btVector3(dx, dy, dz));

	world->world->getCollisionWorld()->rayTest(btVector3(x, y, z), btVector3(dx, dy, dz), rayResult);

	if(!rayResult.hasHit()) return NULL;

	collision = elf_create_collision();

	collision->position.x = rayResult.m_hitPointWorld.x();
	collision->position.y = rayResult.m_hitPointWorld.y();
	collision->position.z = rayResult.m_hitPointWorld.z();
	collision->normal.x = rayResult.m_hitNormalWorld.x();
	collision->normal.y = rayResult.m_hitNormalWorld.y();
	collision->normal.z = rayResult.m_hitNormalWorld.z();
	collision->actor = ((elf_physics_object*)((btRigidBody*)rayResult.m_collisionObject)->getUserPointer())->actor;
	elf_inc_ref((elf_object*)collision->actor);

	return collision;
}

elf_list* elf_get_ray_cast_results(elf_physics_world *world, float x, float y, float z, float dx, float dy, float dz)
{
	elf_list *list;

	list = elf_create_list();

	MultipleRayResultCallback rayResult(btVector3(x, y, z), btVector3(dx, dy, dz), list);

	world->world->getCollisionWorld()->rayTest(btVector3(x, y, z), btVector3(dx, dy, dz), rayResult);

	if(!rayResult.hasHit()) return NULL;

	return list;
}

elf_collision* elf_create_collision()
{
	elf_collision *collision;

	collision = (elf_collision*)malloc(sizeof(elf_collision));
	memset(collision, 0x0, sizeof(elf_collision));
	collision->type = ELF_COLLISION;

	return collision;
}

void elf_destroy_collision(elf_collision *collision)
{
	if(collision->actor) elf_dec_ref((elf_object*)collision->actor);

	free(collision);
}

elf_actor* elf_get_collision_actor(elf_collision *collision)
{
	return collision->actor;
}

elf_vec3f elf_get_collision_position(elf_collision *collision)
{
	return collision->position;
}

elf_vec3f elf_get_collision_normal(elf_collision *collision)
{
	return collision->normal;
}

float elf_get_collision_depth(elf_collision *collision)
{
	return collision->depth;
}

elf_joint* elf_create_joint()
{
	elf_joint *joint;

	joint = (elf_joint*)malloc(sizeof(elf_joint));
	memset(joint, 0x0, sizeof(elf_joint));
	joint->type = ELF_JOINT;

	return joint;
}

void elf_recalc_joint(elf_joint *joint)
{
	elf_vec3f pos1;
	elf_vec3f pos2;
	elf_vec4f qua1;
	elf_vec4f qua2;
	elf_vec4f aqua1;
	elf_vec4f aqua2;
	elf_vec3f world_coord_pivot1;
	elf_vec3f local_coord_pivot2;
	elf_vec3f local_axis1;
	elf_vec3f local_axis2;
	float matrix1[16];
	float matrix2[16];
	float matrix3[16];

	pos1 = elf_get_actor_position(joint->actor1);
	pos2 = elf_get_actor_position(joint->actor2);
	qua1 = elf_get_actor_orientation(joint->actor1);
	qua2 = elf_get_actor_orientation(joint->actor2);

	world_coord_pivot1.x = joint->pivot[0];
	world_coord_pivot1.y = joint->pivot[1];
	world_coord_pivot1.z = joint->pivot[2];

	world_coord_pivot1 = elf_add_vec3f_vec3f(elf_mul_qua_vec3f(qua1, world_coord_pivot1), pos1);
	local_coord_pivot2 = elf_mul_qua_vec3f(elf_get_qua_inverted(qua2), elf_sub_vec3f_vec3f(world_coord_pivot1, pos2));

	btVector3 pivotInA(joint->pivot[0], joint->pivot[1], joint->pivot[2]);
	btVector3 pivotInB(local_coord_pivot2.x, local_coord_pivot2.y, local_coord_pivot2.z);

	aqua1 = elf_create_qua_from_euler(joint->axis[0], joint->axis[1], joint->axis[2]);
	local_axis1 = elf_mul_qua_vec3f(aqua1, elf_create_vec3f_from_values(0.0, 0.0, 1.0));

	aqua2 = elf_mul_qua_qua(aqua1, elf_mul_qua_qua(qua1, elf_get_qua_inverted(qua2)));
	local_axis2 = elf_mul_qua_vec3f(aqua2, elf_create_vec3f_from_values(0.0, 0.0, 1.0));

	btVector3 axisInA(local_axis1.x, local_axis1.y, local_axis1.z);
	btVector3 axisInB(local_axis2.x, local_axis2.y, local_axis2.z);

	if(joint->joint_type == ELF_HINGE)
	{
		joint->constraint = new btHingeConstraint(*joint->actor1->object->body, *joint->actor2->object->body,
			pivotInA, pivotInB, axisInA, axisInB);
		joint->actor1->scene->world->world->addConstraint(joint->constraint);
	}
	else if(joint->joint_type == ELF_BALL)
	{
		joint->constraint = new btPoint2PointConstraint(*joint->actor1->object->body, *joint->actor2->object->body, pivotInA, pivotInB);
		joint->actor1->scene->world->world->addConstraint(joint->constraint);
	}
	else if(joint->joint_type == ELF_CONE_TWIST)
	{
		gfx_matrix4_set_identity(matrix1);
		matrix1[12] = joint->pivot[0];
		matrix1[13] = joint->pivot[1];
		matrix1[14] = joint->pivot[2];
		gfx_qua_to_matrix4(&aqua1.x, matrix2);
		gfx_mul_matrix4_matrix4(matrix1, matrix2, matrix3);
		btTransform frameInA;
		frameInA.setFromOpenGLMatrix(matrix3);

		gfx_matrix4_set_identity(matrix1);
		matrix1[12] = local_coord_pivot2.x;
		matrix1[13] = local_coord_pivot2.y;
		matrix1[14] = local_coord_pivot2.z;
		gfx_qua_to_matrix4(&aqua2.x, matrix2);
		gfx_mul_matrix4_matrix4(matrix1, matrix2, matrix3);
		btTransform frameInB;
		frameInB.setFromOpenGLMatrix(matrix3);

		joint->constraint = new btConeTwistConstraint(*joint->actor1->object->body,
			*joint->actor2->object->body, frameInA, frameInB);
		joint->actor1->scene->world->world->addConstraint(joint->constraint);
	}
}

void elf_set_joint_world(elf_joint *joint, elf_physics_world *world)
{
	if(joint->world && joint->constraint)
	{
		joint->world->world->removeConstraint(joint->constraint);
		delete joint->constraint;
		joint->constraint = NULL;
	}
	joint->world = world;
	if(joint->world)
	{
		if(joint->world != joint->actor1->scene->world || joint->world != joint->actor2->scene->world)
		{
			joint->world = NULL;
			return;
		}
		elf_recalc_joint(joint);
	}
}

elf_joint* elf_create_hinge_joint(elf_actor *actor1, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az)
{
	elf_joint *joint;

	joint = elf_create_joint();
	joint->joint_type = ELF_HINGE;
	if(name) joint->name = elf_create_string(name);

	joint->actor1 = actor1;
	joint->actor2 = actor2;

	joint->pivot[0] = px;
	joint->pivot[1] = py;
	joint->pivot[2] = pz;

	joint->axis[0] = ax;
	joint->axis[1] = ay;
	joint->axis[2] = az;

	if(actor1->scene) elf_set_joint_world(joint, actor1->scene->world);

	return joint;
}

elf_joint* elf_create_ball_joint(elf_actor *actor1, elf_actor *actor2, const char *name, float px, float py, float pz)
{
	elf_joint *joint;

	joint = elf_create_joint();
	joint->joint_type = ELF_BALL;
	if(name) joint->name = elf_create_string(name);

	joint->actor1 = actor1;
	joint->actor2 = actor2;

	joint->pivot[0] = px;
	joint->pivot[1] = py;
	joint->pivot[2] = pz;

	if(actor1->scene) elf_set_joint_world(joint, actor1->scene->world);

	return joint;
}

elf_joint* elf_create_cone_twist_joint(elf_actor *actor1, elf_actor *actor2, const char *name, float px, float py, float pz, float ax, float ay, float az)
{
	elf_joint *joint;

	joint = elf_create_joint();
	joint->joint_type = ELF_BALL;
	if(name) joint->name = elf_create_string(name);

	joint->actor1 = actor1;
	joint->actor2 = actor2;

	joint->pivot[0] = px;
	joint->pivot[1] = py;
	joint->pivot[2] = pz;

	if(actor1->scene) elf_set_joint_world(joint, actor1->scene->world);

	return joint;
}

void elf_clear_joint(elf_joint *joint)
{
	if(joint->constraint)
	{
		if(joint->world) joint->world->world->removeConstraint(joint->constraint);
		delete joint->constraint;
		joint->constraint = NULL;
	}
	joint->actor1 = NULL;
	joint->actor2 = NULL;
	joint->world = NULL;
}

void elf_destroy_joint(elf_joint *joint)
{
	if(joint->name) elf_destroy_string(joint->name);
	if(joint->constraint)
	{
		if(joint->world) joint->world->world->removeConstraint(joint->constraint);
		delete joint->constraint;
	}
	free(joint);
}

const char* elf_get_joint_name(elf_joint *joint)
{
	return joint->name;
}

int elf_get_joint_type(elf_joint *joint)
{
	return joint->joint_type;
}

elf_actor* elf_get_joint_actor_a(elf_joint *joint)
{
	return joint->actor1;
}

elf_actor* elf_get_joint_actor_b(elf_joint *joint)
{
	return joint->actor2;
}

elf_vec3f elf_get_joint_pivot(elf_joint *joint)
{
	elf_vec3f result;

	result.x = joint->pivot[0];
	result.y = joint->pivot[1];
	result.z = joint->pivot[2];

	return result;
}

elf_vec3f elf_get_joint_axis(elf_joint *joint)
{
	elf_vec3f result;

	result.x = joint->axis[0];
	result.y = joint->axis[1];
	result.z = joint->axis[2];

	return result;
}

elf_physics_tri_mesh* elf_create_physics_tri_mesh(float *verts, unsigned int *idx, int indice_count)
{
	elf_physics_tri_mesh *tri_mesh;
	int i;

	if(indice_count < 3) return NULL;

	tri_mesh = (elf_physics_tri_mesh*)malloc(sizeof(elf_physics_tri_mesh));
	memset(tri_mesh, 0x0, sizeof(elf_physics_tri_mesh));
	tri_mesh->type = ELF_PHYSICS_TRI_MESH;

	tri_mesh->triMesh = new btTriangleMesh();

	for(i = 0; i < indice_count; i+=3)
	{
		tri_mesh->triMesh->addTriangle(
			btVector3(verts[idx[i]*3], verts[idx[i]*3+1], verts[idx[i]*3+2]),
			btVector3(verts[idx[i+1]*3], verts[idx[i+1]*3+1], verts[idx[i+1]*3+2]),
			btVector3(verts[idx[i+2]*3], verts[idx[i+2]*3+1], verts[idx[i+2]*3+2])
			);
	}

	return tri_mesh;
}

void elf_destroy_physics_tri_mesh(elf_physics_tri_mesh *tri_mesh)
{
	delete tri_mesh->triMesh;

	free(tri_mesh);
}

elf_physics_object* elf_create_physics_object()
{
	elf_physics_object *object;

	object = (elf_physics_object*)malloc(sizeof(elf_physics_object));
	memset(object, 0x0, sizeof(elf_physics_object));
	object->type = ELF_PHYSICS_OBJECT;

	object->collisions = elf_create_list();
	elf_inc_ref((elf_object*)object->collisions);

	return object;
}

elf_physics_object* elf_create_physics_object_mesh(elf_physics_tri_mesh *tri_mesh, float mass)
{
	elf_physics_object *object;

	object = elf_create_physics_object();

	object->shape = new btBvhTriangleMeshShape(tri_mesh->triMesh, true);

	object->shape_type = ELF_MESH;
	object->mass = mass;

	object->tri_mesh = tri_mesh;
	elf_inc_ref((elf_object*)tri_mesh);

	btScalar bodyMass(mass);
	btVector3 localInertia(0.0, 0.0, 0.0);

	if(!elf_about_zero(mass)) object->shape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setOrigin(btVector3(0.0, 0.0, 0.0));
	startTransform.setRotation(btQuaternion(0.0, 0.0, 0.0, 1.0));

	object->motionState = new btDefaultMotionState(startTransform);
	object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

	object->body->setUserPointer(object);

	return object;
}

elf_physics_object* elf_create_physics_object_sphere(float radius, float mass, float ox, float oy, float oz)
{
	elf_physics_object *object;

	object = elf_create_physics_object();

	object->shape = new btSphereShape(btScalar(radius));
	if(!elf_about_zero(ox) || !elf_about_zero(oy) || !elf_about_zero(oz))
	{
		btTransform localTrans;
		localTrans.setOrigin(btVector3(ox, oy, oz));
		object->cshape = new btCompoundShape();
		object->cshape->addChildShape(localTrans, object->shape);
	}

	object->shape_type = ELF_SPHERE;

	object->mass = mass;

	btScalar bodyMass(mass);
	btVector3 localInertia(0.0, 0.0, 0.0);

	if(!elf_about_zero(mass)) object->shape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setOrigin(btVector3(0.0, 0.0, 0.0));
	startTransform.setRotation(btQuaternion(0.0, 0.0, 0.0, 1.0));

	object->motionState = new btDefaultMotionState(startTransform);
	object->body = new btRigidBody(bodyMass, object->motionState, object->cshape ? object->cshape : object->shape, localInertia);

	object->body->setUserPointer(object);

	return object;
}

elf_physics_object* elf_create_physics_object_box(float hx, float hy, float hz, float mass, float ox, float oy, float oz)
{
	elf_physics_object *object;

	object = elf_create_physics_object();

	object->shape = new btBoxShape(btVector3(hx+0.001, hy+0.001, hz+0.001));
	if(!elf_about_zero(ox) || !elf_about_zero(oy) || !elf_about_zero(oz))
	{
		btTransform localTrans;
		localTrans.setOrigin(btVector3(ox, oy, oz));
		localTrans.setRotation(btQuaternion(0.0, 0.0, 0.0, 1.0));
		object->cshape = new btCompoundShape();
		object->cshape->addChildShape(localTrans, object->shape);
	}

	object->shape_type = ELF_BOX;

	object->mass = mass;

	btScalar bodyMass(mass);
	btVector3 localInertia(0.0, 0.0, 0.0);

	if(!elf_about_zero(mass)) object->shape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setOrigin(btVector3(0.0, 0.0, 0.0));
	startTransform.setRotation(btQuaternion(0.0, 0.0, 0.0, 1.0));

	object->motionState = new btDefaultMotionState(startTransform);
	object->body = new btRigidBody(bodyMass, object->motionState, object->cshape ? object->cshape : object->shape, localInertia);

	object->body->setUserPointer(object);

	return object;
}

void elf_set_physics_object_world(elf_physics_object *object, elf_physics_world *world)
{
	if(object->world)
	{
		object->world->world->removeRigidBody(object->body);
		object->world = NULL;
	}
	object->world = world;
	if(object->world) object->world->world->addRigidBody(object->body);
}

void elf_destroy_physics_object(elf_physics_object *object)
{
	if(object->body)
	{
		if(object->world) object->world->world->removeRigidBody(object->body);
		delete object->body;
	}
	if(object->cshape) delete object->cshape;
	if(object->shape) delete object->shape;
	if(object->motionState) delete object->motionState;
	if(object->tri_mesh) elf_dec_ref((elf_object*)object->tri_mesh);
	elf_dec_ref((elf_object*)object->collisions);

	free(object);
}

void elf_set_physics_object_actor(elf_physics_object *object, elf_actor *actor)
{
	object->actor = actor;
}

void elf_remove_physics_object_collisions(elf_physics_object *object)
{
	if(elf_get_list_length(object->collisions) > 0)
	{
		elf_dec_ref((elf_object*)object->collisions);
		object->collisions = elf_create_list();
		elf_inc_ref((elf_object*)object->collisions);
	}
	object->collision_count = 0;
}

void elf_clear_physics_object_collisions(elf_physics_object *object)
{
	if(elf_get_list_length(object->collisions) > 0) elf_remove_from_list(object->collisions, elf_begin_list(object->collisions));
	object->collision_count = 0;
}

int elf_get_physics_object_collision_count(elf_physics_object *object)
{
	return elf_get_list_length(object->collisions);
}

elf_collision* elf_get_physics_object_collision(elf_physics_object *object, int idx)
{
	if(idx < 0 || idx > elf_get_list_length(object->collisions)-1) return NULL;

	return (elf_collision*)elf_get_item_from_list(object->collisions, idx);
}

void elf_set_physics_object_position(elf_physics_object *object, float x, float y, float z)
{
	object->body->activate(true);
	if(object->body->isStaticObject())
	{
		object->body->setCollisionFlags(object->body->getCollisionFlags() |
			btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	btTransform xform = object->body->getCenterOfMassTransform();
	xform.setOrigin(btVector3(x, y, z));
	object->body->setCenterOfMassTransform(xform);
	object->motionState->setWorldTransform(xform);
}

void elf_set_physics_object_orientation(elf_physics_object *object, float x, float y, float z, float w)
{
	object->body->activate(true);
	if(object->body->isStaticObject())
	{
		object->body->setCollisionFlags(object->body->getCollisionFlags() |
			btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	btTransform xform = object->body->getCenterOfMassTransform();
	xform.setRotation(btQuaternion(x, y, z, w));
	object->body->setCenterOfMassTransform(xform);
	object->motionState->setWorldTransform(xform);
}

void elf_set_physics_object_scale(elf_physics_object *object, float x, float y, float z)
{
	object->body->activate(true);
	object->shape->setLocalScaling(btVector3(x, y, z));
}

int elf_get_physics_object_shape(elf_physics_object *object)
{
	return object->shape_type;
}

float elf_get_physics_object_mass(elf_physics_object *object)
{
	return object->mass;
}

void elf_get_physics_object_position(elf_physics_object *object, float *params)
{
	btVector3 origin = object->motionState->m_graphicsWorldTrans.getOrigin();
	params[0] = origin.x();
	params[1] = origin.y();
	params[2] = origin.z();
}

void elf_get_physics_object_orientation(elf_physics_object *object, float *params)
{
	btQuaternion orient = object->motionState->m_graphicsWorldTrans.getRotation();
	params[0] = orient.x();
	params[1] = orient.y();
	params[2] = orient.z();
	params[3] = orient.w();
}

void elf_get_physics_object_scale(elf_physics_object *object, float *params)
{
	btVector3 origin = object->shape->getLocalScaling();
	params[0] = origin.x();
	params[1] = origin.y();
	params[2] = origin.z();
}

unsigned char elf_is_physics_object_static(elf_physics_object *object)
{
	return object->body->isStaticObject();
}

void elf_set_physics_object_anisotropic_friction(elf_physics_object *object, float x, float y, float z)
{
	object->body->setAnisotropicFriction(btVector3(x, y, z));
}

void elf_set_physics_object_damping(elf_physics_object *object, float lin_damp, float ang_damp)
{
	object->body->setDamping(lin_damp, ang_damp);
}

void elf_set_physics_object_sleep_thresholds(elf_physics_object *object, float lin_thrs, float ang_thrs)
{
	object->body->setSleepingThresholds(lin_thrs, ang_thrs);
}

void elf_set_physics_object_restitution(elf_physics_object *object, float restitution)
{
	object->body->setRestitution(restitution);
}

void elf_add_force_to_physics_object(elf_physics_object *object, float x, float y, float z)
{
	object->body->activate(true);
	object->body->applyCentralForce(btVector3(x, y, z));
}

void elf_add_torque_to_physics_object(elf_physics_object *object, float x, float y, float z)
{
	object->body->activate(true);
	object->body->applyTorque(btVector3(x, y, z));
}

void elf_set_physics_object_linear_velocity(elf_physics_object *object, float x, float y, float z)
{
	object->body->activate(true);
	object->body->setLinearVelocity(btVector3(x, y, z));
}

void elf_set_physics_object_angular_velocity(elf_physics_object *object, float x, float y, float z)
{
	 object->body->activate(true);
	object->body->setAngularVelocity(btVector3(x, y, z));
}

void elf_set_physics_object_linear_factor(elf_physics_object *object, float x, float y, float z)
{
	object->body->setLinearFactor(btVector3(x, y, z));
}

void elf_set_physics_object_angular_factor(elf_physics_object *object, float x, float y, float z)
{
	object->body->setAngularFactor(btVector3(x, y, z));
}

void elf_get_physics_object_linear_velocity(elf_physics_object *object, float *params)
{
	btVector3 vec;
	vec = object->body->getLinearVelocity();
	params[0] = vec.x();
	params[1] = vec.y();
	params[2] = vec.z();
}

void elf_get_physics_object_angular_velocity(elf_physics_object *object, float *params)
{
	btVector3 vec;
	vec = object->body->getAngularVelocity();
	params[0] = vec.x();
	params[1] = vec.y();
	params[2] = vec.z();
}

void elf_get_physics_object_linear_factor(elf_physics_object *object, float *params)
{
	btVector3 vec;
	vec = object->body->getLinearFactor();
	params[0] = vec.x();
	params[1] = vec.y();
	params[2] = vec.z();
}

void elf_get_physics_object_angular_factor(elf_physics_object *object, float *params)
{
	btVector3 vec;
	vec = object->body->getAngularFactor();
	params[0] = vec.x();
	params[1] = vec.y();
	params[2] = vec.z();
}

void elf_get_physics_object_anisotropic_friction(elf_physics_object *object, float *params)
{
	btVector3 vec;
	vec = object->body->getAnisotropicFriction();
	params[0] = vec.x();
	params[1] = vec.y();
	params[2] = vec.z();
}

float elf_get_physics_object_linear_damping(elf_physics_object *object)
{
	return object->body->getLinearDamping();
}

float elf_get_physics_object_angular_damping(elf_physics_object *object)
{
	return object->body->getAngularDamping();
}

float elf_get_physics_object_linear_sleep_threshold(elf_physics_object *object)
{
	return object->body->getLinearSleepingThreshold();
}

float elf_get_physics_object_angular_sleep_threshold(elf_physics_object *object)
{
	return object->body->getAngularSleepingThreshold();
}

float elf_get_physics_object_restitution(elf_physics_object *object)
{
	return object->body->getRestitution();
}

#ifdef __cplusplus
}
#endif

