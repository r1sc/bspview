#include "window.h"

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		{
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
				PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
				32,                        //Colordepth of the framebuffer.
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,                        //Number of bits for the depthbuffer
				0,                        //Number of bits for the stencilbuffer
				0,                        //Number of Aux buffers in the framebuffer.
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};

			windowDeviceContext = GetDC(hwnd);

			int  letWindowsChooseThisPixelFormat;
			letWindowsChooseThisPixelFormat = ChoosePixelFormat(windowDeviceContext, &pfd);
			SetPixelFormat(windowDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

			glContext = wglCreateContext(windowDeviceContext);
			wglMakeCurrent(windowDeviceContext, glContext);			
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(glContext);
		running = FALSE;
		PostQuitMessage(0);		
		break;
	case WM_KEYDOWN:
		KEYSDOWN[wParam] = TRUE;
		break;
	case WM_KEYUP:
		KEYSDOWN[wParam] = FALSE;
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

const char* windowClassName = "bspviewWindowClass";
void window_open(int width, int height)
{	
	for (int i = 0; i < 255; i++)
	{
		KEYSDOWN[i] = FALSE;
	}

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		windowClassName,
		"bspview",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, NULL, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		//return 0;
	}

	ShowWindow(hwnd, 1);
	UpdateWindow(hwnd);
	running = TRUE;
}

MSG msg;
void window_update() {
	int messageAvailable = PeekMessage(&msg, NULL, 0, 0, 1) > 0;
	if(messageAvailable)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void window_swap() {
	SwapBuffers(windowDeviceContext);
}