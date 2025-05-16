#pragma once

class Particle
{
public:
	Particle();
	virtual ~Particle();

	void UpdateParticle();
	virtual void Update() = 0;
	virtual void Draw() const;
};
