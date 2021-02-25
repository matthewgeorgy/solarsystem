@echo off

cl -W4 -Zi ..\src\main.cpp ..\src\glad.c ..\include\imgui\*.cpp ..\lib\glfw3dll.lib ..\lib\glfw3.lib  ..\lib\assimp-vc142-mtd.lib opengl32.lib
