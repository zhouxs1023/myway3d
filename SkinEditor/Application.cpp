/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "Application.h"
#include "SkinManager.h"
#include "ActionManager.h"
#include "CommandManager.h"
#include "ExportManager.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MessageBoxManager.h"
#include "Tools/DialogManager.h"
#include "HotKeyManager.h"
#include "StateManager.h"
#include "RecentFilesManager.h"
#include "SettingsManager.h"
#include "ColourManager.h"
#include "Localise.h"
#include "Grid.h"
#include "resource1.h"

template <> tools::Application* MyGUI::Singleton<tools::Application>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::Application>::mClassTypeName("Application");

namespace tools
{

	Application::Application()
		: mUISystem()
		, mEditorState(nullptr)
		, mTestState(nullptr)
		, OnKeyPress(mUISystem.OnKeyPress, this, &Application::_OnKeyPress)
	{
	}

	Application::~Application()
	{
	}

	void Application::OnPreCreateWindow(WNDCLASS & wc, LPTSTR tile, DWORD & style, POINT & pt, SIZE & sz)
	{
		wc.hIcon = LoadIcon(mhInst, MAKEINTRESOURCE(IDI_ICON1));
		strcpy(tile, "SkinEditor");
	}

	void Application::createScene()
	{
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		new SettingsManager();
		SettingsManager::getInstance().initialise("se_user_settings.xml");

		MyGUI::ResourceManager::getInstance().load("EditorLanguage.xml");

		std::string language = SettingsManager::getInstance().getSector("Settings")->getPropertyValue("InterfaceLanguage");
		if (language.empty() || language == "Auto")
		{
			if (!mLocale.empty())
				MyGUI::LanguageManager::getInstance().setCurrentLanguage(mLocale);
		}
		else
		{
			MyGUI::LanguageManager::getInstance().setCurrentLanguage(language);
		}

		new RecentFilesManager();
		RecentFilesManager::getInstance().initialise();

		new CommandManager();
		CommandManager::getInstance().initialise();

		new SkinManager();
		SkinManager::getInstance().initialise();

		new ActionManager();
		ActionManager::getInstance().initialise();

		new ExportManager();
		ExportManager::getInstance().initialise();

		new MessageBoxManager();
		MessageBoxManager::getInstance().initialise();

		new DialogManager();
		DialogManager::getInstance().initialise();

		new HotKeyManager();
		HotKeyManager::getInstance().initialise();

		new StateManager();
		StateManager::getInstance().initialise();

		new ColourManager();
		ColourManager::getInstance().initialise();

		new Grid();
		Grid::getInstance().initialise();

		MyGUI::ResourceManager::getInstance().load("Initialise.xml");

		const SettingsSector::VectorUString& additionalResources = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalResources");
		for (SettingsSector::VectorUString::const_iterator iter = additionalResources.begin(); iter != additionalResources.end(); ++iter)
			MyGUI::ResourceManager::getInstance().load(*iter);

		mEditorState = new EditorState();
		mTestState = new TestState();

		StateManager::getInstance().registerState(this, "Application");
		StateManager::getInstance().registerState(mEditorState, "EditorState");
		StateManager::getInstance().registerState(mTestState, "TestState");

		StateManager::getInstance().registerEventState("Application", "Start", "EditorState");
		StateManager::getInstance().registerEventState("EditorState", "Test", "TestState");
		StateManager::getInstance().registerEventState("EditorState", "Exit", "Application");
		StateManager::getInstance().registerEventState("TestState", "Exit", "EditorState");

		StateManager::getInstance().pushState(this);
		StateManager::getInstance().stateEvent(this, "Start");
	}

	void Application::destroyScene()
	{
		StateManager::getInstance().rollbackToState(nullptr);

		delete mEditorState;
		mEditorState = nullptr;

		delete mTestState;
		mTestState = nullptr;

		Grid::getInstance().shutdown();
		delete Grid::getInstancePtr();

		ColourManager::getInstance().shutdown();
		delete ColourManager::getInstancePtr();

		StateManager::getInstance().shutdown();
		delete StateManager::getInstancePtr();

		HotKeyManager::getInstance().shutdown();
		delete HotKeyManager::getInstancePtr();

		DialogManager::getInstance().shutdown();
		delete DialogManager::getInstancePtr();

		MessageBoxManager::getInstance().shutdown();
		delete MessageBoxManager::getInstancePtr();

		ExportManager::getInstance().shutdown();
		delete ExportManager::getInstancePtr();

		ActionManager::getInstance().shutdown();
		delete ActionManager::getInstancePtr();

		SkinManager::getInstance().shutdown();
		delete SkinManager::getInstancePtr();

		CommandManager::getInstance().shutdown();
		delete CommandManager::getInstancePtr();

		RecentFilesManager::getInstance().shutdown();
		delete RecentFilesManager::getInstancePtr();

		SettingsManager::getInstance().shutdown();
		delete SettingsManager::getInstancePtr();

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
	}

	void Application::prepare()
	{
		mLocale = "English";
	}

	void Application::onFileDrop(const std::wstring& _fileName)
	{
		CommandManager::getInstance().setCommandData(_fileName);
		CommandManager::getInstance().executeCommand("Command_FileDrop");
	}

	bool Application::onWinodwClose(size_t _handle)
	{
		CommandManager::getInstance().executeCommand("Command_QuitApp");
		return false;
	}

	void Application::_OnKeyPress(Event * sender)
	{
		bool & _inject = *(bool *)sender->GetParam(0);
		MyGUI::KeyCode _key = *(MyGUI::KeyCode *)sender->GetParam(1);
		MyGUI::Char _text = *(MyGUI::Char *)sender->GetParam(2);

		MyGUI::InputManager& input = MyGUI::InputManager::getInstance();

		_inject = !HotKeyManager::getInstance().onKeyEvent(true, input.isShiftPressed(), input.isControlPressed(), _key);
	}


	void Application::command_QuitApp(const MyGUI::UString& _commandName, bool& _result)
	{
		if (DialogManager::getInstance().getAnyDialog())
		{
			DialogManager::getInstance().endTopDialog();
		}
		else
		{
			if (MessageBoxManager::getInstance().hasAny())
			{
				MessageBoxManager::getInstance().endTop(MyGUI::MessageBoxStyle::Cancel);
			}
			else
			{
				CommandManager::getInstance().executeCommand("Command_Quit");
			}
		}

		_result = true;
	}


	const Application::VectorWString& Application::getParams()
	{
		return mParams;
	}

}

