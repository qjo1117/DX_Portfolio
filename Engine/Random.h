#pragma once


// WELL ������� ���� ���� ������
// ���� : ������ �׳��ϱ⿡�� �ɽ��ϰ� ������ ������ ���� ���°� �� ���ٰ� ����� ����
// �ı� : ���θ� ������...

/*------------------
*		Random
------------------*/
class Random
{
private:
	enum SEED {
		DIFFER_VALUE = 100,
	};
public:
	Random() { Init(); }

	void Init() {
		m_index = 0;
		uint64 seed = static_cast<uint64>(time(nullptr));
		for (int32 i = 0; i < 16; ++i) {
			m_state[i] = seed;
			seed += seed + SEED::DIFFER_VALUE;
		}
	}

	void Init(uint64 uSeed) {
		m_index = 0;
		uint64 seed = uSeed;
		for (int32 i = 0; i < 16; ++i) {
			m_state[i] = seed;
			seed += seed + SEED::DIFFER_VALUE;
		}
	}

public:
	uint64 Randint(void) {
		uint64 a = m_state[m_index];
		uint64 c = m_state[(m_index + 13) & 15];
		uint64 b = a ^ c ^ (a << 16) ^ (c << 15);
		c = m_state[(m_index + 9) & 15];
		c ^= (c >> 11);
		a = m_state[m_index] = b ^ c;
		uint64 d = a ^ ((a << 5) & 0xda442d24U);
		m_index = (m_index + 15) & 15;
		a = m_state[m_index];
		m_state[m_index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

		return m_state[m_index];
	}

	uint64 Randint(uint64 min, uint64 max) {
		return min + (Randint() % (max - min));
	}
	float Rand() {
		uint64 value = Randint();
		// ����ü�� �̿��ؼ� float�� �ٲ��ش�.
		union { uint64 u64; float f; } unit;
		unit.u64 = (((value *= 16807) & 0x007fffff) - 1) | 0x3f800000;
		return unit.f - 1.0f;
	}
	float Rand(float min, float max) {
		return min + Rand() * (max - min);
	}

private:
	uint64 m_index = 0;
	uint64 m_state[16] = { 0 };
};
