#include "pch.h"
#include "Timer.h"

void Timer::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	/* ----- 하드웨어의 갱신 빈도수를 구한다 ----- */
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_prevCount)); // CPU 클럭
}

void Timer::Update()
{
	/* ----- DeltaTime, Frame를 구해준다. ----- */
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	/* ----- DeltaTime ----- */
	m_deltaTime = (currentCount - m_prevCount) / static_cast<float>(m_frequency);
	m_prevCount = currentCount;

	m_frameCount++;
	m_frameTime += m_deltaTime;

	/* ----- Frame ----- */
	if (m_frameTime > 1.f) {
		m_fps = static_cast<uint32>(m_frameCount / m_frameTime);

		m_frameTime = 0.f;
		m_frameCount = 0;

		//ShowFps();
	}

	/* ----- State ----- */
	switch (m_state) {
	case TIMER_STATE::STOP:
		m_deltaTime = 0.0f;
		break;
	case TIMER_STATE::FAST:
		m_deltaTime *= m_timeScale;
		break;
	case TIMER_STATE::SLOW:
		m_deltaTime /= m_timeScale;
		break;
	}
}

void Timer::ShowFps()
{
	uint32 fps = m_fps;

	WCHAR text[15] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowTextW(m_hWnd, text);
}


/*
	문제
	C++ 11에서 제공하는 Chrono를 쓰지 않고 QueryPerformanceCounter를 쓰는 이유
	ReleaseMode에서는 조금 더 나은 성능을 보여주지만
	DebugMode에서는 QueryPerformanceCounter가 8배 이상 더 좋은 성능을 낸다.
*/