#pragma once

namespace xInfi {

    class Shape : public IPropertyObj
    {
        DECLARE_PROPERTY();

    public:
        Shape(const char * name) {}
        virtual ~Shape() {}

        virtual const char * GetName() = 0;
        virtual const char * GetTypeName() { return "Shape"; }

        virtual bool SetPosition(float x, float y, float z) { return true; }
        virtual bool SetOrientation(float x, float y, float z) { return false; }
        virtual bool SetScale(float x, float y, float z) { return false; }
    };

    class ShapeFactory
    {
    public:
        ShapeFactory() {}
        virtual ~ShapeFactory() {}

        virtual Shape * CreateShape(const char * name) = 0;

        virtual const char * GetTypeName() { return "Shape"; }
        virtual const char * GetIconFile() { return ""; }
    };

    class ShapeManager : public EventListener
    {
        DECLARE_SINGLETON(ShapeManager);

    public:
        ShapeManager();
        ~ShapeManager();

        virtual void OnCall(Event * sender, void * data);

        void AddShapeFactory(ShapeFactory * sf);
        ShapeFactory * GetShapeFactory(const char * type);
        int GetShapeFactoryCount();
        ShapeFactory * GetShapeFactory(int index);

        Shape * CreateShape(const char * type);
        void DistroyShape(Shape * shape);

        Shape * GetShape(const char * name);

    protected:
        Array<ShapeFactory *> mShapeFactorys;
        Array<Shape *> mShapes;
    };
    
}