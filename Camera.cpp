#include "Camera.h"

Vec3 CCamera::GetForward()
{
    float YawRadians = Yaw * DegreesToRadians;
    float PitchRadians = Pitch * DegreesToRadians;

    Vec3 TempForward;

    TempForward.x = cos(PitchRadians) * sin(YawRadians);
    TempForward.y = sin(PitchRadians);
    TempForward.z = -cos(PitchRadians) * cos(YawRadians);

    return TempForward;
}

Vec3 CCamera::GetRight()
{
    float YawRadians = Yaw * DegreesToRadians;

    Vec3 TempRight;
    TempRight.x = cos(YawRadians);
    TempRight.y = 0.0f;
    TempRight.z = sin(YawRadians);

    return TempRight;
}

void CCamera::Rotate(Vec3 Amt)
{
	Pitch += Amt.x;
	Yaw += Amt.y;

	if (Pitch > 89.0f) Pitch = 89.0f;
	if (Pitch < -89.0f) Pitch = -89.0f;
}

void CCamera::Translate(Vec3 Amt)
{
	Vec3 TempForward	 = GetForward();
	Vec3 TempRight = GetRight();

	Position.x += TempForward.x * Amt.z;
	Position.y += TempForward.y * Amt.z;
	Position.z += TempForward.z * Amt.z;

	Position.x += TempRight.x * Amt.x;
	Position.z += TempRight.z * Amt.x;

	Position.y += Amt.y;
}

void CCamera::Update(CEngineDevice* Dev)
{
	Vec3 TempForward = GetForward();

	Dev->Renderer->SetMatrixMode(MatrixMode::MODELVIEW);

	Dev->Renderer->LookAt
	(
		Position.x, Position.y, Position.z,
		Position.x + TempForward.x,
		Position.y + TempForward.y,
		Position.z + TempForward.z,
		0.0f, 1.0f, 0.0f
	);
}