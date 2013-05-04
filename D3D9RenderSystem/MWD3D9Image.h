#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWImage.h"

namespace Myway
{

class RS_ENTRY D3D9Image : public Image
{
    DeclareRefObj();

    friend class D3D9VideoBufferManager;

public:
    D3D9Image();
    ~D3D9Image();

    void SetA8R8G8B8(int x, int y, Color c);
    void SetX8R8G8B8(int x, int y, Color c);
    void SetR8G8B8(int x, int y, Color c);
    void SetLuminance8(int x, int y, unsigned char l);
    void SetLuminance16(int x, int y, short l);

    Color GetA8R8G8B8(int x, int y) const;
    Color GetX8R8G8B8(int x, int y) const;
    Color GetR8G8B8(int x, int y) const;
    unsigned char GetLuminance8(int x, int y) const;
    short GetLuminance16(int x, int y) const;

    int GetWidth() const;
    int GetHeight() const;
    int GetSrcWidth() const;
    int GetSrcHeight() const;
    FORMAT GetFormat() const;
    int GetMipmapLevel() const;

    void Lock(LockedRect & rect);
    void Unlock();

    IDirect3DTexture9 * _MyTexture() { return mD3D9Texture; } 

protected:
    IDirect3DTexture9 * mD3D9Texture;
};

}