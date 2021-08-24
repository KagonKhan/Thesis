﻿#include "pch.h"
#include "Player.h"

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	for (auto&& weapon : m_Weapons)
		target.draw(*weapon);

	target.draw(m_Sprite);
	target.draw(m_ExhaustAnimatedSpriteLeft);
	target.draw(m_ExhaustAnimatedSpriteRight);
	
	
}


Player::Player(Configuration::Textures avatar_tex_id, const sf::Vector2f& boundaries)
	: Ship(100, Configuration::Textures::PlayerShip), m_Boundaries(boundaries), m_AreActionsBlocked(false),
	m_ExhaustAnimationForward(&Configuration::textures.get(Configuration::Textures::PlayerExhaust)),
	m_ExhaustAnimationBackward(&Configuration::textures.get(Configuration::Textures::PlayerExhaust))
{
	m_AvatarSprite.setTexture(Configuration::textures.get(avatar_tex_id));
	initVariables();
	initWeapons();
	initAnimations();
	
}

void Player::setAreActionsBlocked(bool is_blocked)
{
	m_AreActionsBlocked = is_blocked;
}

void Player::initVariables()
{
	float velocity = 50;
	m_Velocity = sf::Vector2f(velocity * 0.9f, velocity * 0.75f);
	m_Direction = sf::Vector2f(0, 0);
}

void Player::initWeapons()
{
	initWeapon(sf::Vector2f( 60.f, 0.f), .5f, WeaponType::Missile);
	initWeapon(sf::Vector2f(-60.f, 0.f), .5f, WeaponType::Missile);


	initWeapon(sf::Vector2f(-20.f, 0.f),  5.f, WeaponType::Laser);
	initWeapon(sf::Vector2f( 20.f, 0.f),  5.f, WeaponType::Laser);
	initWeapon(sf::Vector2f(-9.f ,-10.f), 5.f, WeaponType::Laser);
	initWeapon(sf::Vector2f( 9.f ,-10.f), 5.f, WeaponType::Laser);


	initWeapon(sf::Vector2f( 0 ,-10.f),   0.1f, WeaponType::Beam);
}

void Player::initWeapon(const sf::Vector2f& offset, float firing_rate, WeaponType weapon_type)
{
	switch (weapon_type) {
		case WeaponType::Laser: {
			std::unique_ptr<LaserTurret> weapon(new LaserTurret(Configuration::Textures::Turret_Laser));
			addWeapon(std::move(weapon));

			break;
		}
		case WeaponType::Missile: {
			std::unique_ptr<MissileTurret> weapon(new MissileTurret(Configuration::Textures::Turret_Laser));
			addWeapon(std::move(weapon));

			break;
		}
		case WeaponType::Beam: {
			std::unique_ptr<BeamTurret> weapon(new BeamTurret(Configuration::Textures::Turret_Laser));
			addWeapon(std::move(weapon));

			break;
		}
	}
	m_Weapons.back()->setPosition(m_Position);
	m_Weapons.back()->setWeaponOffset(offset);
	m_Weapons.back()->setFiringRate(firing_rate);
	m_Weapons.back()->setSpriteRotation(180);
	m_Weapons.back()->setIsWeaponActive(true);
}

void Player::initAnimations()
{
	m_ExhaustAnimationForward.addFramesLine(8, 1, 0, 0, 3);
	m_ExhaustAnimationBackward.addFramesLine(8, 1, 0, 5, 0);

	m_ExhaustAnimatedSpriteLeft.setAnimation(&m_ExhaustAnimationForward);
	m_ExhaustAnimatedSpriteLeft.setLoop(false);
	m_ExhaustAnimatedSpriteLeft.setRepeat(0);
	m_ExhaustAnimatedSpriteLeft.setOrigin(m_ExhaustAnimatedSpriteLeft.getSize().x / 2.f, 0);
	m_ExhaustAnimatedSpriteLeft.setPosition(m_Position);
	m_ExhaustAnimatedSpriteLeft.setOffset(sf::Vector2f(-23.f, 32.f));
	m_ExhaustAnimatedSpriteLeft.setFrame(1);
	m_ExhaustAnimatedSpriteLeft.setScale(1.f, 0.9f);


	m_ExhaustAnimatedSpriteRight = m_ExhaustAnimatedSpriteLeft;
	m_ExhaustAnimatedSpriteRight.setOffset(sf::Vector2f(23.f, 32.f));
}

void Player::update(const sf::Time& deltaTime)
{
	if(!m_AreActionsBlocked)
		updateMovement(deltaTime);
	updateSprites(deltaTime);
	updateWeapons(deltaTime);
}

void Player::updateMovement(const sf::Time& deltaTime)
{
	m_Direction *= 0.97f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		m_Direction.x -= m_Velocity.x;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		m_Direction.x += m_Velocity.x;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		m_Direction.y -= m_Velocity.y;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		m_Direction.y += m_Velocity.y;

#pragma region Movement Boundaries

	// TODO - limit based on sprite size, not based on random nuymbers
	/* Limit movement in X direction, slowly reducing ship velocity */
	if (m_Position.x < 150 && m_Direction.x < 0 || m_Position.x + 150 > m_Boundaries.x && m_Direction.x > 0)
		m_Direction.x *= 0.85f;

	if (m_Position.x < 100 && m_Direction.x < 0 || m_Position.x + 100 > m_Boundaries.x && m_Direction.x > 0)
		m_Direction.x *= 0.65f;

	if (m_Position.x < 50 && m_Direction.x < 0 || m_Position.x + 50 > m_Boundaries.x && m_Direction.x > 0)
		m_Direction.x *= 0.45f;

	if (m_Position.x < 10 && m_Direction.x < 0 || m_Position.x + 10 > m_Boundaries.x && m_Direction.x > 0)
		m_Direction.x *= 0.0f;


	/* Limit movement in Y direction, slowly reducing ship velocity */
	if (m_Position.y > m_Boundaries.y - getSize().y - 10 && m_Direction.y > 0 || m_Position.y < 2.5f * m_Boundaries.y / 4.f + 90.f && m_Direction.y < 0)
		m_Direction.y *= 0.93f;

	if (m_Position.y > m_Boundaries.y - getSize().y + 0 && m_Direction.y > 0 || m_Position.y < 2.5f * m_Boundaries.y / 4.f + 60.f && m_Direction.y < 0)
		m_Direction.y *= 0.75f;

	if (m_Position.y > m_Boundaries.y - getSize().y + 20 && m_Direction.y > 0 || m_Position.y < 2.5f * m_Boundaries.y / 4.f + 30.f && m_Direction.y < 0)
		m_Direction.y *= 0.55f;

	if (m_Position.y > m_Boundaries.y - getSize().y + 40 && m_Direction.y > 0 || m_Position.y < 2.5f * m_Boundaries.y / 4.f + 10.f && m_Direction.y < 0)
		m_Direction.y *= 0.f;

#pragma endregion



	m_Position.x += m_Direction.x * deltaTime.asSeconds();
	m_Position.y += m_Direction.y * deltaTime.asSeconds();

}

void Player::updateSprites(const sf::Time& deltaTime)
{
	m_Sprite.setPosition(m_Position);
	
	for (auto&& weapon : m_Weapons) 
		weapon->setPosition(m_Position);


	// if moving up
	if (m_Direction.y < 0) {
		m_ExhaustAnimatedSpriteLeft.setAnimation(&m_ExhaustAnimationForward);
		m_ExhaustAnimatedSpriteRight.setAnimation(&m_ExhaustAnimationForward);
	}
	// if moving down
	else if (m_Direction.y > 0) {
		m_ExhaustAnimatedSpriteLeft.setAnimation(&m_ExhaustAnimationBackward);
		m_ExhaustAnimatedSpriteRight.setAnimation(&m_ExhaustAnimationBackward);
	}

	m_ExhaustAnimatedSpriteLeft.setPosition(m_Position);
	m_ExhaustAnimatedSpriteRight.setPosition(m_Position);

	m_ExhaustAnimatedSpriteLeft.update(deltaTime);
	m_ExhaustAnimatedSpriteRight.update(deltaTime);

}

void Player::updateWeapons(const sf::Time& deltaTime)
{
	for (auto&& weapon : m_Weapons)
		weapon->update(deltaTime);

	if(!m_AreActionsBlocked)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			shoot();
}
