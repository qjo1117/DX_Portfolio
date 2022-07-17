#pragma once

#include "../Engine/IPlugin.h"

class ActionPlugin :
    public IPlugin
{
public:
	ActionPlugin(PluginManager& manager) : IPlugin(manager) { m_Title = L"ActionPlugin"; }
	virtual ~ActionPlugin() {}

	virtual PLUGINDECL bool Awake() override;
	virtual PLUGINDECL bool Init() override;
	virtual PLUGINDECL bool Update() override;
	virtual PLUGINDECL bool Render() override;
	virtual PLUGINDECL bool End() override;

private:
	Ref<class GameObject> m_light = nullptr;
};

CREATE_PLUGIN(ActionPlugin)