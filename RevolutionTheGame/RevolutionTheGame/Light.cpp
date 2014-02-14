#include "Light.h"

Light::Light() :
	m_color()
{}

Light::Light(const GMath::Vector4f& _col) :
	m_color(_col)
{}

Light::~Light()
{}

void Light::SetColor(const GMath::Vector4f& _col)
{
	m_color = _col;
}

GMath::Vector4f Light::GetColor()  const
{
	return m_color;
}

PositionLight::PositionLight() :
	Light()
{}

PositionLight::~PositionLight()
{}

PositionLight::PositionLight(const GMath::Vector3f& _pos, const GMath::Vector4f& _col) :
	Light(_col),
	m_position(_pos)
{}

void PositionLight::SetPosition(const GMath::Vector3f& _dir)
{
	m_position = _dir;
}

GMath::Vector3f PositionLight::GetPosition() const
{
	return m_position;
}
