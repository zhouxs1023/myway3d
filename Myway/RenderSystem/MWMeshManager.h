#pragma once

#include "MWMeshLoader.h"

namespace Myway
{

/* class: MeshManager
----------------------------------------------------
@Remark:
Mesh Manager.
must be create a mesh by mesh manager.
----------------------------------------------------
*/
class MW_ENTRY MeshManager
{
    DECLARE_SINGLETON (MeshManager);

public:
    static const String     DEFAULT_VERTEX_SHADER;
    static const String     DEFAULT_PIXEL_SHADER;

public:
    MeshManager();
    ~MeshManager();

    MeshPtr         Load(const TString128 & name, const TString128 & source, const TString128 & group);

    MeshPtr         CreateMesh(const TString128 & sMeshName);

    MeshPtr         CreateBox(const TString128 & sMeshName,
                              float width, float height, float depth);

    MeshPtr         CreateSphere(const TString128 & sMeshName, 
                                 short iRings, short iSegments, float fRadius);

     MeshPtr        CreateCylinder(const TString128 & sName,
                                   float fRadius, float fHeight, int iRings);

    MeshPtr         CreatePlane(const TString128 & sMeshName,
                                const Plane & plane,
                                float width, float height,
                                int iTileW = 1, int iTileH = 1);

    bool            IsExist(const TString128 & sMeshName);
    MeshPtr         Find(const TString128 & sMeshName);

    void            Destroy(const TString128 & sMeshName);
    void            Destroy(Mesh * pMesh);
    void            DestoryAll();

protected:
    Array<Mesh*>   mMeshs;
};

}
