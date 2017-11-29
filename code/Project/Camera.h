#pragma once

class Shaders;

class Camera : Noncopyable
{
public:
	Camera(Window *window, const glm::vec3 &pos);

	void Update(float dt);

	glm::mat4 GetViewMatrix();
	
	const glm::vec3& GetPos() const;
	void SetPos(const glm::vec3 &pos);

	void ResizeProjection(int w, int h);
	const glm::mat4& GetPerspectiveProjection() const { return m_perspectiveProjection; }

private:
	static const float MAX_LOOK_PITCH;

	void computeWalkingVectors();
	void controlMouseInput();
	void controlMoving(float dt);
	void controlLooking();
	void computeCameraOrientation();

	Window *m_window;

	glm::mat4 m_perspectiveProjection;

	glm::vec3 m_pos;
	glm::vec3 m_forward;
	glm::vec3 m_side;
	const glm::vec3 m_up;

	glm::vec3 m_cameraLook;

	float m_lookAngleX = 0.0f;
	float m_lookAngleY = 0.0f;
	float m_targetLookAngleX = 0.0f;
	float m_targetLookAngleY = 0.0f;

	double m_oldMouseX;
	double m_oldMouseY;	
};