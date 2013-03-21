#include "stdafx.h"

#include "TerrainHeightDlg.h"
#include "TerrainPane.h"

namespace Infinite {

	TerrainHeightDlg::TerrainHeightDlg(MyGUI::Widget * _parent)
		: BaseLayout("TerrainHeight.layout", _parent)
	{
		assignWidget(mBrushSize, "BrushSize");
		assignWidget(mBrushDensity, "BrushDensity");
		assignWidget(mBrushType, "BrushType");
		assignWidget(mBrushOperator, "BrushOperator");

		mBrushOperator->insertItemAt(0, "Up");
		mBrushOperator->insertItemAt(1, "Down");
		mBrushOperator->insertItemAt(2, "Smooth");
		mBrushOperator->setIndexSelected(0);

		FileSystem fs("../Core/Terrain/Brushes");

		fs.Load();

		Archive::FileInfoVisitor v = fs.GetFileInfos();

		while (!v.Endof())
		{
			const Archive::FileInfo & info = v.Cursor()->second;

			if (info.type == Archive::FILE_ARCHIVE)
			{
				TString128 base = File::RemoveExternName(info.name);
				TString128 ext = File::GetExternName(info.name);

				if (ext == "png")
					mBrushType->addItem(base.c_wstr());
			}

			++v;
		}

		mBrushType->setIndexSelected(0);

		mBrushSize->setScrollRange(200);
		mBrushSize->setScrollPosition(50);

		mBrushDensity->setScrollRange(200);
		mBrushDensity->setScrollPosition(100);

		notifyBrushSizeDensityChanged(NULL, 0);
		notifyBrushTypeChanged(mBrushType, 0);
		notifyBrushOperatorChanged(mBrushOperator, 0);

		mBrushSize->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainHeightDlg::notifyBrushSizeDensityChanged);
		mBrushDensity->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainHeightDlg::notifyBrushSizeDensityChanged);

		mBrushOperator->eventComboChangePosition += MyGUI::newDelegate(this, &TerrainHeightDlg::notifyBrushOperatorChanged);
		mBrushType->eventComboChangePosition += MyGUI::newDelegate(this, &TerrainHeightDlg::notifyBrushTypeChanged);
	}

	TerrainHeightDlg::~TerrainHeightDlg()
	{
	}

	void TerrainHeightDlg::notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position)
	{
		size_t size = mBrushSize->getScrollPosition();
		size_t density = mBrushDensity->getScrollPosition();

		TerrainPane::Instance()->GetTerrainHeight()->SetBrushSize((float)size);
		TerrainPane::Instance()->GetTerrainHeight()->SetBrushDensity(density / (float)100);
	}

	void TerrainHeightDlg::notifyBrushTypeChanged(MyGUI::ComboBox* _sender, size_t _index)
	{
		const char * brush = mBrushType->getItemNameAt(_index).asUTF8_c_str();

		TString128 texture = TString128("Terrain\\Brushes\\") + brush + ".png";

		TerrainPane::Instance()->GetTerrainHeight()->SetBrush(texture);
	}

	void TerrainHeightDlg::notifyBrushOperatorChanged(MyGUI::ComboBox* _sender, size_t _index)
	{
		xEditTerrainHeight::Op op = xEditTerrainHeight::eNone;

		switch (_index)
		{
		case 0:
			op = xEditTerrainHeight::eUp;
			break;

		case 1:
			op = xEditTerrainHeight::eDown;
			break;

		case 2:
			op = xEditTerrainHeight::eSmooth;
			break;
		}

		d_assert (op != xEditTerrainHeight::eNone);

		TerrainPane::Instance()->GetTerrainHeight()->SetOperator(op);
	}
}