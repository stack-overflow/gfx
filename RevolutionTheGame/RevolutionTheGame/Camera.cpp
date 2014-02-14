#include "Camera.h"

Camera::Camera() :
	m_position(0.0f, 0.0f, 0.0f),
	m_rotation(0.0f, 0.0f, 0.0f)
{}

void Camera::SetPosition(GMath::Vector3f pos)
{
	m_position = pos;
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Camera::SetRotation(GMath::Vector3f rot)
{
	m_rotation = rot;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

void Camera::Render()
{
	//GMath::Vector3f up, pos, lookAt;
	float yaw, pitch, roll;
	GMath::Matrix3f rotationMatrix;

	m_up.x = 0.0f;
	m_up.y = 1.0f;
	m_up.z = 0.0f;

	//m_pos = m_position;

	m_lookAt.x = 0.0f;
	m_lookAt.y = 0.0f;
	m_lookAt.z = 1.0f;

	pitch = m_rotation.x * 0.0174532925f;
	yaw   = m_rotation.y * 0.0174532925f;
	roll  = m_rotation.z * 0.0174532925f;

	MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

	m_lookAt *= rotationMatrix;
	m_up *= rotationMatrix;

	m_lookAt += m_position;

	BuildViewMatrix(m_position, m_lookAt, m_up);
}

void Camera::MatrixRotationYawPitchRoll(
	GMath::Matrix3f& matrix,
	float yaw,
	float pitch,
	float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;

	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	matrix[0] = (cYaw * cRoll) + (sYaw * sPitch * sRoll);
	matrix[1] = cPitch * sRoll;
	matrix[2] = ((-sYaw) * cRoll) + (cYaw * sPitch * sRoll);

	matrix[3] = (-sRoll * cYaw) + (sYaw * sPitch * cRoll);
	matrix[4] = (cPitch * cRoll);
	matrix[5] = (sYaw * sRoll) + (cYaw * sPitch * cRoll);

	matrix[6] = sYaw * cPitch;
	matrix[7] = -sPitch;
	matrix[8] = (cYaw * cPitch);
}

void Camera::BuildViewMatrix(GMath::Vector3f pos, GMath::Vector3f lookAt, GMath::Vector3f up)
{
	GMath::Vector3f zAxis, xAxis, yAxis;
	float length, result1, result2, result3;
	m_lookAt = lookAt;
	m_up = up;

	// zAxis = normal(lookAt - position)
	zAxis.x = lookAt.x - pos.x;
	zAxis.y = lookAt.y - pos.y;
	zAxis.z = lookAt.z - pos.z;
	length = sqrt((zAxis.x * zAxis.x) + (zAxis.y * zAxis.y) + (zAxis.z * zAxis.z));
	zAxis.x = zAxis.x / length;
	zAxis.y = zAxis.y / length;
	zAxis.z = zAxis.z / length;

	// xAxis = normal(cross(up, zAxis))
	xAxis.x = (up.y * zAxis.z) - (up.z * zAxis.y);
	xAxis.y = (up.z * zAxis.x) - (up.x * zAxis.z);
	xAxis.z = (up.x * zAxis.y) - (up.y * zAxis.x);
	length = sqrt((xAxis.x * xAxis.x) + (xAxis.y * xAxis.y) + (xAxis.z * xAxis.z));
	xAxis.x = xAxis.x / length;
	xAxis.y = xAxis.y / length;
	xAxis.z = xAxis.z / length;

	// yAxis = cross(zAxis, xAxis)
	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);

	// -dot(xAxis, position)
	result1 = ((xAxis.x * pos.x) + (xAxis.y * pos.y) + (xAxis.z * pos.z)) * -1.0f;

	// -dot(yaxis, eye)
	result2 = ((yAxis.x * pos.x) + (yAxis.y * pos.y) + (yAxis.z * pos.z)) * -1.0f;

	// -dot(zaxis, eye)
	result3 = ((zAxis.x * pos.x) + (zAxis.y * pos.y) + (zAxis.z * pos.z)) * -1.0f;

	// Set the computed values in the view matrix.
	m_viewMatrix[0]  = xAxis.x;
	m_viewMatrix[1]  = yAxis.x;
	m_viewMatrix[2]  = zAxis.x;
	m_viewMatrix[3]  = 0.0f;

	m_viewMatrix[4]  = xAxis.y;
	m_viewMatrix[5]  = yAxis.y;
	m_viewMatrix[6]  = zAxis.y;
	m_viewMatrix[7]  = 0.0f;

	m_viewMatrix[8]  = xAxis.z;
	m_viewMatrix[9]  = yAxis.z;
	m_viewMatrix[10] = zAxis.z;
	m_viewMatrix[11] = 0.0f;

	m_viewMatrix[12] = result1;
	m_viewMatrix[13] = result2;
	m_viewMatrix[14] = result3;
	m_viewMatrix[15] = 1.0f;
}

void Camera::GetViewMatrix(GMath::Matrix4f& matrix)
{
	matrix[0]  = m_viewMatrix[0];
	matrix[1]  = m_viewMatrix[1];
	matrix[2]  = m_viewMatrix[2];
	matrix[3]  = m_viewMatrix[3];

	matrix[4]  = m_viewMatrix[4];
	matrix[5]  = m_viewMatrix[5];
	matrix[6]  = m_viewMatrix[6];
	matrix[7]  = m_viewMatrix[7];

	matrix[8]  = m_viewMatrix[8];
	matrix[9]  = m_viewMatrix[9];
	matrix[10] = m_viewMatrix[10];
	matrix[11] = m_viewMatrix[11];

	matrix[12] = m_viewMatrix[12];
	matrix[13] = m_viewMatrix[13];
	matrix[14] = m_viewMatrix[14];
	matrix[15] = m_viewMatrix[15];
}

inline void CrossProduct3(GMath::Vector3f result, const GMath::Vector3f u, const GMath::Vector3f v)
{
	result.x = u.y*v.z - v.y*u.z;
	result.y = -u.x*v.z + v.x*u.z;
	result.z = u.x*v.y - v.x*u.y;
}

void Camera::Move(const GMath::Vector3f& _delta)
{
		m_position += _delta;
}

GMath::Vector3f Camera::GetPosition() const
{
		return m_position;
}
