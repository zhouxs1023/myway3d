#include "MWBillboard.h"
#include "MWSceneManager.h"


using namespace Myway;


Billboard::Billboard()
: mosition(Vec3::Zero),
  m_texcoord0(Vec2::Zero),
  m_texcoord1(Vec2::Unit),
  m_color(Color4::White),
  m_width(0),
  m_height(0),
  m_align(BAT_CENTER_CENTER),
  m_bVisible(TRUE)
{
}

Billboard::~Billboard()
{
}

void Billboard::SetPosition(const Vec3 & pos)
{
    mosition = pos;
}

void Billboard::SetTexcoord(float left, float top, float right, float bottom)
{
    m_texcoord0.x = left;
    m_texcoord0.y = top;

    m_texcoord1.x = right;
    m_texcoord1.y = bottom;
}

void Billboard::SetColor(const Color4 & color)
{
    m_color = color;
}

void Billboard::SetWidth(float width)
{
    m_width = width;
}

void Billboard::SetHeight(float height)
{
    m_height = height;
}

float Billboard::GetWidth()
{
    return m_width;
}

float Billboard::GetHeight()
{
    return m_height;
}

void Billboard::SetAlignType(BilloardAlignType bat)
{
    m_align = bat;
}

BilloardAlignType Billboard::GetAlignType()
{
    return m_align;
}

void Billboard::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

bool Billboard::GetVisible()
{
    return m_bVisible;
}

void Billboard::GetVerteces(void * verteces, const Vec3 & right, const Vec3 & up, const Vec3 & dir)
{
    struct _vertex
    {
        Vec3    position;
        Color4  color;
        Vec2    texcoord;
    };

    _vertex * vert = (_vertex*)verteces;

    Vec3 width_x_right = m_width * right;
    Vec3 height_x_up = m_height * up;

    if (m_align == BAT_LEFT_TOP)
    {
        vert[0].position    = mosition - width_x_right + height_x_up;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + width_x_right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - height_x_up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - height_x_up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_LEFT_CENTER)
    {
        vert[0].position    = mosition - width_x_right + 0.5f * height_x_up;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + width_x_right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - height_x_up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - height_x_up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_LEFT_BOTTOM)
    {
        vert[0].position    = mosition - width_x_right;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + width_x_right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - height_x_up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - height_x_up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_CENTER_TOP)
    {
        vert[0].position    = mosition - 0.5f * width_x_right + height_x_up;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + width_x_right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - height_x_up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - height_x_up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_CENTER_CENTER)
    {
        vert[0].position    = mosition - 0.5f * width_x_right + 0.5f * height_x_up;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + width_x_right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - height_x_up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - height_x_up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_CENTER_BOTTOM)
    {
        vert[0].position    = mosition - 0.5f * width_x_right;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + width_x_right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - height_x_up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - height_x_up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_RIGHT_TOP)
    {
        vert[0].position    = mosition + height_x_up;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + m_width * right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - m_height * up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - m_height * up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_RIGHT_CENTER)
    {
        vert[0].position    = mosition + 0.5f * height_x_up;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + m_width * right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - m_height * up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - m_height * up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }

    else if (m_align == BAT_RIGHT_BOTTOM)
    {
        vert[0].position    = mosition;
        vert[0].color       = m_color;
        vert[0].texcoord.x  = m_texcoord0.x;
        vert[0].texcoord.y  = m_texcoord0.y;

        vert[1].position    = vert[0].position + m_width * right;
        vert[1].color       = m_color;
        vert[1].texcoord.x  = m_texcoord1.x;
        vert[1].texcoord.y  = m_texcoord0.y;

        vert[2].position    = vert[0].position - m_height * up;
        vert[2].color       = m_color;
        vert[2].texcoord.x  = m_texcoord0.x;
        vert[2].texcoord.y  = m_texcoord1.y;

        vert[3].position    = vert[1].position - m_height * up;
        vert[3].color       = m_color;
        vert[3].texcoord.x  = m_texcoord1.x;
        vert[3].texcoord.y  = m_texcoord1.y;
    }
}




DECLARE_GUID128_CONST(BillboardSet::Guid, 'b', 'i', 'l', 'l', 'b', 'o', 'a', 'r',
                                          'd', 's', 'e', 't', 0, 0, 0, 0);

BillboardSet::BillboardSet(const String & sName)
: m_sName(sName)
{
}

BillboardSet::~BillboardSet()
{
    DestroyAllBillboard();
}

const String & BillboardSet::GetName() const
{
    return m_sName;
}

Billboard * BillboardSet::CreateBillboard()
{
    Billboard * board = new Billboard();

    m_Billboards.PushBack(board);

    return board;
}

Billboard * BillboardSet::CreateBillboard(float width, float height,
                                          const Vec3 & position,
                                          const Vec2 & uvLT, const Vec2 & uvRB,
                                          const Color4 & color)
{
    Billboard * board = CreateBillboard();

    board->SetWidth(width);
    board->SetHeight(height);
    board->SetPosition(position);
    board->SetTexcoord(uvLT.x, uvLT.y, uvRB.x, uvRB.y);
    board->SetColor(color);

    return board;
}

Billboard * BillboardSet::GetBillboard(int index)
{
    assert (index < m_Billboards.Size());

    List<Billboard*>::Iterator iter;

    iter = m_Billboards.Begin();

    while (index--)
    {
        ++iter;
    }

    return *iter;
}

void BillboardSet::DestroyBillboard(Billboard * board)
{
    List<Billboard*>::Iterator iter;
    List<Billboard*>::Iterator end;

    iter = m_Billboards.Begin();
    end = m_Billboards.End();

    while (iter != end && *iter != board)
    {
        ++iter;
    }

    assert(iter != end);

    delete board;
    m_Billboards.Erase(iter);
}

void BillboardSet::DestroyBillboard(int index)
{
    assert (index < m_Billboards.Size());

    List<Billboard*>::Iterator iter;

    iter = m_Billboards.Begin();

    while (index--)
    {
        ++iter;
    }

    delete *iter;
    m_Billboards.Erase(iter);
}

void BillboardSet::DestroyAllBillboard()
{
    List<Billboard*>::Iterator iter;
    List<Billboard*>::Iterator end;

    iter = m_Billboards.Begin();
    end = m_Billboards.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    m_Billboards.Clear();
}

int BillboardSet::GetNumBillboard()
{
    return m_Billboards.Size();
}

const guid128 & BillboardSet::GetGuid()
{
    return Guid;
}

void BillboardSet::NotifyCamera(Camera * cam)
{
    int max_vcount = m_Billboards.Size() * 4;
    int max_icount = m_Billboards.Size() * 6;

    debug_assert(max_icount < 65536, "index buffer is to large.");

    int active_count = 0;

    VertexBufferPtr buffer = mVertexStream.GetStream(0);

    if (buffer.IsNull())
    {
        VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
        decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
        decl->AddElement(0, 12, DECLTYPE_FLOAT4, DECLMETHOD_DEFAULT, DECLUSAGE_COLOR, 0);
        decl->AddElement(0, 28, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD, 0);
        decl->EndDecl();

        mVertexStream.SetDeclaration(decl);
    }

    if (buffer.IsNull() || buffer->GetSize() < max_vcount)
    {
        buffer = VideoBufferManager::Instance().CreateVertexBuffer(max_vcount * 36,
                                                                   USAGE_WRITEONLY | USAGE_DYNAMIC,
                                                                   POOL_DEFAULT);
        mVertexStream.Bind(0, buffer, 36);
    }

    char * verteces;
    buffer->Lock(0, 0, (void**)&verteces, LOCK_DISCARD | LOCK_NOOVERWRITE);

    const Vec3 & right = cam->GetRight();
    const Vec3 & up = cam->GetUp();
    const Vec3 & dir = cam->GetDirection();

    List<Billboard*>::Iterator iter;
    List<Billboard*>::Iterator end;

    iter = m_Billboards.Begin();
    end = m_Billboards.End();

    while (iter != end)
    {
        if ((*iter)->GetVisible())
        {
            (*iter)->GetVerteces(verteces, right, up, dir);

            ++active_count;
            verteces += 36 * 4;
        }

        ++iter;
    }

    buffer->Unlock();

    IndexBufferPtr ibuffer = mIndexStream.GetStream();

    if (ibuffer.IsNull() || ibuffer->GetSize() < max_icount)
    {
        ibuffer = VideoBufferManager::Instance().CreateIndexBuffer(max_icount * sizeof(short), 
                                                                   USAGE_WRITEONLY | USAGE_DYNAMIC,
                                                                   POOL_DEFAULT, FMT_INDEX16);

        mIndexStream.Bind(ibuffer, 0);
    }

    short * indeces;
    ibuffer->Lock(0, 0, (void**)&indeces, LOCK_DISCARD);

    for (short i = 0; i < (short)(active_count * 4); i += 4)
    {
        *indeces++ = i;
        *indeces++ = i + 1;
        *indeces++ = i + 2;

        *indeces++ = i + 2;
        *indeces++ = i + 1;
        *indeces++ = i + 3;
    }

    ibuffer->Unlock();

    mVertexStream.SetCount(max_vcount);
    mIndexStream.SetCount(max_icount);
    mPrimCount = max_icount / 3;
    mPrimType = PRIM_TRIANGLELIST;
}

void BillboardSet::AddRenderQueue(RenderQueue * rq)
{
    rq->AddRenderer(this);
}

void BillboardSet::GetWorldPosition(Vec3 * pos)
{
    *pos = m_node->GetWorldPosition();
}

void BillboardSet::GetWorldTransform(Mat4 * form)
{ 
    *form = m_node->GetWorldMatrix();
}

LightList * BillboardSet::GetLightList()
{
    return &m_lights;
}