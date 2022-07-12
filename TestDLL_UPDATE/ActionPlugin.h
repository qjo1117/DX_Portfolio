#pragma once

#include "../Engine/IPlugin.h"

class ActionPlugin :
    public IPlugin
{
public:
	ActionPlugin(PluginManager& manager) : IPlugin(manager) { }
	virtual ~ActionPlugin() {}

	virtual PLUGINDECL bool Init() override;
	virtual PLUGINDECL bool Update() override;
	virtual PLUGINDECL bool Render() override;
	virtual PLUGINDECL bool End() override;
	virtual PLUGINDECL const std::string& GetName() { return "ActionPlugin"; }

};

CREATE_PLUGIN(ActionPlugin)