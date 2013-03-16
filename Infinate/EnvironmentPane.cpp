#include "stdafx.h"

#include "EnvironmentPane.h"
#include "xScene.h"
#include "xEnvironment.h"

namespace Infinite {

	EnvironmentPane::EnvironmentPane(MyGUI::Widget * _parent)
		: BaseLayout("Environment.layout", _parent)
		, OnEnvChanged(xEnvironment::OnChanged, this, &EnvironmentPane::_OnEnvChanged)
	{
		assignBase(mNight, "Night");
		assignBase(mMorning, "Morning");
		assignBase(mNoon, "Noon");
		assignBase(mEvening, "Evening");
		assignBase(mGlobal, "Global");

		assignWidget(mTab, "TabControl");

		mTab->eventTabChangeSelect += MyGUI::newDelegate(this, &EnvironmentPane::OnTabChangeSelect);

		mTab->setIndexSelected(1);
	}

	EnvironmentPane::~EnvironmentPane()
	{
		mTab->eventTabChangeSelect -= MyGUI::newDelegate(this, &EnvironmentPane::OnTabChangeSelect);
	}

	void EnvironmentPane::OnTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
	{
		if (_index >= EVKT_Night && _index <= EVKT_Evening)
			Environment::Instance()->SetKey((EvKeyType)_index);
	}

	void EnvironmentPane::_OnEnvChanged(Event * _sender)
	{
		EvKeyFrame * kNight = Environment::Instance()->GetKeyFrame(EVKT_Night);
		EvKeyFrame * kMorning = Environment::Instance()->GetKeyFrame(EVKT_Morning);
		EvKeyFrame * kNoon = Environment::Instance()->GetKeyFrame(EVKT_Noon);
		EvKeyFrame * kEvening = Environment::Instance()->GetKeyFrame(EVKT_Evening);
		EvGlobalParam * kGlobal = Environment::Instance()->GetGlobalParam();

		mNight->SetObject(kNight);
		mMorning->SetObject(kMorning);
		mNoon->SetObject(kNoon);
		mEvening->SetObject(kEvening);
		mGlobal->SetObject(kGlobal);
	}
}
