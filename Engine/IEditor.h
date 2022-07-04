#pragma once



class IEditor
{
public:
	IEditor() {}
	virtual ~IEditor() {}

	virtual void Init() = 0;
	virtual void Show() = 0;
	virtual void End() {};
	void ShowEditor() { ImGui::Begin(Title.c_str()); Show(); ImGui::End(); }

	/* ------ Property ------ */
	PRIVATE_PROPERTY(string, Title) = typeid(IEditor).name();
};

class MenuInfo : public enable_shared_from_this<MenuInfo>
{
public:
	bool IsMenuItem() { return vecChildMenu.empty(); }
	bool IsMenuBar() { return !vecChildMenu.empty(); }

public:
	/* ------ Property ------ */
	PRIVATE_PROPERTY(string, strTitle);
	PRIVATE_PROPERTY(vector<Ref<MenuInfo>>, vecChildMenu);
	PRIVATE_PROPERTY(function<void(void)>, FuncOn);
};