#include "glib/physics/PhysicsManager.h"
#include "glib/physics/component/BoxCollider.h"

#include <vector>
#include <iostream>

namespace glib
{
	class PhysicsManagerImpl
	{
	private:
		std::vector<PhysicsObject*> m_Objects;
		Vec2 m_Gravity;
	public:
		PhysicsManagerImpl(Vec2 gravity) : m_Gravity(gravity)
		{

		}
		
		~PhysicsManagerImpl()
		{

		}

		void Add(PhysicsObject* obj)
		{
			m_Objects.push_back(obj);
		}

		void Remove(PhysicsObject* obj)
		{
			m_Objects.erase(std::find(m_Objects.begin(), m_Objects.end(), obj));
		}

		void Update(float _delta)
		{
			float delta = _delta / 1000.0f;

			for (PhysicsObject* obj : m_Objects)
			{
				obj->pos.x += obj->velocity.x * delta;
				obj->pos.y += obj->velocity.y * delta;

				if (obj->hasGravity)
				{
					obj->velocity.x -= m_Gravity.x * delta;
					obj->velocity.y -= m_Gravity.y * delta;
				}

				BoxCollider* bc = nullptr;
				if (obj->HasComponent(BOX_COLLIDER))
				{
					bc = (BoxCollider*)obj->GetFirstComponent(BOX_COLLIDER);
				}

				for (PhysicsObject* other : m_Objects)
				{
					//float dst = obj->Distance(other);
					BoxCollider* bc1 = nullptr;
					if (other->HasComponent(BOX_COLLIDER))
					{
						bc1 = (BoxCollider*)other->GetFirstComponent(BOX_COLLIDER);
					}

					if (bc != nullptr && bc1 != nullptr)
					{
						if (bc->CheckFace(TOP, bc1))
						{
							std::cout << "COLLIDING!" << std::endl;
							other->velocity.y = 0;
							other->pos.y = obj->pos.y - (bc1->p1.y + bc1->p2.y);
						}
					}
				}
			}
		}
	};
}

using namespace glib;

glib::PhysicsManager::PhysicsManager()
{
	impl = new PhysicsManagerImpl(gravity);
}

glib::PhysicsManager::~PhysicsManager()
{
	delete impl;
}

void glib::PhysicsManager::Add(PhysicsObject& obj)
{
	impl->Add(&obj);
}

void glib::PhysicsManager::Add(PhysicsObject* obj)
{
	impl->Add(obj);
}

void glib::PhysicsManager::Remove(PhysicsObject& obj)
{
	impl->Remove(&obj);
}

void glib::PhysicsManager::Remove(PhysicsObject* obj)
{
	impl->Remove(obj);
}

void glib::PhysicsManager::Update(float delta)
{
	impl->Update(delta);
}
