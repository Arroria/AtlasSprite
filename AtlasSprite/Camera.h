#pragma once
class Camera
{
private:
	float m_screenRatio;
	float m_viewScale;
	D3DXVECTOR2 m_focus;

public:
	void Update();
	void SetProj();

public:
	Camera();
	~Camera();
};

