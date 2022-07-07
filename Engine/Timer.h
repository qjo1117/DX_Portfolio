#pragma once

/*----------
	Timer
-----------*/
class Timer
{
	DECLARE_SINGLE(Timer)

public:
	/* ----- External Function ----- */
	void Init(HWND hWnd);
	void Update();

	/* ----- Helper Function ----- */
	uint32	GetFps() { return m_fps; }
	float	GetDeltaTime() { return m_deltaTime; }

	void	SetTimeScale(float timeScale) { m_timeScale = timeScale; }

	/* ----- Time State Function ----- */
	void	Stop() { m_state = TIMER_STATE::STOP; }
	void	Run() { m_state = TIMER_STATE::NONE; }
	void	Slow() { m_state = TIMER_STATE::SLOW; }
	void	Fast() { m_state = TIMER_STATE::FAST; }

public:
	void ShowFps();

private:
	/* ----- Frequency Variable ----- */
	uint64		m_frequency = 0;
	uint64		m_prevCount = 0;
	float		m_deltaTime = 0.0f;

private:
	/* ----- Visiable Frame Variable ----- */
	uint32		m_frameCount = 0;
	float		m_frameTime = 0.0f;
	uint32		m_fps = 0;

private:
	/* ----- 시간이 변할때 쓰는 변수 ----- */
	float		m_timeScale = 2.0f;
	TIMER_STATE	m_state = TIMER_STATE::NONE;

private:
	/* ----- Mapping ----- */
	HWND		m_hWnd = nullptr;
};

