#pragma once
#include "Model.h"
#include "MathUtils.h"

class Object
{
public:
	Object(Model *_model) :
		m_model(_model),
		m_position(0.0f, 0.0f, 0.0f)
	{}

	__forceinline
	GMath::Vector3f GetPosition() const
	{
		return m_position;
	}

	__forceinline
	Model *const GetModel()
	{
		return m_model.get();
	}

	__forceinline
	void SetPosition(const GMath::Vector3f& _pos)
	{
		m_position = _pos;
	}

	__forceinline
	void Move(const GMath::Vector3f& _delta)
	{
		m_position += _delta;
	}

private:
	std::unique_ptr<Model> m_model;
	GMath::Vector3f        m_position;

};
