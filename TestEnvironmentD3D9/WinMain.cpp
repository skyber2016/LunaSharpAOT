#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "NativeWindow.h"
#include <InitGuid.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8")
IDirect3D9* pD3d = nullptr;
IDirect3DDevice9* pDevice = nullptr;
ID3DXFont* pFont = nullptr;
ID3DXSprite* pSprite = nullptr;
char szDevice[256]{ 0 };


// Khai báo biến toàn cục
LPDIRECTINPUT8 g_pDI = NULL;
LPDIRECTINPUTDEVICE8 g_pKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pMouse = NULL;



// Khởi tạo DirectInput
HRESULT InitDirectInput(HINSTANCE hInstance)
{
	// Tạo đối tượng DirectInput
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDI, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT InitKeyboard(HWND hWnd)
{
	// Tạo thiết bị bàn phím
	if (FAILED(g_pDI->CreateDevice(GUID_SysKeyboard, &g_pKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// Thiết lập định dạng dữ liệu cho thiết bị
	if (FAILED(g_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// Thiết lập cấp độ hợp tác
	if (FAILED(g_pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	// Lấy quyền điều khiển thiết bị
	g_pKeyboard->Acquire();

	return S_OK;
}

HRESULT InitMouse(HWND hWnd)
{
	// Tạo thiết bị chuột
	if (FAILED(g_pDI->CreateDevice(GUID_SysMouse, &g_pMouse, NULL)))
	{
		return E_FAIL;
	}

	// Thiết lập định dạng dữ liệu cho thiết bị
	if (FAILED(g_pMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// Thiết lập cấp độ hợp tác
	if (FAILED(g_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	// Lấy quyền điều khiển thiết bị
	g_pMouse->Acquire();

	return S_OK;
}

BYTE diKeys[256];

void ReadKeyboard()
{
	HRESULT hr;
	hr = g_pKeyboard->GetDeviceState(sizeof(diKeys), (LPVOID)&diKeys);
	if (FAILED(hr))
	{
		// Nếu thiết bị bị mất hoặc không được lấy quyền, lấy lại quyền điều khiển
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			g_pKeyboard->Acquire();
		}
	}
}

DIMOUSESTATE2 mouseState;

void ReadMouse()
{
	HRESULT hr;
	hr = g_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	if (FAILED(hr))
	{
		// Nếu thiết bị bị mất hoặc không được lấy quyền, lấy lại quyền điều khiển
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			g_pMouse->Acquire();
		}
	}
}

void CleanupDirectInput()
{
	if (g_pKeyboard)
	{
		g_pKeyboard->Unacquire();
		g_pKeyboard->Release();
		g_pKeyboard = NULL;
	}

	if (g_pMouse)
	{
		g_pMouse->Unacquire();
		g_pMouse->Release();
		g_pMouse = NULL;
	}

	if (g_pDI)
	{
		g_pDI->Release();
		g_pDI = NULL;
	}
}


bool InitD3D(HWND hWnd, UINT uWidth, UINT uHeight)
{
	pD3d = Direct3DCreate9( D3D_SDK_VERSION );
	D3DPRESENT_PARAMETERS dp{ 0 };
	dp.BackBufferWidth = uWidth;
	dp.BackBufferHeight = uHeight;
	dp.hDeviceWindow = hWnd;
	dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dp.Windowed = TRUE;

	HRESULT hr = pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dp, &pDevice );
	if (FAILED( hr ) || !pDevice)
	{
		MessageBox( NULL, _T( "Failed to create D3D Device." ), _T( "Error" ), MB_ICONERROR | MB_OK );
		return false;
	}

	sprintf_s( szDevice, 256, "DevicePtr: 0x%p", &pDevice );

	// Create font to draw string
	hr = D3DXCreateFont( pDevice, 21, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T( "Consolas" ), &pFont );
	if (FAILED( hr ))
	{
		MessageBox( NULL, _T( "Failed to create font." ), _T( "Error" ), MB_ICONERROR | MB_OK );
		return false;
	}

	hr = D3DXCreateSprite( pDevice, &pSprite );
	if (FAILED( hr ))
	{
		MessageBox( NULL, _T( "Failed to create sprite." ), _T( "Error" ), MB_ICONERROR | MB_OK );
		return false;
	}

	return true;
}

void Render(NativeWindow& wnd)
{
	pDevice->BeginScene();
	pDevice->Clear( 1, nullptr, D3DCLEAR_TARGET, 0x00111111, 1.0f, 0 );

	RECT rc = { 0, 0, WND_WIDTH, WND_HEIGHT };
	pSprite->Begin( D3DXSPRITE_ALPHABLEND );
	pFont->DrawTextA( pSprite, szDevice, strlen(szDevice), &rc, DT_NOCLIP, 0xFFFFFFFF );
	pSprite->End();

	pDevice->EndScene();
	pDevice->Present( 0, 0, 0, 0 );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	NativeWindow wnd;

	wnd.Create( hInstance, nCmdShow );
	auto hWnd = wnd.GetHandle();
	if (!InitD3D(hWnd, WND_WIDTH, WND_HEIGHT ))
		return 1;
	// Khởi tạo Direct3D, DirectInput, và các thành phần khác
	if (FAILED(InitDirectInput(hInstance)))
	{
		return 0;
	}

	if (FAILED(InitKeyboard(hWnd)) || FAILED(InitMouse(hWnd)))
	{
		return 0;
	}
	MSG m;
	while (true)
	{
		while (PeekMessage( &m, NULL, 0, 0, PM_REMOVE ) && m.message != WM_QUIT)
		{
			TranslateMessage( &m );
			DispatchMessage( &m );
		}
		if (m.message == WM_QUIT)
			break;

		Render(wnd);
		// Đọc trạng thái đầu vào
		ReadKeyboard();
		ReadMouse();
	}

	// Dọn dẹp tài nguyên
	CleanupDirectInput();

	return 0;
}