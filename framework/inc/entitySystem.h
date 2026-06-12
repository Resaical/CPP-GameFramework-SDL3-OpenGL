#pragma once
#include <vector>
#include <map>

#define MAX_UPDATE_GROUP 6

class Entity;

class Component
{
public:
	Component() {}
	virtual ~Component() { delete entity; }

	virtual void Init() {}
	virtual void Update(float dt) {}
	void SetUpdateGroup(int updateGroupIn = MAX_UPDATE_GROUP) 
	{
		updateGroup = updateGroupIn;
	};
	
	Entity* entity = nullptr;
	bool Active = true;
	int updateGroup = MAX_UPDATE_GROUP;
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

	void Update(int updateGroup, float dt) 
	{
		for (auto& c : m_components) 
		{
			if (c->updateGroup != updateGroup) continue;
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
		for (auto e : entities)
		{
			delete e;
		}
		entities.clear();
	};

	Entity* CreateEntity()
	{
		Entity* e = new Entity();
		entities.push_back(e);
		return e;
	}

	void Init()
	{
		for (int i = 0; i < (MAX_UPDATE_GROUP + 1); i++)
		{
			groupUpdated.push_back(false);
		}

		for (auto& e : entities)
		{
			e->Init();
		}

	}

	void UpdateGroup(float dt, int updateGroup = 0)
	{
		if (updateGroup > MAX_UPDATE_GROUP) return;

		for (auto e : entities)
		{
			e->Update(updateGroup, dt);
		}

		groupUpdated[updateGroup] = true;
	}

	void UpdateAllGroupsLeft(float dt)
	{
		int i = 0;
		for (auto g : groupUpdated)
		{
			if (!g)
			{
				for (auto e : entities)
				{
					e->Update(i, dt);
				}
			}
			i++;
		}
		for (auto& g : groupUpdated)
		{
			g = false;
		}		
	}

	std::vector<bool> groupUpdated;
	std::vector<Entity*> entities;
};