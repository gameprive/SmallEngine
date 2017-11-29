#include "stdafx.h"
#include "Camera.h"

//-----------------------------------------------------------------------
const float Camera::MAX_LOOK_PITCH = PI_2 - 0.2f;
//-----------------------------------------------------------------------
Camera::Camera(Window *window, const glm::vec3 &pos)
	: m_window(window)
	, m_pos(pos)
	, m_up(0.0, 1.0, 0.0)
	, m_cameraLook(0.0, 0.0, 0.0)
{
	computeWalkingVectors();
	m_window->GetMousePos(m_oldMouseX, m_oldMouseY);
}
//-----------------------------------------------------------------------
void Camera::Update(float dt)
{
	controlMouseInput();
	computeWalkingVectors();
	controlMoving(dt);
	controlLooking();
	computeCameraOrientation();
}
//-----------------------------------------------------------------------
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_pos, m_cameraLook, glm::vec3(0.0, 1.0, 0.0));
}
//-----------------------------------------------------------------------
const glm::vec3& Camera::GetPos() const
{
	return m_pos;
}
//-----------------------------------------------------------------------
void Camera::SetPos(const glm::vec3 &pos)
{
	m_pos = pos;
}
//-----------------------------------------------------------------------
void Camera::ResizeProjection(int w, int h)
{
	const float FOV = 45.0f;
	const float ASPECT_RATIO = (float)w / (float)h;
	const float PERSPECTIVE_NEAR_RANGE = 0.1f;
	const float PERSPECTIVE_FAR_RANGE = 8000.0f;
	m_perspectiveProjection = glm::perspective(FOV, ASPECT_RATIO, PERSPECTIVE_NEAR_RANGE, PERSPECTIVE_FAR_RANGE);
}
//-----------------------------------------------------------------------
void Camera::computeWalkingVectors()
{
	m_forward = glm::rotate(glm::vec3(0.0, 0.0, -1.0), m_lookAngleY, m_up);
	m_side = glm::normalize(glm::cross(m_forward, m_up));
}
//-----------------------------------------------------------------------
void Camera::controlMouseInput()
{
	double mouseX, mouseY;
	m_window->GetMousePos(mouseX, mouseY);

	// compute a smooth look direction based on the mouse motion
	m_targetLookAngleX -= (mouseY - m_oldMouseY) * 0.002;
	m_targetLookAngleY -= (mouseX - m_oldMouseX) * 0.002;
	if ( m_targetLookAngleX > MAX_LOOK_PITCH ) m_targetLookAngleX = MAX_LOOK_PITCH;
	if ( m_targetLookAngleX < -MAX_LOOK_PITCH ) m_targetLookAngleX = -MAX_LOOK_PITCH;

	// track our old mouse position for mouse movement calculations next frame
	m_oldMouseX = mouseX;
	m_oldMouseY = mouseY;
}
//-----------------------------------------------------------------------
void Camera::controlMoving(float dt)
{
	glm::vec3 targetVelocity(0.0);
	if ( m_window->GetKey('W') )
		targetVelocity += glm::vec3(0.0, 0.0, MOVE_SPEED);
	if ( m_window->GetKey('A') )
		targetVelocity += glm::vec3(-MOVE_SPEED, 0.0, 0.0);
	if ( m_window->GetKey('S') )
		targetVelocity += glm::vec3(0.0, 0.0, -MOVE_SPEED);
	if ( m_window->GetKey('D') )
		targetVelocity += glm::vec3(MOVE_SPEED, 0.0, 0.0);

	m_pos = m_pos + (m_forward * targetVelocity.z * dt) + (m_side * targetVelocity.x * dt);
}
//-----------------------------------------------------------------------
void Camera::controlLooking()
{
	// compute our pitch angle, taking into account any impact animations
	m_lookAngleX = m_lookAngleX + (m_targetLookAngleX - m_lookAngleX) * 0.8f;
	if ( m_lookAngleX > MAX_LOOK_PITCH ) m_lookAngleX = MAX_LOOK_PITCH;
	if ( m_lookAngleX < -MAX_LOOK_PITCH ) m_lookAngleX = -MAX_LOOK_PITCH;

	// y-look direction is simpler
	m_lookAngleY = m_lookAngleY + (m_targetLookAngleY - m_lookAngleY) * 0.8f;
}
//-----------------------------------------------------------------------
void Camera::computeCameraOrientation()
{
	glm::mat4 camera = glm::mat4(1.0);
	camera = glm::rotate(camera, m_lookAngleY, glm::vec3(0.0, 1.0, 0.0));
	camera = glm::rotate(camera, m_lookAngleX, glm::vec3(1.0, 0.0, 0.0));

	m_cameraLook = m_pos - glm::vec3(camera[2]);
}
//-----------------------------------------------------------------------