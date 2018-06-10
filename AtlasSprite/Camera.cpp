#include "stdafx.h"
#include "Camera.h"



Camera::Camera()
	: m_screenRatio(1280.f / 960.f)
	, m_viewScale(1)
	, m_focus(0, 0)
{
}
Camera::~Camera()
{
}



void Camera::Update()
{
	int mouseWheel = g_inputDevice.MouseWheel() / 120;
	while (mouseWheel)
	{
		if (mouseWheel < 0)
		{
			m_viewScale *= 1.1f;
			mouseWheel++;
		}
		else
		{
			m_viewScale *= 0.9f;
			mouseWheel --;
		}
	}



	POINT mouseDelta = g_inputDevice.MouseDelta();
	if (g_inputDevice.IsKeyPressed(VK_MBUTTON))
	{
		m_focus.x += m_viewScale * 0.001f * mouseDelta.x;
		m_focus.y -= m_viewScale * 0.001f * mouseDelta.y;
	}
}

void Camera::SetProj()
{
	D3DXMATRIX tm;
	D3DXMatrixTranslation(&tm, m_focus.x, m_focus.y, 0);
	DEVICE->SetTransform(D3DTS_PROJECTION, &(tm * MatrixPerspectiveBySprite(m_screenRatio * m_viewScale, m_viewScale)));
}