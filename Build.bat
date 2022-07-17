@echo off

@call "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat"

devenv "D:/Cpp/DirectX12/DX_Portfolio/Game.sln" /rebuild Debug /projectconfig "TestDLL_UPDATE/TestDLL_UPDATE.vcxproj"

echo TestDLL_UPDATE> Rebuild.txt
