#pragma once

#include "Infinite.h"

class INFI_ENTRY xEvent
{
public:
	static Event OnCreatePane;

	static Event OnInit;
	static Event OnInitUI;
	static Event OnShutdown;

	static Event OnSelectObj;
	static Event OnUnSelectObj;

	static Event OnUpdate;
	static Event OnUpdateGame;
	static Event OnAppModeChanged;

	static Event OnOperatorChanged;		// param0: old op, param1: new op.

	static Event OnDragFile;

	static Event OnObjCreated;
	static Event OnObjDistroy;

	static Event OnNewScene;
	static Event OnSaveScene;
	static Event OnLoadScene;
	static Event OnAfterLoadScene;
	static Event OnUnloadScene;

	static Event OnSerialize;
};