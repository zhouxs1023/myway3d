#include "stdafx.h"

#include "MainWorkSpace.h"
#include "Editor.h"
#include "xRenderSetting.h"

#define RENDER_WINDOW_TEXTURE "UI_RenderTexture"

namespace Infinite {

	IMP_SLN(RenderWindow);

	RenderWindow::RenderWindow(MyGUI::ImageBox * _window)
	{
		INIT_SLN;

		mRenderWindow = _window;
		mRenderTexture = (MGUI_Texture *)MGUI_System::Instance()->GetRenderManager()->createTexture(RENDER_WINDOW_TEXTURE);

		mFocus = false;

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
		Editor::Instance()->SetFoucs(true);
	}

	void RenderWindow::OnMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		mFocus = false;
		Editor::Instance()->SetFoucs(false);
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
		if (!mFocus || !xScene::Instance()->IsInited())
			return ;

		SceneNode * cam = World::Instance()->MainCameraNode();

		if (IKeyboard::Instance()->KeyPressed(KC_W))
		{
			cam->Move(5.0f);
		}
		else if (IKeyboard::Instance()->KeyPressed(KC_S))
		{
			cam->Move(-5.0f);
		}
		else if (IKeyboard::Instance()->KeyPressed(KC_A))
		{
			cam->Right(-5.0f);
		}
		else if (IKeyboard::Instance()->KeyPressed(KC_D))
		{
			cam->Right(5.0f);
		}
		else if (IKeyboard::Instance()->KeyUp(KC_DELETE))
		{
			for (int i = 0; i < Editor::Instance()->GetSelectedShapeSize(); ++i)
				ShapeManager::Instance()->Distroy(Editor::Instance()->GetSelectedShape(i));

			if (Editor::Instance()->GetSelectedShapeSize() > 0)
				Editor::Instance()->SetSelectedShape(NULL);
		}

		// parse 
		else if (IKeyboard::Instance()->KeyPressed(KC_LCONTROL) &&
			IKeyboard::Instance()->KeyUp(KC_V))
		{
			int count = 0;
			Shape * objs[1024];

			for (int i = 0; i < Editor::Instance()->GetSelectedShapeSize(); ++i)
			{
				Shape * newObj = Editor::Instance()->GetSelectedShape(i)->Clone();

				if (newObj)
					objs[count++] = newObj;
			}

			if (count)
				Editor::Instance()->SetSelectedShapes(objs, count);
		}

		// undo
		else if (IKeyboard::Instance()->KeyPressed(KC_LCONTROL) &&
			IKeyboard::Instance()->KeyUp(KC_Z))
		{
			xUndoRedoManager::Instance()->Undo();
		}

		// redo
		else if (IKeyboard::Instance()->KeyPressed(KC_LCONTROL) &&
			IKeyboard::Instance()->KeyUp(KC_Y))
		{
			xUndoRedoManager::Instance()->Redo();
		}
		

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
			cam->Move(0.5f * IMouse::Instance()->MouseWheel());
		}

		MyGUI::IntCoord rc = mRenderWindow->getClientCoord();
		MyGUI::IntPoint pt = mRenderWindow->getAbsolutePosition();

		Point2i mousePt = IMouse::Instance()->GetPosition();
		float mouseX = (mousePt.x - pt.left) / (float)rc.width;
		float mouseY = (mousePt.y - pt.top) / (float)rc.height;

		Editor::Instance()->_SetMousePosition(Point2f(mouseX, mouseY));

		// pick
		eOperator op = Editor::Instance()->GetOperator();
		if ((op == eOP_Pick || op == eOP_Move || op == eOP_Rotate || op == eOP_Scale) &&
			IMouse::Instance()->KeyUp(MKC_BUTTON0) && !Gizmo::Instance()->IsPicked())
		{
			float x = mouseX, y = mouseY;

			if (x > 0 && x < 1 && y > 0 && y < 1)
			{
				Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

				HitInfoSetArray infos;

				IPhyWorld::Instance()->RayCheck(infos, ray, 1000.0f, PICK_Flag, true);

				if (infos.Size() > 0 && infos[0].node)
				{
					Shape * obj = ShapeManager::Instance()->Get(infos[0].node);

					if (obj)
						Editor::Instance()->SetSelectedShape(obj);
				}
				else
				{
					Editor::Instance()->SetSelectedShape(NULL);
				}
			}
		}

		Gizmo::Instance()->Update(mouseX, mouseY);


		if (op != eOP_Terrain && Utils::IsMouseDoubleClick(MKC_BUTTON0))
		{
			TString128 type = ToolControl::Instance()->GetSelectObject();

			if (type != "")
			{
				Shape * shape = ShapeManager::Instance()->Create(type.c_str());

				if (shape)
				{
					Vec3 pos = Editor::Instance()->GetHitPosition(mouseX, mouseY);

					shape->SetPosition(pos);

					Editor::Instance()->SetSelectedShape(shape);
				}
			}
		}
	}

	void RenderWindow::OnDragFile(const char * file)
	{
		MyGUI::IntCoord rc = mRenderWindow->getClientCoord();
		MyGUI::IntPoint pt = mRenderWindow->getAbsolutePosition();

		Point2i mousePt = IMouse::Instance()->GetPosition();
		float mouseX = (mousePt.x - pt.left) / (float)rc.width;
		float mouseY = (mousePt.y - pt.top) / (float)rc.height;

		if (mouseX <= 0 || mouseX >= 1 || mouseY <= 0 || mouseY >= 1 ||
			!xScene::Instance()->IsInited())
			return ;

		TString128 fileName = file;

		xEvent::OnDragFile(&Point2f(mouseX, mouseY), fileName.c_str());
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
			Editor::Instance()->SetOperator(eOP_Pick);
		}
		else if (_sender == mMove)
		{
			mMove->setAlpha(1);
			Editor::Instance()->SetOperator(eOP_Move);
		}
		else if (_sender == mRotate)
		{
			mRotate->setAlpha(1);
			Editor::Instance()->SetOperator(eOP_Rotate);
		}
		else if (_sender == mScale)
		{
			mScale->setAlpha(1);
			Editor::Instance()->SetOperator(eOP_Scale);
		}
		else if (_sender == mTerrain)
		{
			mTerrain->setAlpha(1);
			Editor::Instance()->SetOperator(eOP_Terrain);
		}
		else
		{
			Editor::Instance()->SetOperator(eOP_Unknown);
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
		, OnAfterSceneLoaded(xEvent::OnAfterLoadScene, this, &ToolControl::_OnAfterSceneLoaded)
		, OnSelectObj(xEvent::OnSelectObj, this, &ToolControl::_OnSelect)
		, OnUnSelectObj(xEvent::OnUnSelectObj, this, &ToolControl::_OnUnSelect)
		, OnInit(xEvent::OnInit, this, &ToolControl::_OnInit)
		, OnObjCreated(xEvent::OnObjCreated, this, &ToolControl::_OnObjCreated)
		, mCurrentObject(NULL)
	{
		assignWidget(mTabControl, "TabControl");
		assignWidget(mObject, "Object");
		assignBase(mPropery, "Property");
		assignBase(mRenderSetting, "RenderSetting");
		assignBase(mEnvironmentPane, "Environment");
		assignBase(mTerrainPane, "Terrain");

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

	TString128 ToolControl::GetSelectObject()
	{
		return mCurrentObject != NULL ? mCurrentObject->getCaption().asUTF8_c_str() : "";
	}

	void ToolControl::_OnInit(Event * sender)
	{
		MyGUI::ItemBox* box = mObject->createWidget<MyGUI::ItemBox>("ItemBoxEmpty", MyGUI::IntCoord(0, 0, mObject->getSize().width, mObject->getSize().height), MyGUI::Align::Stretch);

		box->requestCreateWidgetItem = MyGUI::newDelegate(this, &ToolControl::requestCreateObjectItem);
		box->requestCoordItem = MyGUI::newDelegate(this, &ToolControl::requestCoordItem);
		box->requestDrawItem = MyGUI::newDelegate(this, &ToolControl::requestDrawItem);

		for (int i = 0; i < ShapeManager::Instance()->GetFactoryCount(); ++i)
		{
			ShapeFactory * sf = ShapeManager::Instance()->GetFactory(i);
			box->addItem(sf);
		}
	}

	void ToolControl::requestCreateObjectItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
	{
		MyGUI::Button* button = _item->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, _item->getWidth(), _item->getHeight()), MyGUI::Align::Stretch);

		button->setTextAlign(MyGUI::Align::Center);
		button->eventMouseButtonClick += MyGUI::newDelegate(this, &ToolControl::notifySelectObjectType);
		//button->setNeedToolTip(true);
		//button->eventToolTip += MyGUI::newDelegate(this, &ToolControl::notifyToolTip);

		_item->setUserData(button);
	}

	void ToolControl::requestCoordItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag)
	{
		_coord.set(0, 0, 120, 25);
	}

	void ToolControl::requestDrawItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info)
	{
		MyGUI::Button* button = *_item->getUserData<MyGUI::Button*>();
		ShapeFactory * sf = *_sender->getItemDataAt<ShapeFactory*>(_info.index);

		if (_info.update)
		{
			button->setCaption(sf->GetTypeName());
		}

		//button->setStateSelected(_info.select);
	}

	void ToolControl::notifySelectObjectType(MyGUI::Widget* _sender)
	{
		MyGUI::Button* button = _sender->castType<MyGUI::Button>();

		if (mCurrentObject)
			mCurrentObject->setStateSelected(false);

		mCurrentObject = button;

		mCurrentObject->setStateSelected(true);
	}


	void ToolControl::_OnAfterSceneLoaded(Event * sender)
	{
		mRenderSetting->SetObject(xRenderSetting::Instance());
	}

	void ToolControl::_OnSelect(Event * sender)
	{
		Shape * obj = Editor::Instance()->GetSelectedShape(0);

		if (Editor::Instance()->GetSelectedShapeSize() > 1)
			obj = NULL;

		mPropery->SetObject(obj);
	}

	void ToolControl::_OnUnSelect(Event * sender)
	{
		mPropery->SetObject(NULL);
	}

	void ToolControl::_OnObjCreated(Event * sender)
	{
		if (mCurrentObject != NULL)
		{
			mCurrentObject->setStateSelected(false);
			mCurrentObject = NULL;
		}
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