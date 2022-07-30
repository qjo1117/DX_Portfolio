#pragma once



class Scene;


/*-----------------
	SceneManager
------------------*/
class SceneManager
{
private:
	/* ----- SingleTon ----- */
	DECLARE_SINGLE(SceneManager)

public:
	/* ----- External Function ----- */
	void Update();
	void Render();
	void LoadScene(SCENE_TYPE type);

	void End();

	Ref<class GameObject> Pick(int32 screenX, int32 screenY);
	Ref<class GameObject> Pick(int32 screenX, int32 screenY, class RayCastHitInfo& hit);

public:
	/* ----- Template Function ----- */
	template<typename T>
	void CreateScene(SCENE_TYPE type)
	{
		uint32 index = static_cast<uint32>(type);
		assert(m_scenes[static_cast<uint32>(type)] == nullptr);

		Ref<T> scene = make_shared<T>();

		if (m_scenes[index] == nullptr) {
			m_scenes[index] = scene;
		}
	}

public:
	/* ----- Helper Function ----- */
	Ref<Scene>	GetCurrentScene() { return m_currentScene; }
	SCENE_TYPE			GetType() { return m_type; }
private:
	/* ----- Scene Variable ----- */
	Ref<Scene> m_currentScene;
	array<Ref<Scene>, static_cast<uint32>(SCENE_TYPE::END)> m_scenes;
	SCENE_TYPE m_type;
};


/* ----------------------------------------
	TODO :	Load할때 내용물 클리어하는 것
			Change할때 내용물 클리어하는 것
			★File에서 데이터를 로드★
---------------------------------------- */
