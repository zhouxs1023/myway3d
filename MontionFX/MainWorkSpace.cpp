#include "stdafx.h"

#include "MainWorkSpace.h"
#include "Editor.h"

#define RENDER_WINDOW_TEXTURE "UI_RenderTexture"

namespace MotionFX {

	IMP_SLN(RenderWindow);

	RenderWindow::RenderWindow(MyGUI::ImageBox * _window)
	{
		INIT_SLN;

		mRenderWindow = _window;
		mRenderTexture = (MGUI_Texture *)MGUI_System::Instance()->GetRenderManager()->createTexture(RENDER_WINDOW_TEXTURE);

		mFocus = false;
		mWheelStep = 0.5f;

		Init();
	}

	RenderWindow::~RenderWindow()
	{
		SHUT_SLN;

		MGUI_System::Instance()->GetRenderManager()->destroyTexture(mRenderTexture);
	}

	void RenderWindow::Init()
	{
		RenderScheme * scheme = Engine::Instance()->GetRenderScheme();

		int w = mRenderWindow->getSize().width;
		int h = mRenderWindow->getSize().height;

		MyGUI::IntCoord coord = mRenderWindow->getClientCoord();

		scheme->Resize(w, h);
		World::Instance()->MainCamera()->SetAspect((float)w / h);

		mRenderTexture->createFromTexture(scheme->GetColorTexture());

		mRenderWindow->eventMouseSetFocus += MyGUI::newDelegate(this, &RenderWindow::OnMouseSetFocus);
		mRenderWindow->eventMouseLostFocus += MyGUI::newDelegate(this, &RenderWindow::OnMouseLostFocus);

		mRenderWindow->eventKeySetFocus += MyGUI::newDelegate(this, &RenderWindow::OnKeySetFocus);
		mRenderWindow->eventKeyLostFocus += MyGUI::newDelegate(this, &RenderWindow::OnKeyLostFocus);

		mRenderWindow->eventMouseDrag += MyGUI::newDelegate(this, &RenderWindow::OnMouseDrag);
		mRenderWindow->eventMouseButtonClick += MyGUI::newDelegate(this, &RenderWindow::OnMouseClick);
		mRenderWindow->eventKeyButtonPressed += MyGUI::newDelegate(this, &RenderWindow::OnKeyDown);
		mRenderWindow->eventKeyButtonReleased += MyGUI::newDelegate(this, &RenderWindow::OnKeyUp);
	}

	void RenderWindow::Shutdown()
	{
		mRenderWindow->eventMouseSetFocus -= MyGUI::newDelegate(this, &RenderWindow::OnMouseSetFocus);
		mRenderWindow->eventMouseLostFocus -= MyGUI::newDelegate(this, &RenderWindow::OnMouseLostFocus);

		mRenderWindow->eventKeySetFocus -= MyGUI::newDelegate(this, &RenderWindow::OnKeySetFocus);
		mRenderWindow->eventKeyLostFocus -= MyGUI::newDelegate(this, &RenderWindow::OnKeyLostFocus);

		mRenderWindow->eventMouseDrag -= MyGUI::newDelegate(this, &RenderWindow::OnMouseDrag);
		mRenderWindow->eventMouseButtonClick -= MyGUI::newDelegate(this, &RenderWindow::OnMouseClick);
		mRenderWindow->eventKeyButtonPressed -= MyGUI::newDelegate(this, &RenderWindow::OnKeyDown);
		mRenderWindow->eventKeyButtonReleased -= MyGUI::newDelegate(this, &RenderWindow::OnKeyUp);

		mRenderTexture->destroy();
	}

	void RenderWindow::OnMouseSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
		mFocus = true;
	}

	void RenderWindow::OnMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		mFocus = false;
	}

	void RenderWindow::OnKeySetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
	}

	void RenderWindow::OnKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
	}

	void RenderWindow::OnMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
	}

	void RenderWindow::OnMouseClick(MyGUI::Widget* _sender)
	{
	}

	void RenderWindow::OnKeyDown(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
	{
	}

	void RenderWindow::OnKeyUp(MyGUI::Widget* _sender, MyGUI::KeyCode _key)
	{
	}

	void RenderWindow::Update()
	{
		if (!mFocus)
			return ;

		SceneNode * cam = World::Instance()->MainCameraNode();

		// camera
		if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(MKC_BUTTON1))
		{
			Point2i pt = IMouse::Instance()->GetPositionDiff();

			if (abs(pt.y) >abs(pt.x))
			{
				cam->Pitch(pt.y * 0.005f, TS_LOCAL);
			}
			else
			{
				cam->Yaw(pt.x * 0.005f, TS_PARENT);
			}
		}

		if (IMouse::Instance()->MouseWheel())
		{
			cam->Move(0.1f * mWheelStep * IMouse::Instance()->MouseWheel());
		}

		MyGUI::IntCoord rc = mRenderWindow->getClientCoord();
		MyGUI::IntPoint pt = mRenderWindow->getAbsolutePosition();

		Point2i mousePt = IMouse::Instance()->GetPosition();
		float mouseX = (mousePt.x - pt.left) / (float)rc.width;
		float mouseY = (mousePt.y - pt.top) / (float)rc.height;

		Editor::Instance()->_SetMousePosition(Point2f(mouseX, mouseY));
	}

	void RenderWindow::OnDragFile(const char * file)
	{
		MyGUI::IntCoord rc = mRenderWindow->getClientCoord();
		MyGUI::IntPoint pt = mRenderWindow->getAbsolutePosition();

		Point2i mousePt = IMouse::Instance()->GetPosition();
		float mouseX = (mousePt.x - pt.left) / (float)rc.width;
		float mouseY = (mousePt.y - pt.top) / (float)rc.height;

		if (mouseX <= 0 || mouseX >= 1 || mouseY <= 0 || mouseY >= 1)
			return ;

		const char * testFile = file;
		int length = strlen(file);

		bool exist = false;

		while (length > 0)
		{
			if (testFile[length - 1] == '\\')
			{
				const char * tfile = testFile + length;
				exist = ResourceManager::Instance()->Exist(tfile);

				if (exist)
					break;
			}

			--length;
		}

		if (!exist)
			return ;

		testFile += length;
		TString128 type = File::GetExternName(testFile);

		if (type == "mesh")
		{
			Editor::Instance()->GetActor()->SetMesh(testFile);

			// look at entity
			Aabb bound = Editor::Instance()->GetActor()->GetWorldAabb();

			float size = 0;

			size = Math::Maximum(size, bound.GetWidth());
			size = Math::Maximum(size, bound.GetHeight());
			size = Math::Maximum(size, bound.GetDepth());

			size = Math::Minimum(10.0f, size);

			Camera * pCamera = World::Instance()->MainCamera();
			pCamera->SetPosition(bound.GetCenter() + Vec3(0, +0.5f, -1) * size * 1.5f);
			pCamera->LookAt(bound.GetCenter());

			mWheelStep = size * 0.1f;
		}
		else if (type == "anim")
		{
			if (Editor::Instance()->GetActor()->GetMesh() == NULL)
				return ;

			TString128 baseName = File::GetBaseName(testFile);
			TString128 animName = File::RemoveExternName(baseName);

			Editor::Instance()->GetActor()->LoadAnimation(animName.c_str(), testFile);
			Editor::Instance()->GetActor()->PlayAnimation(animName.c_str());
		}
	}











	const float gToolBarAlpha = 0.5f;

	ToolBar::ToolBar(MyGUI::Widget * _parent)
		: BaseLayout("ToolBar.layout", _parent)
	{
		assignWidget(mPick, "Pick");
		assignWidget(mMove, "Move");
		assignWidget(mRotate, "Rotate");
		assignWidget(mScale, "Scale");
		assignWidget(mTerrain, "Terrain");

		mPick->setImageTexture("iPick.png");
		mMove->setImageTexture("iMove.png");
		mRotate->setImageTexture("iRotate.png");
		mScale->setImageTexture("iScale.png");
		mTerrain->setImageTexture("iTerrain.png");

		mPick->eventMouseButtonClick += MyGUI::newDelegate(this, &ToolBar::NotifyOperatorChanged);
		mMove->eventMouseButtonClick += MyGUI::newDelegate(this, &ToolBar::NotifyOperatorChanged);
		mRotate->eventMouseButtonClick += MyGUI::newDelegate(this, &ToolBar::NotifyOperatorChanged);
		mScale->eventMouseButtonClick += MyGUI::newDelegate(this, &ToolBar::NotifyOperatorChanged);
		mTerrain->eventMouseButtonClick += MyGUI::newDelegate(this, &ToolBar::NotifyOperatorChanged);

		NotifyOperatorChanged(NULL);
	}

	ToolBar::~ToolBar()
	{
	}

	void ToolBar::NotifyOperatorChanged(MyGUI::Widget* _sender)
	{
		mPick->setAlpha(gToolBarAlpha);
		mMove->setAlpha(gToolBarAlpha);
		mRotate->setAlpha(gToolBarAlpha);
		mScale->setAlpha(gToolBarAlpha);
		mTerrain->setAlpha(gToolBarAlpha);

		if (_sender == mPick)
		{
			mPick->setAlpha(1);
		}
		else if (_sender == mMove)
		{
			mMove->setAlpha(1);
		}
		else if (_sender == mRotate)
		{
			mRotate->setAlpha(1);
		}
		else if (_sender == mScale)
		{
			mScale->setAlpha(1);
		}
		else if (_sender == mTerrain)
		{
			mTerrain->setAlpha(1);
		}
		else
		{
		}
	}






	StatusBar::StatusBar(MyGUI::Widget * _parent)
		: BaseLayout("StatusBar.layout", _parent)
		, OnUpdateText(RenderEvent::OnAfterRender, this, &StatusBar::_OnUpdateText)
	{
		assignWidget(mText, "Text");
	}

	StatusBar::~StatusBar()
	{
	}

	void StatusBar::_OnUpdateText(Event * _sender)
	{
		float fps = Engine::Instance()->GetFPS();
		int tc = Engine::Instance()->GetPrimitiveCount();
		int bc = Engine::Instance()->GetBatchCount();

		char buffer[128];

		sprintf_s(buffer, 128, "FPS: %.2f, TC: %d, BC: %d.", fps, tc, bc);

		mText->setCaption(buffer);
	}





	WorkspaceControl::WorkspaceControl(MyGUI::Widget * _parent)
		: BaseLayout("Workspace.layout", _parent)
	{
		assignBase(mToolBar, "ToolBar");
		assignBase(mStatusBar, "StatusBar");
		assignWidget(mImageBox, "RenderWindow");

		mRenderWindow = new RenderWindow(mImageBox);

		mImageBox->setImageTexture(RENDER_WINDOW_TEXTURE);
	}

	WorkspaceControl::~WorkspaceControl()
	{
		delete mRenderWindow;
	}

	void WorkspaceControl::Resize()
	{
	}










	IMP_SLN(ToolControl);
	ToolControl::ToolControl(MyGUI::Widget * _parent)
		: BaseLayout("ToolControl.layout", _parent)
	{
		assignWidget(mTabControl, "TabControl");

		INIT_SLN;
	}

	ToolControl::~ToolControl()
	{
		SHUT_SLN;
	}

	MyGUI::TabItem * ToolControl::GetPage(const char * name)
	{
		for (int i = 0; i < (int)mTabControl->getItemCount(); ++i)
		{
			MyGUI::TabItem * p = mTabControl->getItemAt(i);

			if (p->getCaption() == name)
				return p;
		}

		MyGUI::TabItem * page = mTabControl->addItem(name);

		return page;
	}










	MainWorkSpace::MainWorkSpace(MyGUI::Widget * _parent)
		: BaseLayout("MainWorkSpace.layout", _parent)
	{
		assignWidget(mLeftPanel, "Left");
		assignWidget(mRightPanel, "Right");
		assignWidget(mSeparatorH, "SeparatorH");

		assignBase(mToolControl, "ToolsControl");
		assignBase(mWorkspaceControl, "WorkspaceControl");

		mMinSizeLeft = MyGUI::utility::parseValue<int>(mLeftPanel->getUserString("MinSize"));
		mMinSizeRight = MyGUI::utility::parseValue<int>(mRightPanel->getUserString("MinSize"));

		mSeparatorH->eventMouseButtonPressed += MyGUI::newDelegate(this, &MainWorkSpace::notifyMouseButtonPressed);
		mSeparatorH->eventMouseButtonReleased += MyGUI::newDelegate(this, &MainWorkSpace::notifyMouseButtonReleased);
		mSeparatorH->eventMouseDrag += MyGUI::newDelegate(this, &MainWorkSpace::notifyMouseDrag);

		mNeedResize = false;
	}

	MainWorkSpace::~MainWorkSpace()
	{
	}

	void MainWorkSpace::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			mStartLeftPanel = mLeftPanel->getCoord();
			mStartRightPanel = mRightPanel->getCoord();
			mStartSeparatorH = mSeparatorH->getCoord();
			mStartMousePosition = MyGUI::InputManager::getInstance().getMousePosition();
			mNeedResize = false;
		}
	}

	void MainWorkSpace::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			RenderWindow::Instance()->Shutdown();
			RenderWindow::Instance()->Init();

			mNeedResize = false;
		}
	}

	void MainWorkSpace::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			MyGUI::IntPoint mousePosition = MyGUI::InputManager::getInstance().getMousePosition();
			int delta = mousePosition.left - mStartMousePosition.left;

			MyGUI::IntCoord leftPanel = mStartLeftPanel;
			MyGUI::IntCoord rightPanel = mStartRightPanel;
			MyGUI::IntCoord separatorHPanel = mStartSeparatorH;

			leftPanel.width += delta;
			separatorHPanel.left += delta;
			rightPanel.left += delta;
			rightPanel.width -= delta;

			int diffLeft = mMinSizeLeft - leftPanel.width;
			if (diffLeft > 0)
			{
				leftPanel.width += diffLeft;
				separatorHPanel.left += diffLeft;
				rightPanel.left += diffLeft;
				rightPanel.width -= diffLeft;
			}

			int diffRight = mMinSizeRight - rightPanel.width;
			if (diffRight > 0)
			{
				leftPanel.width -= diffRight;
				separatorHPanel.left -= diffRight;
				rightPanel.left -= diffRight;
				rightPanel.width += diffRight;
			}

			mLeftPanel->setCoord(leftPanel);
			mRightPanel->setCoord(rightPanel);
			mSeparatorH->setCoord(separatorHPanel);

			mNeedResize = true;
		}
	}

}