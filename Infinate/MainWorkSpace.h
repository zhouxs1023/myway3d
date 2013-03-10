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

#include "Common\\BaseLayout.h"

namespace Infinite {

	class RenderWindow
	{
		DECLARE_SINGLETON(RenderWindow);

	public:
		RenderWindow(MyGUI::ImageBox * _window);
		~RenderWindow();

		void Init();
		void Shutdown();

	protected:
		MyGUI::ImageBox * mRenderWindow;
		MGUI_Texture * mRenderTexture;
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
	};









	//
	//
	class ToolControl : public wraps::BaseLayout
	{
	public:
		ToolControl(MyGUI::Widget * _parent);
		virtual ~ToolControl();

	protected:
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
