#pragma once
#include "Component.h"

/*------------------
	MonoBehaviour
-------------------*/
class  MonoBehaviour abstract : public Component
{
public:
	/* ----- Constructor Function ----- */
	MonoBehaviour();
	virtual ~MonoBehaviour();

	GENERATE_CLASS_TYPE_INFO(MonoBehaviour);

public:
	/* ----- virtual Function ----- */
	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() sealed { }

	/* ----- GUI Function ----- */
	virtual void EditorUpdate() {
		if (ImGui::CollapsingHeader(&GetTypeInfo().GetTypeName().data()[6]) == true) {

			vector<const Property*> propertys = GetTypeInfo().GetProperties();

			for (const Property* variable : propertys) {
				string typeName = variable->GetTypeInfo().GetTypeName();

				if (typeName == "float") {
					float& temp = variable->Get<float>(this);
					ImGui::DragFloat(variable->GetName(), &temp, 0.1f, -1000.0f, 1000.0f);
				}
				else if (typeName == "int") {
					int32& temp = variable->Get<int32>(this);
					ImGui::DragInt(variable->GetName(), &temp, 1, -1000, 1000);
				}
				else if (typeName == "bool") {
					bool& temp = variable->Get<bool>(this);
					ImGui::Checkbox(variable->GetName(), &temp);
				}
				else if (typeName.find("shared_ptr") != string::npos) {
					if (ImGui::Button(variable->GetName())) {
						// TODO : 현재(씬) 있는 녀석들을 나열해야한다.
					}
				}
			}
		}
		
	}

};

