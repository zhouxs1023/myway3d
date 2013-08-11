/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "Common.h"
#include "Application.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "GroupMessage.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include "CommandManager.h"
#include "SettingsManager.h"
#include "WidgetSelectorManager.h"
#include "HotKeyManager.h"
#include "MessageBoxManager.h"
#include "Tools/DialogManager.h"
#include "StateManager.h"
#include "Localise.h"
#include "WidgetCreatorManager.h"
#include "RecentFilesManager.h"
#include "BackwardCompatibilityManager.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RTTLayer.h"
#include "ColourManager.h"
#include "EditorToolTip.h"
#include "PropertyFieldManager.h"
#include "Grid.h"

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

	void _loadSkins()
	{
		Archive::FileInfoList skins;
		ResourceManager::Instance()->GetFileInfosByFloder(skins, "Skins");

		Archive::FileInfoList::Iterator whr = skins.Begin();
		Archive::FileInfoList::Iterator end = skins.End();

		while (whr != end)
		{
			if (whr->name.Find(".xml") != NPOS)
			{
				MyGUI::ResourceManager::getInstance().load(whr->name.c_str());
			}

			++whr;
		}
	}

	void Application::createScene()
	{
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>("Layer");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		MyGUI::ResourceManager::getInstance().load("EditorLayers.xml");
		//MyGUI::ResourceManager::getInstance().load("EditorSettings.xml");

		_loadSkins();
	
		new SettingsManager();
		SettingsManager::getInstance().initialise("le_user_settings.xml");

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

		new CommandManager();
		CommandManager::getInstance().initialise();

		new WidgetSelectorManager();
		WidgetSelectorManager::getInstance().initialise();

		new WidgetCreatorManager();
		WidgetCreatorManager::getInstance().initialise();

		new HotKeyManager();
		HotKeyManager::getInstance().initialise();

		new MessageBoxManager();
		MessageBoxManager::getInstance().initialise();

		new DialogManager();
		DialogManager::getInstance().initialise();

		new WidgetTypes();
		WidgetTypes::getInstance().initialise();

		new EditorWidgets();
		EditorWidgets::getInstance().initialise();

		new BackwardCompatibilityManager();
		BackwardCompatibilityManager::getInstance().initialise();

		new UndoManager();
		UndoManager::getInstance().initialise(EditorWidgets::getInstancePtr());

		new StateManager();
		StateManager::getInstance().initialise();

		new GroupMessage();
		GroupMessage::getInstance().initialise();

		new RecentFilesManager();
		RecentFilesManager::getInstance().initialise();

		new ColourManager();
		ColourManager::getInstance().initialise();

		new EditorToolTip();
		EditorToolTip::getInstance().initialise();

		new PropertyFieldManager();
		PropertyFieldManager::getInstance().initialise();

		new Grid();
		Grid::getInstance().initialise();

		MyGUI::ResourceManager::getInstance().load("Initialise.xml");

		const SettingsSector::VectorUString& additionalResources = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalResources");
		for (SettingsSector::VectorUString::const_iterator iter = additionalResources.begin(); iter != additionalResources.end(); ++iter)
			MyGUI::ResourceManager::getInstance().load(*iter);

	/*	CommandManager::getInstance().registerCommand("Command_StatisticInfo", MyGUI::newDelegate(this, &Application::command_StatisticInfo));
		CommandManager::getInstance().registerCommand("Command_FocusVisible", MyGUI::newDelegate(this, &Application::command_FocusVisible));
		CommandManager::getInstance().registerCommand("Command_ScreenShot", MyGUI::newDelegate(this, &Application::command_ScreenShot));
		CommandManager::getInstance().registerCommand("Command_UpdateAppCaption", MyGUI::newDelegate(this, &Application::command_UpdateAppCaption));*/

		CommandManager::getInstance().registerCommand("Command_QuitApp", MyGUI::newDelegate(this, &Application::command_QuitApp));

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

		PropertyFieldManager::getInstance().shutdown();
		delete PropertyFieldManager::getInstancePtr();

		EditorToolTip::getInstance().shutdown();
		delete EditorToolTip::getInstancePtr();

		ColourManager::getInstance().shutdown();
		delete ColourManager::getInstancePtr();

		BackwardCompatibilityManager::getInstance().shutdown();
		delete BackwardCompatibilityManager::getInstancePtr();

		RecentFilesManager::getInstance().shutdown();
		delete RecentFilesManager::getInstancePtr();

		StateManager::getInstance().shutdown();
		delete StateManager::getInstancePtr();

		GroupMessage::getInstance().shutdown();
		delete GroupMessage::getInstancePtr();

		DialogManager::getInstance().shutdown();
		delete DialogManager::getInstancePtr();

		MessageBoxManager::getInstance().shutdown();
		delete MessageBoxManager::getInstancePtr();

		UndoManager::getInstance().shutdown();
		delete UndoManager::getInstancePtr();

		EditorWidgets::getInstance().shutdown();
		delete EditorWidgets::getInstancePtr();

		WidgetTypes::getInstance().shutdown();
		delete WidgetTypes::getInstancePtr();

		HotKeyManager::getInstance().shutdown();
		delete HotKeyManager::getInstancePtr();

		WidgetCreatorManager::getInstance().shutdown();
		delete WidgetCreatorManager::getInstancePtr();

		WidgetSelectorManager::getInstance().shutdown();
		delete WidgetSelectorManager::getInstancePtr();

		CommandManager::getInstance().shutdown();
		delete CommandManager::getInstancePtr();

		SettingsManager::getInstance().shutdown();
		delete SettingsManager::getInstancePtr();

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>("Layer");
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

