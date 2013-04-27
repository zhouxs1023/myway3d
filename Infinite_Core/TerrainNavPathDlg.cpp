#include "stdafx.h"

#include "TerrainNavPathDlg.h"
#include "TerrainPane.h"

namespace Infinite {

	TerrainNavPathDlg::TerrainNavPathDlg(MyGUI::Widget * _parent)
		: BaseLayout("TerrainNavPath.layout", _parent)
	{
		assignWidget(mBrushSize, "BrushSize");
		assignWidget(mBrushOperator, "BrushOperator");

		mBrushOperator->insertItemAt(0, "Set");
		mBrushOperator->insertItemAt(1, "Clear");
		mBrushOperator->setIndexSelected(0);

		mBrushSize->setScrollRange(10);
		mBrushSize->setScrollPosition(5);

		notifyBrushSizeDensityChanged(NULL, 0);
		notifyBrushOperatorChanged(mBrushOperator, 0);

		mBrushSize->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainNavPathDlg::notifyBrushSizeDensityChanged);
		mBrushOperator->eventComboChangePosition += MyGUI::newDelegate(this, &TerrainNavPathDlg::notifyBrushOperatorChanged);
	}

	TerrainNavPathDlg::~TerrainNavPathDlg()
	{
	}

	void TerrainNavPathDlg::notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position)
	{
		size_t size = mBrushSize->getScrollPosition();

		TerrainPane::Instance()->GetTerrainNavPath()->SetBrushSize((float)size);
	}

	void TerrainNavPathDlg::notifyBrushOperatorChanged(MyGUI::ComboBox* _sender, size_t _index)
	{
		xEditTerrainNavPath::Op op = xEditTerrainNavPath::eNone;

		switch (_index)
		{
		case 0:
			op = xEditTerrainNavPath::eSet;
			break;

		case 1:
			op = xEditTerrainNavPath::eClear;
			break;
		}

		d_assert (op != xEditTerrainNavPath::eNone);

		TerrainPane::Instance()->GetTerrainNavPath()->SetOperator(op);
	}
}