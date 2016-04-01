#pragma once
#include <Windows.h>
#include <GL\gl.h>
WNDCLASSEX wc;
HWND hwnd;
HGLRC glContext;
HDC windowDeviceContext;

char KEYSDOWN[255];
void window_open(int width, int height);
int window_update();
void window_swap();
void window_close();