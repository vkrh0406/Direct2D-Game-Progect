#include "Hw2.h"
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "Bullet.h"




int gamePoint; // ǥ���� ���� ����
std::vector<Bullet> bullets;
int vectorsize = 0; //���� ������
clock_t cooltime_start, cooltime_end;
float enemy_spawn_cooltime = 2.0f;

bool isClick;//��� �߰��� ���簢�� Ŭ�� ���� true�϶� Ŭ��
bool isClick2;  //������ ���� Ŭ�� ����
float mouse_current_x, mouse_current_y; // ���� ���콺 ��ǥ��
float angle, scale = 0;; // ȸ����, ������ ��
int downY = 600; // ���� ���� �ʱ� Y��
int currentY = downY; //���� �ذ�

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	srand(time(NULL));
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			DemoApp app;

			if (SUCCEEDED(app.Initialize(hInstance)))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

DemoApp::DemoApp() :

	m_hwnd(NULL),
	m_pD2DFactory(NULL),
	m_pRenderTarget(NULL),
	m_pStrokeStyleDash(NULL),
	m_pGridPatternBitmapBrush(NULL),
	m_pOriginalShapeBrush(NULL),
	m_pTransformedShapeBrush(NULL),
	m_pFillBrush(NULL),


	m_pFillBrush1(NULL),
	m_pFillBrush2(NULL),
	m_pFillBrush3(NULL),
	m_pFillBrush4(NULL),
	m_pFillBrush5(NULL),
	m_pFillBrush6(NULL),
	m_pFillBrush7(NULL),
	m_pFillBrush8(NULL),


	m_pTextBrush(NULL),
	m_pDWriteFactory(NULL),
	m_pTextFormat(NULL),
	m_Animation(),
	m_pBitmap(NULL),
	m_pBitmap_Mountain(NULL),
	m_pBitmap_Mountains(NULL),
	m_pBitmap_Gun(NULL),
	m_pBitmap_Enemy(NULL),
	m_pBitmap_Bullet(NULL),
	m_pBitmap_Trees(NULL),
	m_pPathGeometry(NULL)
{
}

DemoApp::~DemoApp()
{
	SAFE_RELEASE(m_pD2DFactory);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pStrokeStyleDash);
	SAFE_RELEASE(m_pGridPatternBitmapBrush);
	SAFE_RELEASE(m_pOriginalShapeBrush);
	SAFE_RELEASE(m_pTransformedShapeBrush);
	SAFE_RELEASE(m_pFillBrush);
	SAFE_RELEASE(m_pTextBrush);
	SAFE_RELEASE(m_pDWriteFactory);
	SAFE_RELEASE(m_pTextFormat);
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pBitmap_Mountain);
	SAFE_RELEASE(m_pBitmap_Mountains);
	SAFE_RELEASE(m_pBitmap_Trees);
	SAFE_RELEASE(m_pBitmap_Gun);
	SAFE_RELEASE(m_pBitmap_Enemy);
	SAFE_RELEASE(m_pBitmap_Bullet);
}

HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr;

	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.lpszClassName = L"D2DDemoApp";
		RegisterClassEx(&wcex);

		m_hwnd = CreateWindow(
			L"D2DDemoApp", L"Direct2D Demo Application",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			800, 600, NULL, NULL, hInstance, this);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			float length = 0;
			hr = m_pPathGeometry->ComputeLength(NULL, &length);

			if (SUCCEEDED(hr))
			{
				m_Animation.SetStart(0); //start at beginning of path
				m_Animation.SetEnd(length); //length at end of path
				m_Animation.SetDuration(4.0f); //seconds

				ShowWindow(m_hwnd, SW_SHOWNORMAL);
				UpdateWindow(m_hwnd);
			}
		}
	}
	QueryPerformanceFrequency(&m_nFrequency);
	QueryPerformanceCounter(&m_nPrevTime);

	return hr;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	ID2D1GeometrySink* pSink = NULL;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
	}

	// ������ �ﰢ�� ����� ��� ���ϸ� ������.
	if (SUCCEEDED(hr))
	{
		hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pPathGeometry->Open(&pSink);
	}
	if (SUCCEEDED(hr))
	{
		pSink->BeginFigure(D2D1::Point2F(0.0f, 0.0f), D2D1_FIGURE_BEGIN_FILLED);

		const D2D1_POINT_2F ptLines[] = { {0.0f, 0.0f}, {800.0f, 0.0f}, {1600.0f, 0.0f} };
		pSink->AddLines(ptLines, 3);

		pSink->EndFigure(D2D1_FIGURE_END_OPEN);

		hr = pSink->Close();
	}

	

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
			);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			13.0f,
			L"en-us",
			&m_pTextFormat
			);
	}

	if (SUCCEEDED(hr))
	{
		float dashes[] = {10.0f, 2.0f};
		hr = m_pD2DFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_CUSTOM,
				0.0f),
			dashes,
			ARRAYSIZE(dashes),
			&m_pStrokeStyleDash
			);
	}
	SAFE_RELEASE(pSink);

	return hr;
}

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);

		
		// �ܺ� ���Ϸκ��� ��Ʈ�� ��ü�� ������.
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pWICFactory, L".\\background2.png", 100, 0, &m_pBitmap);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pWICFactory, L".\\Mountain.png", 100, 0, &m_pBitmap_Mountain);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pWICFactory, L".\\Mountains.png", 100, 0, &m_pBitmap_Mountains);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pWICFactory, L".\\Trees.png", 100, 0, &m_pBitmap_Trees);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pWICFactory, L".\\Gun.png", 100, 0, &m_pBitmap_Gun);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pWICFactory, L".\\Enemy.png", 100, 0, &m_pBitmap_Enemy);
		}if (SUCCEEDED(hr))
		{
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pWICFactory, L".\\Bullet.png", 100, 0, &m_pBitmap_Bullet);
		}

		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ����� �ܰ����� ���� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &m_pOriginalShapeBrush);
		}

		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ������ ä��� �뵵�� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pFillBrush);
		}
		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ������ ä��� �뵵�� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &m_pFillBrush1);
		}

		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ������ ä��� �뵵�� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pFillBrush2);
		}
		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ������ ä��� �뵵�� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pFillBrush3);
		}
		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ������ ä��� �뵵�� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &m_pFillBrush4);
		}
		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ������ ä��� �뵵�� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkRed), &m_pFillBrush5);
		}

		if (SUCCEEDED(hr))
		{
			// �ؽ�Ʈ�� ���� ���� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pTextBrush);
		}

		if (SUCCEEDED(hr))
		{
			// ��ȯ �� ����� �ܰ����� ���� ���� ����.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &m_pTransformedShapeBrush);
		}

		

		if (SUCCEEDED(hr))
		{
		   hr = CreateGridPatternBrush(m_pRenderTarget, &m_pGridPatternBitmapBrush);
		}
	}

	return hr;
}

HRESULT DemoApp::CreateGridPatternBrush(ID2D1RenderTarget* pRenderTarget, ID2D1BitmapBrush** ppBitmapBrush)
{
	HRESULT hr = S_OK;

	// Create a compatible render target.
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	hr = pRenderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(10.0f, 10.0f), &pCompatibleRenderTarget);

	if (SUCCEEDED(hr))
	{
		// Draw a pattern.
		ID2D1SolidColorBrush* pGridBrush = NULL;
		hr = pCompatibleRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.93f, 0.94f, 0.96f, 1.0f), &pGridBrush);

		if (SUCCEEDED(hr))
		{
			pCompatibleRenderTarget->BeginDraw();
			pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 10.0f, 1.0f), pGridBrush);
			pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 1.0f, 10.0f), pGridBrush);
			hr = pCompatibleRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				// ��� ��ġ ������ �ڿ����� �ݳ�.
				DiscardDeviceResources();
			}
			
			if (SUCCEEDED(hr))
			{
				// ����Ÿ�����κ��� ��Ʈ���� ����.
				ID2D1Bitmap* pGridBitmap = NULL;
				hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);

				if (SUCCEEDED(hr))
				{
					// ��Ʈ�� ���� ������.
					hr = m_pRenderTarget->CreateBitmapBrush(
						pGridBitmap,
						D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP),
						ppBitmapBrush
						);

					pGridBitmap->Release();
				}
			}

			pGridBrush->Release();
		}

		pCompatibleRenderTarget->Release();
	}

	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pTransformedShapeBrush);
	SAFE_RELEASE(m_pFillBrush);
	SAFE_RELEASE(m_pFillBrush1);
	SAFE_RELEASE(m_pFillBrush2);
	SAFE_RELEASE(m_pFillBrush3);
	SAFE_RELEASE(m_pFillBrush4);
	SAFE_RELEASE(m_pFillBrush5);
	SAFE_RELEASE(m_pFillBrush6);
	SAFE_RELEASE(m_pFillBrush7);
	SAFE_RELEASE(m_pFillBrush8);
	SAFE_RELEASE(m_pOriginalShapeBrush);
	SAFE_RELEASE(m_pTextBrush);
	SAFE_RELEASE(m_pGridPatternBitmapBrush);
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pBitmap_Mountain);
	SAFE_RELEASE(m_pBitmap_Mountains);
	SAFE_RELEASE(m_pBitmap_Trees);
}

void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DemoApp::OnRender()
{
	HRESULT hr = CreateDeviceResources();
	WCHAR text[100];


	D2D1_POINT_2F point;
	D2D1_POINT_2F tangent;

	D2D1_MATRIX_3X2_F triangleMatrix;
	D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
	float minWidthHeightScale = min(rtSize.width, rtSize.height) / 512;

	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(minWidthHeightScale, minWidthHeightScale);

	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(-800.0f ,0.0f );


	if (SUCCEEDED(hr))
	{
		// �׸��⸦ ������.
		m_pRenderTarget->BeginDraw();

		// ����Ÿ�� ��ȯ�� �׵� ��ȯ���� ������.
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		// ����Ÿ�� ������ Ŭ������.
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		m_pRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height), m_pGridPatternBitmapBrush);

		//�׸��� ����

		//��� 1
		D2D1_SIZE_F size = m_pBitmap->GetSize();
		m_pRenderTarget->DrawBitmap(m_pBitmap, D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height));
		//��� 2 (��)
		m_pRenderTarget->DrawBitmap(m_pBitmap_Mountain, D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height));
		
		m_pRenderTarget->DrawBitmap(m_pBitmap_Mountains, D2D1::RectF(0.0f, rtSize.height / 2, rtSize.width, rtSize.height));
		//�ִϸ��̼�

		// ����Ÿ�� ��ȯ�� �׵� ��ȯ���� ������.
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		
		
		
		
		static float anim_time = 0.0f;

		float length = m_Animation.GetValue(anim_time);

		// ���� �ð��� �ش��ϴ� ���� ���̿� ��ġ�ϴ� �̵� ���� ���� ������ ����.
		m_pPathGeometry->ComputePointAtLength(length, NULL, &point, &tangent);

		// �ﰢ���� ������ �����Ͽ� �̵� ������ ���󰡴� ������ �ǵ��� ��.
		triangleMatrix = D2D1::Matrix3x2F(
			tangent.x, tangent.y,
			-tangent.y, tangent.x,
			point.x, point.y);

		m_pRenderTarget->SetTransform(triangleMatrix * scale * translation);
		//��� 3 ��Ʈ��
		
		m_pRenderTarget->DrawBitmap(m_pBitmap_Trees, D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height));

		m_pRenderTarget->SetTransform(translation);
		
		// ����Ÿ�� ��ȯ�� �׵� ��ȯ���� ������.
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		/*m_pOriginalShapeBrush->SetOpacity(0.5);
		m_pFillBrush->SetOpacity(0.5f);*/

	
		//DrawAll();

		
		// �÷��̾��� ���콺�� �� ����

		m_pRenderTarget->DrawBitmap(m_pBitmap_Gun, D2D1::RectF(mouse_current_x-80.0f, mouse_current_y-80.0f, mouse_current_x+80.0f, mouse_current_y + 80.0f));

		//�� ����
		m_pRenderTarget->DrawBitmap(m_pBitmap_Enemy, D2D1::RectF(10.0f,250.0f,150.0f, 400.0f));

		std::vector<Bullet>::iterator iter;

		int i = 0;
		//�Ѿ� ���� �� �浹 ����
		for (iter=bullets.begin();iter!=bullets.end();)
		{
			Bullet temp = *iter;
			
			float x = temp.x;
			float y = temp.y;
			float size = temp.size;
			float translation_size = temp.translation_size;
			
			m_pRenderTarget->DrawBitmap(m_pBitmap_Bullet, D2D1::RectF(x, y, x+size, y+size-50));

			


			bullets.at(i).x = x - 5.0f;

			if (x >= 10.0f && x <= 150.0f && y >= 250.0f && y <= 400.0f) //�浹 Ȯ��
			{
				gamePoint += 100;

				iter=bullets.erase(iter);
			}
			else {
				i++;
				iter++;
			}
			
			
		}




		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		
		_swprintf(text, L"���콺x:%f\n���콺y:%f\n ����: %d \n", mouse_current_x, mouse_current_y,gamePoint);

		m_pRenderTarget->DrawText(text, wcslen(text), m_pTextFormat, D2D1::RectF(10.0f, 10.0f, 150.0f, 150.0f), m_pTextBrush);

		



		// �׸��� ������� ������.
		hr = m_pRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}


		// �ִϸ��̼��� ���� �����ϸ� �ٽ� ó������ �ǵ����� �ݺ��ǵ��� ��.
		if (anim_time >= m_Animation.GetDuration())
		{
			anim_time = 0.0f;
		}
		else
		{
			LARGE_INTEGER CurrentTime;
			QueryPerformanceCounter(&CurrentTime);

			float elapsedTime = (float)((double)(CurrentTime.QuadPart - m_nPrevTime.QuadPart) / (double)(m_nFrequency.QuadPart));
			m_nPrevTime = CurrentTime;

			anim_time += elapsedTime;
		}
	}

	return hr;
}

//void DemoApp::DrawAll()
//{
//	float x = 560.0f;  //�ʱ� �ڽ��� ��ġ�� ��ǥ x y
//	float y = 50.0f;
//	WCHAR text[100];
//
//	if (!isClick) { // ��� �߾��� ���簢���� ���� Ŭ�� �ȵ��� ��
//	D2D1_RECT_F rectangle = D2D1::Rect(x, y, x + 70.0f, y + 70.0f);
//
//	// ��ȯ �� �簢���� �׸�.
//	m_pFillBrush1->SetOpacity(0.5f);
//	m_pRenderTarget->FillRectangle(rectangle, m_pFillBrush1);
//	m_pRenderTarget->DrawRectangle(rectangle, m_pTransformedShapeBrush);
//	}
//	else //���簢���� Ŭ�� �ǰ��� ������ �߰��ϴ� ����
//	{
//		float diffY = (currentY - current_y) / (currentY - 60.0f);// ���� �����Ϳ� �Ÿ� /��ü�Ÿ� �󸶸�ŭ ������ �ִ°� ���
//
//
//		D2D1_RECT_F rectangle1 = D2D1::Rect(current_x-35.0f, current_y-35.0f, current_x+35.0f, current_y + 35.0f);
//		angle = 360-360 * diffY;// ȸ����
//		scale = 2.0f* (1 - diffY) + 1.0f;//������
//
//		
//		D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(current_x,current_y));
//		D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(D2D1::Size(2.0f* (1 - diffY)+1.0f,1.0f), D2D1::Point2F(current_x, current_y));
//
//		m_pRenderTarget->SetTransform(scale*rotation);
//		//selectFillBrush(rectangle1, 0.5f, temp_std.color);
//		m_pRenderTarget->DrawRectangle(rectangle1, m_pOriginalShapeBrush);
//
//		//�л� ����ü ������ �ؽ�Ʈ �߰�
//		WCHAR text[100];
//		//_swprintf(text, L" %s  ,  %d", temp_std.name.c_str(),temp_std.jumsu);
//
//		m_pRenderTarget->DrawText(text, wcslen(text), m_pTextFormat, D2D1::RectF(current_x -35.0f, current_y - 15.0f, current_x + 35.0f, current_y + 35.0f), m_pTextBrush);
//
//
//
//		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
//		//Ŭ�� ������ �ڽ� �̸� �����ֱ�
//		D2D1_RECT_F rectangle2 = D2D1::Rect(465, currentY -35, 745, currentY +35 );
//		m_pRenderTarget->DrawRectangle(rectangle2, m_pOriginalShapeBrush, 1.5f, m_pStrokeStyleDash);
//		float opacity = 0.5f;
//		//selectFillBrush(rectangle2,opacity,temp_std.color);
//		//_swprintf(text, L" %s  ,  %d", temp_std.name.c_str(), temp_std.jumsu);
//		m_pRenderTarget->DrawText(text, wcslen(text), m_pTextFormat, D2D1::RectF(550, currentY - 35, 745, currentY + 35), m_pTextBrush);
//		
//	}
//	
//	if (isClick2)  //���� Ŭ�� ���� Ȯ��
//	{
//		float diffY = (currentY - current_y) / (currentY - 60.0f);// ���� �����Ϳ� �Ÿ� /��ü�Ÿ� �󸶸�ŭ ������ �ִ°� ���
//
//
//		D2D1_RECT_F rectangle1 = D2D1::Rect(current_x - 35.0f, current_y - 35.0f, current_x + 35.0f, current_y + 35.0f);
//		angle = 360 - 360 * diffY;//����
//		scale = 2.0f* (1 - diffY) + 1.0f;//������
//
//		D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(current_x, current_y));
//		D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(D2D1::Size(2.0f* (1 - diffY) + 1.0f, 1.0f), D2D1::Point2F(current_x, current_y));
//
//		m_pRenderTarget->SetTransform(scale*rotation);
//		//selectFillBrush(rectangle1, 0.5f, temp_std.color);
//		m_pRenderTarget->DrawRectangle(rectangle1, m_pOriginalShapeBrush);
//
//	
//		//_swprintf(text, L" %s  ,  %d", temp_std.name.c_str(), temp_std.jumsu);
//		m_pRenderTarget->DrawText(text, wcslen(text), m_pTextFormat, D2D1::RectF(current_x - 35.0f, current_y - 35.0f, current_x + 35.0f, current_y + 35.0f), m_pTextBrush);
//	}
//
//
//
//	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
//
//
//	int rectangle_y = downY; //��� ������ ������
//	
//
//
//
//}

void DemoApp::selectFillBrush(D2D1_RECT_F &rectangle2, float opacity,int color)
{
	switch (color) {
	case 1:
		m_pFillBrush->SetOpacity(opacity);
		m_pRenderTarget->FillRectangle(rectangle2, m_pFillBrush5);
		break;
	case 2:
		m_pFillBrush1->SetOpacity(opacity);
		m_pRenderTarget->FillRectangle(rectangle2, m_pFillBrush1);
		break;
	case 3:
		m_pFillBrush2->SetOpacity(opacity);
		m_pRenderTarget->FillRectangle(rectangle2, m_pFillBrush2);
		break;
	case 4:
		m_pFillBrush3->SetOpacity(opacity);
		m_pRenderTarget->FillRectangle(rectangle2, m_pFillBrush3);
		break;
	case 5:
		m_pFillBrush4->SetOpacity(opacity);
		m_pRenderTarget->FillRectangle(rectangle2, m_pFillBrush4);
		break;

	}
}



void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));

		return 1;
	}

	DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

	if (pDemoApp)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
		
			
			mouse_current_x = LOWORD(lParam);
			mouse_current_y = HIWORD(lParam);

		
			bullets.push_back(Bullet(mouse_current_x, mouse_current_y));
			//// Ư�� ��ǥ���� Ŭ������ Ȯ���� (���� �ڽ�)
			//if (x >= 560 && x <= 630 && y >= 50 && y <= 120 && vectorsize<=7)
			//{
			//	isClick = true;
			//	

			//}
			//if (x >= 465 && x <= 745 && y >= currentY+35 && y <= currentY+105) //������ �� ���� �ڽ� Ŭ�� Ȯ��
			//{
			//	isClick2 = true;
			//

			//}


			InvalidateRect(hwnd, NULL, false);
			break;
		case WM_LBUTTONUP:
		
			mouse_current_x = LOWORD(lParam);
			mouse_current_y = HIWORD(lParam);

			//if (current_x >= 465 && current_x <= 745 && current_y >= currentY-45 && current_y<= currentY + 45 && isClick==true)
			//{ // ������ �߰� �巡�׾� ��� ����
			//	
			//	

			//}
			//if (current_x >= 560 && current_x <= 630 && current_y >= 50 && current_y <= 120 && isClick2 == true)
			//{ 
			//}
			//else if (isClick2 == true) //������ ���� ����
			//{
			//	
			//}
			//isClick = false;
			//isClick2 = false;

			InvalidateRect(hwnd, NULL, false);
			break;
		case WM_MOUSEMOVE:
			mouse_current_x = LOWORD(lParam);
			mouse_current_y = HIWORD(lParam);

			InvalidateRect(hwnd,NULL, false);
			break;
		case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			return 0;

		case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			return 0;

		case WM_PAINT:
			{
				pDemoApp->OnRender();
				//ValidateRect(hwnd, NULL);
			}
			return 0;

		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			return 1;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}




// Creates a Direct2D bitmap from the specified file name.
HRESULT DemoApp::LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
				}
			}
		}
		else // Don't scale the image.
		{
			hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		}
	}
	if (SUCCEEDED(hr))
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
	}

	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pScaler);

	return hr;
}
