#include "MWLight.h"
#include "MWMaterialManager.h"
#include "MWNode.h"

using namespace Myway;

Light::Light(const TString128 & sName)
: Mover(sName),
  mDiffuse(Color4::White),
  mSpecular(Color4::Black),
  mRange(0),
  mFalloff(0),
  mAtten0(1),
  mAtten1(0),
  mAtten2(0),
  mInner(1),
  mOuter(0),
  mType(LT_POINT)
{
}

Light::~Light()
{
}