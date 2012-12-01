#pragma once

#include "MWTString.h"
#include "MWMath.h"

namespace Myway {

    enum PropertyType {
        PT_Bool,
        PT_Int,
        PT_Float,
        PT_Vec2,
        PT_Vec3,
        PT_Vec4,
        PT_Color,
        PT_Filename,
		PT_String,
		PT_TString,

        PT_UNKNOWN,
    };
    

    struct MW_ENTRY Property
    {
        TString128 name;
        TString128 group;
        TString128 displayName;
        PropertyType type;
        int offset;
        int size;

        Property(const TString128 & _name, const TString128 & _group, const TString128 & _displayName, PropertyType _type, int _offset, int _size)
            : name(_name)
            , group(_group)
            , displayName(_displayName)
            , type(_type)
            , offset(_offset)
            , size(_size)
        {
        }

        bool AsBool(const void * data) const
        {
            d_assert (type == PT_Bool);
            return *(const bool *)data;
        }

        int AsInt(const void * data) const
        {
            d_assert (type == PT_Bool);
            return *(const int *)data;
        }

        float AsFloat(const void * data) const
        {
            d_assert (type == PT_Float);
            return *(const float *)data;
        }

        Vec2 AsVec2(const void * data) const
        {
            d_assert (type == PT_Vec2);
            return *(const Vec2 *)data;
        }

        Vec3 AsVec3(const void * data) const
        {
            d_assert (type == PT_Vec3);
            return *(const Vec3 *)data;
        }

        Vec4 AsVec4(const void * data) const
        {
            d_assert (type == PT_Vec4);
            return *(const Vec4 *)data;
        }

        Color4 AsColor(const void * data) const
        {
            d_assert (type == PT_Color);
            return *(const Color4 *)data;
        }

        const char * AsFilename(const void * data) const
        {
            d_assert (type == PT_Filename);
            return (const char *)data;
        }

        const char * AsString(const void * data) const
        {
            d_assert (type == PT_String);
            return (const char *)data;
        }

		template <int size>
		TString<size> AsTString(const void * data) const
		{
			d_assert (type == PT_TString);
			return *(TString<size>*)data;
		}

    };

#define DF_PROPERTY_BEGIN(classname) const Property classname::msPropertys[] = {
#define DF_PROPERTY_END() Property("unused", "unused", "unused", PT_UNKNOWN, -1, 0) };

#define DF_PROPERTY(classname, varname, group, displayName, type, size) Property(#varname, group, displayName, type, offsetof(classname, varname), size),


#define DECLARE_PROPERTY(superClass)								\
protected:                                                          \
    static const Property msPropertys[];                            \
                                                                    \
public:                                                             \
    virtual const Property * GetProperty(const char * name)         \
    {                                                               \
        int index = 0;                                              \
        while (msPropertys[index].type != PT_UNKNOWN)               \
        {                                                           \
            if (msPropertys[index].name == name)                    \
                return &msPropertys[index];                         \
                                                                    \
            ++index;                                                \
        }                                                           \
																	\
		return superClass::GetProperty(name);						\
    }                                                               \
                                                                    \
    virtual const Property * GetProperty(int index)                 \
    {                                                               \
		int superSize = superClass::GetPropertySize();				\
		int mySize = _GetPropertySize();							\
		if (index < superSize)										\
			return superClass::GetProperty(index);					\
																	\
        return &msPropertys[index - superSize];                     \
    }                                                               \
                                                                    \
    virtual int GetPropertySize()                                   \
	{																\
        return _GetPropertySize() + superClass::_GetPropertySize(); \
    }                                                               \
                                                                    \
    virtual const void * GetPropertyData(const Property * p)        \
    {                                                               \
        return (const char *)this + p->offset;                      \
    }                                                               \
                                                                    \
    virtual bool SetPropertyData(const Property * p, const void * data)     \
    {                                                               \
        int size = p->size;                                         \
        char * addr = (char *)this + p->offset;                     \
        memcpy(addr, data, size);                                   \
        return OnPropertyChanged(p);                                \
    }																\
																	\
	int _GetPropertySize()											\
	{                                                               \
		int size = 0;												\
		while (msPropertys[size].type != PT_UNKNOWN)				\
			++size;													\
		return size;                                                \
	}																




    struct MW_ENTRY IPropertyObj
    {
    public:
        IPropertyObj() {}
        virtual ~IPropertyObj() {}

		virtual int GetPropertySize() { return 0; };
		virtual const Property * GetProperty(int index) { return NULL; }
		virtual const Property * GetProperty(const char * name) { return NULL; }
		virtual const void * GetPropertyData(const Property * p)  { return NULL; }
        virtual bool SetPropertyData(const Property * p, const void * data) { return false; }

        virtual bool OnPropertyChanged(const Property * p) { return true; }

				int _GetPropertySize() { return 0; }
    };
}
