#pragma once
#include "MathUtils.h"

class Light
{
public:
	Light();
	Light(const GMath::Vector4f& _col);
	~Light();

	void SetColor(const GMath::Vector4f& _col);

	GMath::Vector4f GetColor() const;

protected:
	GMath::Vector4f m_color;
};

class PositionLight :
	public Light
{
public:
	PositionLight();
	PositionLight(const GMath::Vector3f& _dir, const GMath::Vector4f& _pos);
	~PositionLight();

	void SetPosition(const GMath::Vector3f& _dir);

	GMath::Vector3f GetPosition() const;

private:
	GMath::Vector3f m_position;
};
