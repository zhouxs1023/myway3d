#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWTexture.h"

namespace Myway
{

/* class: D3D9Texture
--------------------------------------------------------------------------------------
    @Remark:
        Texture For Direct3D 9.0.
--------------------------------------------------------------------------------------
*/
class RS_ENTRY D3D9Texture : public Texture
{
    DECLARE_ALLOC();

    DeclareRefObj();
    DeclareD3DResource();

    friend class D3D9VideoBufferManager; 
    friend class Image;

public:
    D3D9Texture(IDirect3DDevice9 * Device);

    virtual ~D3D9Texture();

    virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags);
    virtual void Unlock(int iLevel);

    void Load();
    void Unload();
    void Reload();
    void LoadImp(DataStreamPtr stream);

    IDirect3DTexture9 * GetD3DTexture() { return mD3D9Texture; }

protected:
    IDirect3DDevice9 *      mD3D9Device;
    IDirect3DTexture9 *     mD3D9Texture;
};

class RS_ENTRY D3D9VolumeTexture : public Texture
{
    DeclareRefObj();
    DeclareD3DResource();

    friend class D3D9VideoBufferManager; 

public:
    D3D9VolumeTexture(IDirect3DDevice9 * Device);

    virtual ~D3D9VolumeTexture();

    virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags);
    virtual void Unlock(int iLevel);

    void Load();
    void Unload();
    void Reload();
    void LoadImp(DataStreamPtr stream);

    IDirect3DVolumeTexture9 * GetD3D9Texture() { return mD3D9VolumeTexture; }

protected:
    IDirect3DDevice9 *          mD3D9Device;
    IDirect3DVolumeTexture9 *   mD3D9VolumeTexture;
};


class RS_ENTRY D3D9CubeTexture : public Texture
{
    DeclareRefObj();
    DeclareD3DResource();

    friend class D3D9VideoBufferManager; 

public:
    D3D9CubeTexture(IDirect3DDevice9 * Device);

    virtual ~D3D9CubeTexture();

    virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags);
    virtual void Unlock(int iLevel);

    void Load();
    void Unload();
    void Reload();
    void LoadImp(DataStreamPtr stream);

    IDirect3DCubeTexture9 * GetD3D9Texture() { return mD3D9CubeTexture;}

protected:
    IDirect3DDevice9 *      mD3D9Device;
    IDirect3DCubeTexture9 * mD3D9CubeTexture;
};

}