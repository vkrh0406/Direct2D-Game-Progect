#pragma once

// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
// DX Header Files
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <xaudio2.h>
#include "Animation.h"

// 자원 안전 반환 매크로.
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#define FAILED(hr)      (((HRESULT)(hr)) < 0)

class DemoApp
{
public:
	DemoApp();
	~DemoApp();
	HRESULT Initialize(HINSTANCE hInstance);
	void RunMessageLoop();

private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	HRESULT CreateGridPatternBrush(ID2D1RenderTarget* pRenderTarget, ID2D1BitmapBrush** ppBitmapBrush);
	void DiscardDeviceResources();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);

	void DrawAll();
	void AddPositionY(float* y,float* a);


	void selectFillBrush(D2D1_RECT_F &rectangle2,float opacity,int color);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);


public:

	//사운드 관련 함수
	HRESULT FindMediaFileCch(WCHAR* strDestPath, int cchDest, LPCWSTR strFilename);
	HRESULT PlayPCM(IXAudio2* pXaudio2, LPCWSTR szFilename);



private:
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pWICFactory; //이미지 얻기위한 WIC 팩토리
	ID2D1StrokeStyle* m_pStrokeStyleDash;

	ID2D1SolidColorBrush* m_pOriginalShapeBrush;
	ID2D1SolidColorBrush* m_pTransformedShapeBrush;
	ID2D1SolidColorBrush* m_pFillBrush;


	ID2D1SolidColorBrush* m_pFillBrush1;
	ID2D1SolidColorBrush* m_pFillBrush2;
	ID2D1SolidColorBrush* m_pFillBrush3;
	ID2D1SolidColorBrush* m_pFillBrush4;
	ID2D1SolidColorBrush* m_pFillBrush5;
	ID2D1SolidColorBrush* m_pFillBrush6;
	ID2D1SolidColorBrush* m_pFillBrush7;
	ID2D1SolidColorBrush* m_pFillBrush8;


	ID2D1SolidColorBrush* m_pTextBrush;
	ID2D1BitmapBrush* m_pGridPatternBitmapBrush;

	// DirectWrite
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	//BitMap
	ID2D1Bitmap* m_pBitmap;
	ID2D1Bitmap* m_pBitmap_Mountain;
	ID2D1Bitmap* m_pBitmap_Mountains;
	ID2D1Bitmap* m_pBitmap_Trees;
	ID2D1Bitmap* m_pBitmap_Gun;
	ID2D1Bitmap* m_pBitmap_Enemy;
	ID2D1Bitmap* m_pBitmap_Bullet;

	//Animation
	AnimationLinear<float> m_Animation;

	//기하
	ID2D1PathGeometry* m_pPathGeometry;

	//시간계산 변수
	LARGE_INTEGER m_nPrevTime;
	LARGE_INTEGER m_nFrequency;

	//사운드
	IXAudio2* pXAudio2 = NULL;
	IXAudio2MasteringVoice* pMasteringVoice = NULL;
};
