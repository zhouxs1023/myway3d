#pragma once

#include "MWD3D11RenderSystemEntry.h"
#include "MWD3D11Helper.h"
#include "MWTexture.h"

namespace Myway
{

class RS_ENTRY D3D11Texture : public Texture
{
    DeclareRefObj();

    friend class D3D11VideoBufferManager; 
    friend class Image;

public:
    D3D11Texture();

    virtual ~D3D11Texture();

    virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags);
    virtual void Unlock(int iLevel);

    void Load();
    void Unload();
    void Reload();
    void LoadImp(DataStreamPtr stream);

	ID3D11Texture2D * GetD3DTexture() { return mD3D11Texture; }

protected:
    ID3D11Texture2D *     mD3D11Texture;
};

class RS_ENTRY D3D11Texture3D : public Texture
{
    DeclareRefObj();

    friend class D3D11VideoBufferManager; 

public:
    D3D11Texture3D();
    virtual ~D3D11Texture3D();

    virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags);
    virtual void Unlock(int iLevel);

    void Load();
    void Unload();
    void Reload();
    void LoadImp(DataStreamPtr stream);

    ID3D11Texture3D * GetD3D11Texture() { return mD3D11Texture3D; }

protected:
    ID3D11Texture3D * mD3D11Texture3D;
};


class RS_ENTRY D3D11TextureCube : public Texture
{
    DeclareRefObj();

    friend class D3D11VideoBufferManager; 

public:
    D3D11TextureCube();
    virtual ~D3D11TextureCube();

    virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags);
    virtual void Unlock(int iLevel);

    void Load();
    void Unload();
    void Reload();
    void LoadImp(DataStreamPtr stream);

	ID3D11Texture2D * GetD3D11Texture() { return mD3D11Texture2D;}

protected:
	ID3D11Texture2D * mD3D11Texture2D;

};

}