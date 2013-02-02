/*
	EMotion FX 2 - Character Animation System
	(c)Copyright 2001-2004 Mystic Game Development (http://www.mysticgd.com)
	All rights reserved.

	NOTE:
	Since you are looking at this file it most likely means you have licensed or are evaluating EMotion FX.
	If this is not the case, please send us an email immediately at legal@mysticgd.com so that we can find out
	who has leaked out our code.

	Main include header.
*/

#ifndef __EMOTIONFX_H
#define __EMOTIONFX_H

// automatically link the EMotion FX library?
// NOTE: this does not define if Core is being linked automatically or not, see Core.h for more info
// #define AUTOLINK_EMOTIONFX

// when automatically linking is enabled, always link the release version?
//#define EMOTIONFX_ALWAYS_LINK_RELEASE


// nodes
#include "Actor.h"
#include "Node.h"
#include "NodeTransform.h"
#include "NodeIDGenerator.h"
#include "NodeAttribute.h"
#include "NodeLimitAttribute.h"
#include "NodeDOF.h"
#include "NodePhysicsAttribute.h"

// geometry
#include "SimpleMesh.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "HwShaderBuffer.h"
#include "Material.h"
#include "StandardMaterial.h"
#include "FXMaterial.h"

#include "VertexAttributeLayer.h"
#include "UVVertexAttributeLayer.h"
#include "SkinningInfoVertexAttributeLayer.h"

// mesh deformers
#include "MeshDeformer.h"
#include "MeshDeformerStack.h"
#include "SoftSkinDeformer.h"
#include "SoftSkinDeformerSSE.h"
#include "SoftSkinManager.h"
#include "SmartMeshMorphDeformer.h"

// keyframing
#include "KeyFrame.h"
#include "KeyTrack.h"
#include "KeyFrameFinder.h"
#include "CompressedKeyFrames.h"

// keyframe generators
#include "KeyFrameGenerator.h"
#include "EyeBlinkKeyFrameGenerator.h"

// interpolators
#include "LinearInterpolators.h"
#include "BezierInterpolators.h"
#include "HermiteInterpolators.h"
#include "TCBSplineInterpolators.h"

// motion system
#include "Motion.h"
#include "MotionSystem.h"
#include "MotionLayerSystem.h"
#include "MotionInstance.h"
#include "MotionLayer.h"
#include "MotionLink.h"
#include "MotionQueue.h"
#include "MotionPart.h"
#include "PlayBackInfo.h"
#include "LayerPass.h"
#include "FacialLayerPass.h"
#include "RepositioningLayerPass.h"

// play modes
#include "PlayMode.h"
#include "PlayModeForward.h"
#include "PlayModeBackward.h"
#include "PlayModePingPong.h"

// skeletal motion
#include "SkeletalMotion.h"

// facial motion
#include "FacialMotion.h"
#include "FacialMotionPart.h"
#include "ExpressionPartController.h"
#include "ExpressionPart.h"
#include "MeshExpressionPart.h"
#include "FacialSetup.h"

// collision detection systems
#include "NodeCollisionSystem.h"
#include "ActorCollisionSystem.h"
#include "SimpleNodeCollisionSystem.h"
#include "SimpleActorCollisionSystem.h"

// controllers
#include "Controller.h"
#include "PhysicsController.h"

// constraints
#include "Constraint.h"
#include "LookAtConstraint.h"
#include "PositionConstraint.h"
#include "RotationConstraint.h"
#include "ScaleConstraint.h"
#include "TransformConstraint.h"

// normal map support
#include "NormalMap.h"
#include "ImageResChecker.h"
#include "NormalMapGenerator.h"
#include "BasicNormalMapGenerator.h"

// lma importer
#include "Importer/Importer.h"
#include "Importer/ChunkProcessors.h"
#include "Importer/FileFormatStructs.inl"

// inverse kinematics
#include "TwoLinkIKSolver.h"
#include "CCDIKSolver.h"

// jacobian pseudo inverse kinematics
#include "JacobianIKConstraint.h"
#include "JacobianIKActorDOF.h"
#include "JacobianIKData.h"
#include "JacobianIKSolver.h"
#include "JacobianIKController.h"
#include "JacobianIKFixedPoint.h"
#include "JacobianIKRelativePoint.h"

// character factory
#include "CharacterFactory.h"

// misc
#include "MemoryCategories.h"


#endif	// __EMOTIONFX_H