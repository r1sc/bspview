#pragma once
#include <Windows.h>
#include <GL\gl.h>
WNDCLASSEX wc;
HWND hwnd;
HGLRC glContext;
HDC windowDeviceContext;

void window_open(int width, int height);
int window_update();
void window_swap();
void window_close();