#pragma once
#include <Windows.h>
#include <GL\gl.h>
WNDCLASSEX wc;
HWND hwnd;
HGLRC glContext;
HDC windowDeviceContext;
BOOL running;

char KEYSDOWN[255];
void window_open(int width, int height);
void window_update();
void window_swap();
void window_close();