#include "MWApp_Win32.h"
#include "MGUI_System.h"

using namespace Myway;

class MyApp : public App_Win32
{
	MGUI_System mUISystem;
	MyGUI::Widget* mDemoView;

public:
	MyApp()
	{
	}

	~MyApp() {}
	
	void CreateUI()
	{
		bool hr = MyGUI::ResourceManager::getInstance().load("MyGUI_BlueWhiteTheme.xml");

		d_assert (hr);

		MyGUI::VectorWidgetPtr windows = MyGUI::LayoutManager::getInstance().loadLayout("Demos\\Demo_Themes\\Themes.layout");
		MYGUI_ASSERT(windows.size() == 1, "Error load layout");
		mDemoView = windows[0];

		MyGUI::ComboBox * mComboSkins = MyGUI::Gui::getInstance().findWidget<MyGUI::ComboBox>("Combo");
		mComboSkins->setComboModeDrop(true);
		mComboSkins->addItem("blue & white");
		mComboSkins->addItem("black & blue");
		mComboSkins->addItem("black & orange");

		mComboSkins->setIndexSelected(0);
		mComboSkins->eventComboAccept += MyGUI::newDelegate(this, &MyApp::notifyComboAccept);
	}

	void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		int i = 0;
		int j = 0;
	}

	void DestroyUI()
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(mDemoView);
	}

	virtual bool Init()
	{
		App_Win32::Init();

		mUISystem.Init();

		CreateUI();

		return true;
	}

	virtual void Shutdown()
	{
		DestroyUI();

		mUISystem.Shutdown();

		App_Win32::Shutdown();
	}

	virtual void OnMessage(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		mUISystem.InjectKeyEvent(uMsg, wParam, lParam);
	}

	virtual void Update()
	{
		App_Win32::Update();

		InputSystem::Instance()->Update();

		mUISystem.InjectMouseEvent();

		Point2f mousePt = IMouse::Instance()->GetPositionUnit();

		if (mousePt.x >= 0 && mousePt.x <= 1.0f &&
			mousePt.y >= 0 && mousePt.y <= 1.0f)
		{
			while (::ShowCursor(FALSE) > 1) ;
		}
		else
		{
			while (::ShowCursor(TRUE) < 1) ;
		}
	}
};


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));

	//BreakAllock(803);

	char sFileName[1024];
	GetModuleFileName(GetModuleHandle(NULL), sFileName, 1024);

	TString128 fileDir = File::GetFileDir(sFileName);

	SetCurrentDirectory(fileDir.c_str());

	MyApp app;

	app.Run(hInstance);

	return 0;
}