//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "BaseLayout.h"
#include "PropertyGrid.h"
#include "EnvironmentPane.h"
#include "TerrainPane.h"

namespace Infinite {

	class RenderWindow
	{
		DECLARE_SINGLETON(RenderWindow);

	public:
		RenderWindow(MyGUI::ImageBox * _window);
		~RenderWindow();

		void Init();
		void Shutdown();
		void Update();

	protected:
		void OnMouseSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);
		void OnMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);

		void OnKeySetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);
		void OnKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);

		void OnMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void OnMouseClick(MyGUI::Widget* _sender);
		void OnKeyDown(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);
		void OnKeyUp(MyGUI::Widget* _sender, MyGUI::KeyCode _key);


	protected:
		MyGUI::ImageBox * mRenderWindow;
		MGUI_Texture * mRenderTexture;

		bool mFocus;
	};

	




	//
	//
	class ToolBar : public wraps::BaseLayout
	{
	public:
		ToolBar(MyGUI::Widget * _parent);
		virtual ~ToolBar();

	protected:
		void NotifyOperatorChanged(MyGUI::Widget* _sender);

	protected:
		MyGUI::ImageBox * mPick;
		MyGUI::ImageBox * mMove;
		MyGUI::ImageBox * mRotate;
		MyGUI::ImageBox * mScale;
		MyGUI::ImageBox * mTerrain;
	};







	//
	//
	class WorkspaceControl : public wraps::BaseLayout
	{
	public:
		WorkspaceControl(MyGUI::Widget * _parent);
		virtual ~WorkspaceControl();

		void Resize();

	protected:
		MyGUI::ImageBox * mImageBox;
		RenderWindow * mRenderWindow;
		ToolBar * mToolBar;
	};









	//
	//
	class ToolControl : public wraps::BaseLayout
	{
		DECLARE_SINGLETON(ToolControl);

	public:
		ToolControl(MyGUI::Widget * _parent);
		virtual ~ToolControl();

		TString128 GetSelectObject();
		MyGUI::TabItem * GetPage(const char * name);

	protected:
		void _OnInit(Event * sender);
		void _OnAfterSceneLoaded(Event * sender);
		void _OnSelect(Event * sender);
		void _OnUnSelect(Event * sender);
		void _OnObjCreated(Event * sneder);

		void requestCreateObjectItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item);
		void requestCoordItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag);
		void requestDrawItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info);

		void notifySelectObjectType(MyGUI::Widget* _sender);

	protected:
		tEventListener<ToolControl> OnInit;
		tEventListener<ToolControl> OnAfterSceneLoaded;
		tEventListener<ToolControl> OnSelectObj;
		tEventListener<ToolControl> OnUnSelectObj;
		tEventListener<ToolControl> OnObjCreated;

		MyGUI::TabControl * mTabControl;
		MyGUI::Widget * mObject;
		PropertyGrid * mPropery;
		PropertyGrid * mRenderSetting;
		EnvironmentPane * mEnvironmentPane;
		TerrainPane * mTerrainPane;

		MyGUI::Button * mCurrentObject;
	};











	//
	//
	class MainWorkSpace : public wraps::BaseLayout
	{
	public:
		MainWorkSpace(MyGUI::Widget * _parent);
		virtual ~MainWorkSpace();

	protected:
		ToolControl * mToolControl;
		WorkspaceControl * mWorkspaceControl;
	};

}
