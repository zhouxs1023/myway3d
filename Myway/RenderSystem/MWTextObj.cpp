#include "MWTextObj.h"
#include "MWNode.h"
#include "MWRenderQueue.h"
#include "MWSceneManager.h"
#include "MWMaterialManager.h"

using namespace Myway;


DECLARE_GUID128_CONST(TextObj::guid, 't', 'e', 'x', 't', 'o', 'b', 'j', 0,
                                     0, 0, 0, 0, 0, 0, 0, 0);

TextObj::TextObj(const String & name)
: m_name(name),
  mtid(INVALID_FTID),
  m_need_update(TRUE),
  m_need_update_text(TRUE),
  m_height(1.0f),
  mont(NULL),
  mMat_world(Mat4::Identity)
{
}

TextObj::~TextObj()
{
}

const String & TextObj::GetName() const
{
    return m_name;
}

void TextObj::GetWorldPosition(Vec3 * pos)
{
    *pos = m_node->GetWorldPosition();
}

void TextObj::GetWorldTransform(Mat4 * form)
{
    *form = mMat_world;
}

void TextObj::SetFont(const String & sFontName)
{
    if (!IS_INVALID_FTID(mtid))
    {
        mont->DelRef(mtid);
        mtid = INVALID_FTID;
    }

    mont = FontManager::Instance().GetFont(sFontName);

    if (mont == NULL)
    {
        mont = FontManager::Instance().LoadFont(sFontName);
    }

    m_need_update_text = TRUE;
    m_need_update = TRUE;

    d_assert(mont);
}

void TextObj::SetTextSize(float height)
{
    m_height = height;
    m_need_update = TRUE;
}

void TextObj::SetText(const FString & text)
{
    m_text = text;

    if (!IS_INVALID_FTID(mtid))
    {
        mont->DelRef(mtid);
        mtid = INVALID_FTID;
    }

    m_need_update_text = TRUE;
    m_need_update = TRUE;
}

const guid128 & TextObj::GetGuid()
{
    return guid;
}

void TextObj::NotifyCamera(Camera * cam)
{
    Vec3 xaxis = cam->GetRight();
    Vec3 yaxis = cam->GetUp();
    Vec3 zaxis = cam->GetDirection();

    Math::VecCross(xaxis, yaxis, zaxis);
    Math::VecCross(yaxis, zaxis, xaxis);

    Math::VecNormalize(xaxis, xaxis);
    Math::VecNormalize(yaxis, yaxis);
    Math::VecNormalize(zaxis, zaxis);

    Quat quat;
    Math::QuatFromAxis(quat, xaxis, yaxis, zaxis);

    Math::MatTransform(mMat_world, m_node->GetWorldPosition(), quat, m_node->GetWorldScale());
}

void TextObj::AddRenderQueue(RenderQueue * rq)
{
    if (m_text.Length())
    {
        if (m_need_update_text)
            _UpdateText();
            

        if (m_need_update)
            _UpdateGeomtry();

        rq->AddRenderer(this);
    }
}

void TextObj::_UpdateText()
{
    mtid = mont->LoadGlyphs(m_text, m_glyphs, m_texture);
    m_Material->GetTechnique(0)->GetPass(0)->GetTextureStage(0)->SetTexture(m_texture);

    if (!IS_INVALID_FTID(mtid))
        mont->AddRef(mtid);

    m_need_update_text = FALSE;
}

void TextObj::_UpdateGeomtry()
{
    assert(mont);

    int vertex_count = m_text.Length() * 4;
    int index_count = m_text.Length() * 6;
    int prim_count = m_text.Length() * 2;

    debug_assert(index_count < 65536, "string is to large.");

    VertexDeclarationPtr decl = mVertexStream.GetDeclaration();

    if (decl.IsNull())
    {
        decl = VideoBufferManager::Instance().CreateVertexDeclaration();
        decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
        decl->AddElement(0, 12, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD, 0);
        decl->EndDecl();

        mVertexStream.SetDeclaration(decl);
    }

    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(20 * vertex_count, 
                                                                               USAGE_WRITEONLY,
                                                                               POOL_MANAGED);

    float * verteces;
    buffer->Lock(0, 0, (void**)&verteces, LOCK_DISCARD);

    List<ftglyph>::Iterator iter;
    List<ftglyph>::Iterator end;

    iter = m_glyphs.Begin();
    end = m_glyphs.End();

    float posx0 = 0, posy0 = 0, posx1 = 0, posy1 = 0;

    while (iter != end)
    {
        posx0 = posx1;
        posy0 = m_height;
        posx1 += iter->aspect * m_height;
        posy1 = 0;

        //vertex 0
        *verteces++ = posx0;
        *verteces++ = posy0;
        *verteces++ = 0;
        *verteces++ = iter->u0;
        *verteces++ = iter->v0;

        //vertex 1
        *verteces++ = posx1;
        *verteces++ = posy0;
        *verteces++ = 0;
        *verteces++ = iter->u1;
        *verteces++ = iter->v0;

        //vertex 2
        *verteces++ = posx0;
        *verteces++ = posy1;
        *verteces++ = 0;
        *verteces++ = iter->u0;
        *verteces++ = iter->v1;

        //vertex 3
        *verteces++ = posx1;
        *verteces++ = posy1;
        *verteces++ = 0;
        *verteces++ = iter->u1;
        *verteces++ = iter->v1;

        ++iter;
    }

    buffer->Unlock();

    mVertexStream.Bind(0, buffer, 20);

    IndexBufferPtr ibuffer = VideoBufferManager::Instance().CreateIndexBuffer(sizeof(short) * index_count,
                                                                              USAGE_WRITEONLY,
                                                                              POOL_MANAGED,
                                                                              FMT_INDEX16);
    short * indeces;
    ibuffer->Lock(0, 0, (void**)&indeces, LOCK_DISCARD);

    for (short i = 0; i < (short)vertex_count; i += 4)
    {
        *indeces++ = i;
        *indeces++ = i + 1;
        *indeces++ = i + 2;

        *indeces++ = i + 2;
        *indeces++ = i + 1;
        *indeces++ = i + 3;
    }

    ibuffer->Unlock();

    mVertexStream.SetCount(vertex_count);
    mIndexStream.SetCount(index_count);

    SetPrimitiveCount(prim_count);
    SetPrimitiveType(PRIM_TRIANGLELIST);

    m_need_update = FALSE;
}