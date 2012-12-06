#pragma once

#include "xEditor.h"

class X_ENTRY xEvent
{
public:
	static Event OnCreatePane;

	static Event OnInit;
	static Event OnInitUI;
	static Event OnShutdown;
	static Event OnSelectObj;
	static Event OnUnSelectObj;
	static Event OnUpdate;
	static Event OnDragFile;
	static Event OnObjCreated;
	static Event OnObjDistroy;

	static Event OnNewScene;
	static Event OnSaveScene;
	static Event OnLoadScene;
	static Event OnAfterLoadScene;
	static Event OnUnloadScene;
	static Event OnSerialize;

	static Event OnOperatorBarChecked;
};