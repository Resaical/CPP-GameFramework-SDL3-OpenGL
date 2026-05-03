#pragma once
#include <vector>


class Entity;

class Component
{
public:
	Component() {}
	virtual ~Component() { delete entity; }

	virtual void Init() {}
	virtual void Update(float dt) {}

	Entity* entity = nullptr;
	bool Active = true;
};

class Entity
{
public:

	template <typename T>
	T* AddComponent()
	{
		T* c = new T();
		c->entity = this;
		m_components.push_back(c);
		return c;
	}

	template <typename T>
	T* GetComponent()
	{
		for (auto& c : m_components)
		{
			T* result = dynamic_cast<T*>(c);
			if (result) return result;
		}
		return nullptr;
	}
	void Init()
	{
		for (auto& c : m_components)
		{
			if (!c->Active) continue;
			c->Init();
		}
	}

	void Update(float dt) 
	{
		for (auto& c : m_components) 
		{
			if (!c->Active) continue;
			c->Update(dt);
		}
	}

private:
	std::vector<Component*> m_components;

};

class EntitySystem
{
public:

	EntitySystem() {};
	~EntitySystem() 
	{
		m_entities.clear();
	};

	Entity* CreateEntity()
	{
		Entity* e = new Entity();
		m_entities.push_back(e);
		return e;
	}

	void Init()
	{
		for (auto& e : m_entities)
		{
			e->Init();
		}

	}

	void Update(float dt)
	{
		for (auto& e : m_entities)
		{
			e->Update(dt);
		}
	}

	std::vector<Entity*> m_entities;
};