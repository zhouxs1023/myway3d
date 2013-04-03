#include "stdafx.h"

#include "TerrainWaterDlg.h"
#include "TerrainPane.h"

namespace Infinite {

	TerrainWaterDlg::TerrainWaterDlg(MyGUI::Widget * _parent)
		: BaseLayout("TerrainWater.layout", _parent)
	{
		assignWidget(mBrushSize, "BrushSize");
		assignWidget(mBrushOperator, "BrushOperator");
		assignWidget(mWaterHeight, "WaterHeight");

		mBrushOperator->insertItemAt(0, "Add");
		mBrushOperator->insertItemAt(1, "Remove");
		mBrushOperator->setIndexSelected(0);

		mBrushSize->setScrollRange(20);
		mBrushSize->setScrollPosition(5);

		notifyBrushSizeDensityChanged(NULL, 0);
		notifyBrushOperatorChanged(mBrushOperator, 0);

		mBrushSize->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainWaterDlg::notifyBrushSizeDensityChanged);
		mBrushOperator->eventComboChangePosition += MyGUI::newDelegate(this, &TerrainWaterDlg::notifyBrushOperatorChanged);
		mWaterHeight->eventEditTextChange += MyGUI::newDelegate(this, &TerrainWaterDlg::notifyWaterHeightChanged);
	}

	TerrainWaterDlg::~TerrainWaterDlg()
	{
	}

	void TerrainWaterDlg::notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position)
	{
		size_t size = mBrushSize->getScrollPosition();

		TerrainPane::Instance()->GetTerrainWater()->SetBrushSize(size);
	}

	void TerrainWaterDlg::notifyBrushOperatorChanged(MyGUI::ComboBox* _sender, size_t _index)
	{
		TerrainPane::Instance()->GetTerrainWater()->SetBrushOp(_index == 0);
	}

	void TerrainWaterDlg::notifyWaterHeightChanged(MyGUI::EditBox * _sender)
	{
		TString128 txt = _sender->getOnlyText().asUTF8_c_str();

		float height = txt.ToFloat();

		TerrainPane::Instance()->GetTerrainWater()->SetHeight(height);
	}

	void TerrainWaterDlg::_UnloadScene(Event * sender)
	{
	}

	void TerrainWaterDlg::_AfterLoadScene(Event * sender)
	{
		float height = Environment::Instance()->GetWaterManager()->GetWater()->GetHeight();

		char txt[32];
		sprintf_s(txt, 32, "%.2f", height);

		mWaterHeight->setCaption(txt);
	}
}