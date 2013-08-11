#include "stdafx.h"
#include "GUI_Util.h"
#include "Game.h"

IMP_SLN(MGUI_Util);

MGUI_Util::MGUI_Util()
	: EvPostUpdateScene(RenderEvent::OnPostRenderEvent, this, &MGUI_Util::OnPostUpdateScene_)
	, EvAfterDefferedShading(RenderEvent::OnAfterDefferedShading, this, &MGUI_Util::OnAfterDefferedShading_)
{
	msInstance = this;

	mRenderOp = NULL;
	mTech = NULL;
}

MGUI_Util::~MGUI_Util()
{
	msInstance = NULL;
}

void MGUI_Util::Init()
{
	// vertex buffer
	mText3DVB = VideoBufferManager::Instance()->CreateVertexBuffer(
		TEXT_VB_SIZE * sizeof(TextVertex), sizeof(TextVertex), USAGE_DYNAMIC);

	// index buffer
	mText3DIB = VideoBufferManager::Instance()->CreateIndexBuffer(TEXT_IB_SIZE * sizeof(short));

	short * idx = (short *)mText3DIB->Lock(0, 0, LOCK_NORMAL);
	for (int i = 0; i < MAX_TEXT_SIZE; ++i)
	{
		*idx++ = i * 4 + 0;
		*idx++ = i * 4 + 1;
		*idx++ = i * 4 + 2;

		*idx++ = i * 4 + 2;
		*idx++ = i * 4 + 1;
		*idx++ = i * 4 + 3;
	}
	mText3DIB->Unlock();

	// render op
	mRenderOp = new RenderOp;
	mRenderOp->vxStream.Bind(0, mText3DVB);
	mRenderOp->ixStream.Bind(mText3DIB);

	mTech = Game::Game::Instance()->GetShaderLib()->GetTechnique("NamePad");
	d_assert (mTech);
}

void MGUI_Util::Shutdown()
{
	delete mRenderOp;

	mText3DVB = NULL;
	mText3DIB = NULL;
	mRenderOp = NULL;
}

void MGUI_Util::RenderText3D(const char * str, const char * fontName, const Vec3 & position, float charHeight, const Color4 & color)
{
	int len = strlen(str);
	if (len == 0 && len >= MAX_TEXT_SIZE)
		return ;

	Text3D text;

	text.font = MyGUI::FontManager::getInstance().getByName(fontName);
	d_assert (text.font != NULL);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, text.wstr, MAX_TEXT_SIZE);

	text.position = position;
	text.charHeight = charHeight;
		
	mText3DArray.PushBack(text);
}

struct TextQSortFunction
{
	bool operator ()(const MGUI_Util::Text3D * a, const MGUI_Util::Text3D * b) const
	{
		return (intptr_t)a->font < (intptr_t)b->font;
	};
};

void MGUI_Util::_PrepareForRender()
{
	if (mText3DArray.Empty())
		return ;

	mText3DSortedArray.Clear();
	mText3DPrimtive.Clear();

	for (int i = 0; i < mText3DArray.Size(); ++i)
	{
		mText3DSortedArray.PushBack(&mText3DArray[i]);
	}

	Sort<Text3D*, TextQSortFunction>(&mText3DSortedArray[0], mText3DSortedArray.Size(), TextQSortFunction());

	TextVertex * vert = (TextVertex *)mText3DVB->Lock(0, 0, LOCK_DISCARD);

	MyGUI::IFont * font = mText3DSortedArray[0]->font;
	MyGUI::GlyphInfo * gryph[MAX_TEXT_SIZE];
	Camera * cam = World::Instance()->MainCamera();
	Vec3 right = cam->GetRight();
	Vec3 up = cam->GetUp();

	int count = 0;
	int startVert = 0;

	for (int i = 0; i < mText3DSortedArray.Size(); ++i)
	{
		Text3D * t = mText3DSortedArray[i];

		// check visible
		int len = wcslen(t->wstr);
		float width = 0;

		for (int j = 0; j < len; ++j)
		{
			wchar_t wc = t->wstr[j];
			MyGUI::GlyphInfo * gh = font->getGlyphInfo(wc);

			gryph[j] = gh;

			if (gh->height > 0.0001f)
				width += gh->width / gh->height * t->charHeight;
		}

		if (width == 0 && !IsVisible(t->position, Size2F(width, t->charHeight)))
			continue;

		// fill geometry
		Vec3 kPos = t->position - 0.5f * width * right + t->charHeight * up;

		for (int j = 0; j < len; ++j)
		{
			MyGUI::GlyphInfo * gh = gryph[j];

			if (gh->height > 0)
			{
				float w = t->charHeight * gh->width / gh->height;
				float h = t->charHeight;
				int _index = count * 4;

				vert[_index + 0].position = kPos;
				vert[_index + 0].color = t->color;
				vert[_index + 0].u = gh->uvRect.left;
				vert[_index + 0].v = gh->uvRect.top;

				vert[_index + 1].position = kPos + w * right;
				vert[_index + 1].color = t->color;
				vert[_index + 1].u = gh->uvRect.right;
				vert[_index + 1].v = gh->uvRect.top;

				vert[_index + 2].position = kPos - h * up;
				vert[_index + 2].color = t->color;
				vert[_index + 2].u = gh->uvRect.left;
				vert[_index + 2].v = gh->uvRect.bottom;

				vert[_index + 3].position = kPos + w * right - h * up;
				vert[_index + 3].color = t->color;
				vert[_index + 3].u = gh->uvRect.right;
				vert[_index + 3].v = gh->uvRect.bottom;

				count += 1;
			}
		}

		if (t->font != font || i == mText3DSortedArray.Size() - 1)
		{
			TextPrimitive prim;

			prim.font = font;
			prim.startVertex = startVert;
			prim.primCount = count * 2;

			mText3DPrimtive.PushBack(prim);

			startVert += count * 4;
			font = t->font;
		}
	}

	mText3DVB->Unlock();
}

void MGUI_Util::_RenderText3DImp()
{
	for (int i = 0; i < mText3DPrimtive.Size(); ++i)
	{
		const TextPrimitive & prim = mText3DPrimtive[i];

		mRenderOp->vxStream.SetStart(prim.startVertex);
		mRenderOp->iPrimCount = prim.primCount;

		RenderSystem::Instance()->Render(mTech, mRenderOp);
	}
}

bool MGUI_Util::IsVisible(const Vec3 & position, const Size2F & size)
{
	Camera * cam = World::Instance()->MainCamera();

	Aabb bound;
	Vec3 right = cam->GetRight();
	Vec3 up = cam->GetUp();
	Vec3 halfVec = 0.5f * size.w * right;

	bound.minimum = position - halfVec;
	bound.maximum = position + halfVec +  + size.h * up;

	return cam->IsVisible(bound);
}

void MGUI_Util::OnPostUpdateScene_(Event * sender)
{
	IGameMode * pCurrentMode = Game::Game::Instance()->GetMode();

	if (pCurrentMode == NULL)
		return ;

	for (int i = 0; i < pCurrentMode->GetObjectCount(); ++i)
	{
		IGameObject * obj = pCurrentMode->GetObjectByIndex(i);

		if (obj->IsVisible())
		{
			RenderText3D(obj->GetName(), "MYSH_10", obj->GetPosition() + Vec3(0, 10, 0), 0.5f, Color4::Green);
		}
	}

	_PrepareForRender();
}

void MGUI_Util::OnAfterDefferedShading_(Event * sender)
{
	_RenderText3DImp();
}
