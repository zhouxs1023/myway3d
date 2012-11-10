#include "stdafx.h"
#include "Shape.h"
#include "Controller.h"

namespace xInfi {

    DF_PROPERTY_BEGIN(Shape)
    DF_PROPERTY_END();



    ShapeManager gShapeMgr;

    IMP_SLN(ShapeManager);

    ShapeManager::ShapeManager()
    {
        INIT_SLN;
        Controller::OnEngineDeInit += this;
    }

    ShapeManager::~ShapeManager()
    {
        Controller::OnEngineDeInit -= this;
        SHUT_SLN;
    }

    void ShapeManager::OnCall(Event * sender, void * data)
    {
        if (sender == &Controller::OnEngineDeInit)
        {
            for (int i = 0; i < mShapeFactorys.Size(); ++i)
            {
                delete mShapeFactorys[i];
            }

            for (int i = 0; i < mShapes.Size(); ++i)
            {
                delete mShapes[i];
            }

            mShapeFactorys.Clear();
            mShapes.Clear();
        }
    }

    void ShapeManager::AddShapeFactory(ShapeFactory * sf)
    {
        d_assert (GetShapeFactory(sf->GetTypeName()) == NULL);

        mShapeFactorys.PushBack(sf);
    }

    ShapeFactory * ShapeManager::GetShapeFactory(const char * type)
    {
        for (int i = 0; i < mShapeFactorys.Size(); ++i)
        {
            if (Strcmp(type, mShapeFactorys[i]->GetTypeName()) ==0)
                return mShapeFactorys[i];
        }

        return NULL;
    }

    int ShapeManager::GetShapeFactoryCount()
    {
        return mShapeFactorys.Size();
    }

    ShapeFactory * ShapeManager::GetShapeFactory(int index)
    {
        d_assert (index < GetShapeFactoryCount());
        return mShapeFactorys[index];
    }


    Shape * ShapeManager::CreateShape(const char * type)
    {
        ShapeFactory * sf = GetShapeFactory(type);

        d_assert (sf);

        int uId = 0;
        TString128 uName = "Shape";
        TString128 uShapeName;
        bool flag = true;

        while (1)
        {
            uShapeName = uName + uId++;

            if (GetShape(uShapeName.c_str()) == NULL)
                break;
        }

        Shape * shape = sf->CreateShape(uShapeName.c_str());

        mShapes.PushBack(shape);

        return shape;
    }

    void ShapeManager::DistroyShape(Shape * shape)
    {
        for (int i = 0; i < mShapes.Size(); ++i)
        {
            if (shape == mShapes[i])
            {
                delete mShapes[i];
                return ;
            }
        }

        d_assert (0);
    }

    Shape * ShapeManager::GetShape(const char * name)
    {
        for (int i = 0; i < mShapes.Size(); ++i)
        {
            if (Strcmp(name, mShapes[i]->GetName()) == 0)
                return mShapes[i];
        }

        return NULL;
    }
}