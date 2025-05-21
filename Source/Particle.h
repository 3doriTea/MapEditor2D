#pragma once
#include <vector>
#include "../Library/GameObject.h"

template<class ParticleT>
class Particle;

template<class ParticleT>
class ParticleManager : public GameObject
{
public:
	ParticleManager() {};
	~ParticleManager() {};

	void Spawan(const int _x, const int _y) {};

private:
	std::vector<ParticleT> particles_{};
};

template<class ParticleT>
class Particle
{
	friend ParticleManager;

public:
	Particle() {};
	virtual ~Particle() {};

	void Update();
	virtual void UpdateParticle() = 0;
	virtual void DrawParticle() const = 0;
};
