#pragma once
#include "MathUtils.h"

class Camera
{
public:
	Camera();

	void SetPosition(GMath::Vector3f pos);
	void SetPosition(float x, float y, float z);
	void SetRotation(GMath::Vector3f rot);
	void SetRotation(float x, float y, float z);

	void Render();
	void GetViewMatrix(GMath::Matrix4f& matrix);
	GMath::Vector3f GetPosition() const;
	void Move(const GMath::Vector3f& _delta);

private:
	void MatrixRotationYawPitchRoll(GMath::Matrix3f& matrix, float yaw, float pitch, float roll);
	void BuildViewMatrix(GMath::Vector3f pos, GMath::Vector3f lookAt, GMath::Vector3f up);
private:
	GMath::Vector3f m_position;
	GMath::Vector3f m_rotation;

	GMath::Vector3f m_up;
	GMath::Vector3f m_lookAt;

	GMath::Matrix4f m_viewMatrix;
};