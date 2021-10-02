#pragma once
#include "Ammunition.h"

/* There's issue with this design. Others weapons are just launch the bullets are done.
This particular weapon shoots constantly. Might want to figure out a different approach */
class Beam :
	public Ammunition
{
private:
	void initAnimation() override;
	void updateIndividualBehavior(const sf::Time& deltaTime) override;
	void onDeletion(bool playAnimation = false) override;
public:
	Beam(Configuration::TexturesWeaponry tex_id, const sf::Vector2f& boundaries, float deg_angle, float speed = 400.f);

	float dealDamage() override;
};