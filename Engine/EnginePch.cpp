#include "pch.h"
#include "EnginePch.h"
#include "Engine.h"

unique_ptr<Engine>								GEngine = make_unique<Engine>();
unique_ptr<unordered_map<string, void*>>		GSingle = make_unique<unordered_map<string, void*>>();