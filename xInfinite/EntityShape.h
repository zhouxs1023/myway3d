#pragma once

#include "Shape.h"
#include "Controller.h"

namespace xInfi {

    class EntityShape : public Shape
    {
        DECLARE_PROPERTY();

    public:
        char Name[128];
        char MeshFile[128];
        Vec3 Position;
        Vec3 Orientation;
        Vec3 Scale;

    public:
        EntityShape(const char * name);
        virtual ~EntityShape();

        virtual bool SetPosition(float x, float y, float z);
        virtual bool SetOrientation(float x, float y, float z);
        virtual bool SetScale(float x, float y, float z);

        virtual const char * GetName() { return Name; }
        virtual const char * GetTypeName() { return "Entity"; }

        virtual bool OnPropertyChanged(const Property * p);

    protected:
        void _setName(const TString128 & name);
        void _setMeshFile(const TString128 & meshFile);
        void _setPosition(const Vec3 & position);
        void _setOrientation(const Vec3 & ort);
        void _setScale(const Vec3 & scale);

    protected:
        SceneNode * mNode;
        Entity * mEntity;
    };

    class EntityShapeCreator : public ShapeFactory
    {
    public:
        EntityShapeCreator() {};
        virtual ~EntityShapeCreator() {};

        virtual Shape * CreateShape(const char * name) { return new EntityShape(name); }
        virtual const char * GetTypeName() { return "Entity"; }
        virtual const char * GetIconFile() { return ""; }
    };

    class EntityShapeCreatorListener : public EventListener
    {
    public:
        EntityShapeCreatorListener()
        {
            Controller::OnEngineInit += this;
        }

        virtual ~EntityShapeCreatorListener()
        {
            Controller::OnEngineInit -= this;
        }

        virtual void OnCall(Event * sender, void * data)
        {
            if (sender == &Controller::OnEngineInit)
            {
                ShapeManager::Instance()->AddShapeFactory(new EntityShapeCreator());
            }
        }

    };
}