#pragma once

class EntitySystem;

class Game
{
public:

	Game() {};
	~Game() 
	{
		delete entitySystem;
	};

	void Init();

	void Update(float dt);

	EntitySystem* entitySystem;
};