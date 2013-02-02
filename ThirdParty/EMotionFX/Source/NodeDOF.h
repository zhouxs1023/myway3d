/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODEDOF_H__
#define __NODEDOF_H__

// include the Core system
#include "../Core/Source/MCore.h"

#include "NodeAttribute.h"


namespace EMotionFX
{

/**
 * The NodeDOF class.
 * It specifies which rotations can be applied to a node 
 * and the order in which it must be done.
 */
class NodeDOF : public NodeAttribute
{
	public:
		// the type ID, returned by GetType()
		enum { TYPE_ID = 0xCADA0001 };

		// the axis indices
		enum CanonicalAxis
		{
			AxisX		= 0,
			AxisY		= 1,
			AxisZ		= 2,
			AxisNONE	= -1
		};

		/**
		 * Constructor setting rotation.
		 * @param rotOrderAxis1 The first rotation axis.
		 * @param rotOrderAxis2 The second rotation axis.
		 * @param rotOrderAxis3 The third rotation axis.
		 */
		NodeDOF(const CanonicalAxis rotOrderAxis1, const CanonicalAxis rotOrderAxis2 = AxisNONE, const CanonicalAxis rotOrderAxis3 = AxisNONE);

		/**
		 * Default constructor. It returs a NodeDOF based on Euler's.
		 */
		NodeDOF();

		/**
		 * Destructor.
		 */
		~NodeDOF();

		/**
		 * Set the rotation order and the axis.
		 * @param rotOrderAxis1 The first rotation axis.
		 * @param rotOrderAxis2 The second rotation axis.
		 * @param rotOrderAxis3 The third rotation axis.
		 */
		void SetRotationOrder(const CanonicalAxis rotOrderAxis1, const CanonicalAxis rotOrderAxis2, const CanonicalAxis rotOrderAxis3);

		/**
		 * Set the NodeDOF based on Euler's.
		 */
		inline void SetEulerRotation();

		/**
		 * Set the limits for the i-th DOF.
		 */
		void SetLimits(const int index, const float max, const float min);

		/**
		 * Returns the max value for the node DOF
		 */
		inline float GetMaxValue(const int index) const;

		/**
		 * Returns the min value for the node DOF
		 */
		inline float GetMinValue(const int index) const;

		/**
		 * Get the attribute type.
		 * @return The attribute ID.
		 */
		int GetType() const;

		/**
		 * Get the attribute type as a string.
		 * This string should contain the name of the class.
		 * @return The string containing the type name.
		 */
		const char* GetTypeString() const;

		/**
		 * Clone the node attribute.
		 * @return Returns a pointer to a newly created exact copy of the node attribute.
		 */
		NodeAttribute* Clone();

		/**
		 * Returns the number of DOFs (rotations) applied to de LM_Node.
		 * @return The number of DOFs.
		 */
		inline int GetNumDOF() const;

		/**
		 * Set the 'id' for the node. It's reffered to the order in actor`s array.
		 */
		inline void SetNodeID(const int id);

		/**
		 * Returns the node 'id'
		 */ 
		inline int GetNodeID() const;

		/**
		 * Returns the i-th angle value. It must be previously calculated.
		 * @return The amgle value.
		 */
		inline float GetAngleValue(const int index) const;

		/**
		 * Returns the i-th DOF axis. It must be previously calculated.
		 * @return The DOF axis vector.
		 */
		inline MCore::Vector3 GetAxis(const int index) const;

		/**
		 * Returns the rotation angles values from actor's actual pose.
		 * @return A float array containing the values.
		 */
		void CalcValues(MCore::Matrix& localM, const MCore::Vector3& localS);

		/**
		 * Sets the angles values to those given by 'v1', 'v2' and 'v3'.
		 */
		inline void SetValues(const float v1, const float v2, const float v3);

		/**
		 * Calculates DOFs rotation axis from parent world matrix
		 * and knowing angles values.
		 */
		void CalcAxis(MCore::Matrix& parentM);

		/**
		 * Returns the rotation matrix using those angles values.
		 */
		MCore::Matrix GetMatrix(const float v1, const float v2, const float v3);

	private:
		int				mNumDOF;				/**< Number of DOFs for the node. */
		int				mNodeID;				/**< . */
		CanonicalAxis	mRotationOrder[3];		/**< Axis for each rotation and order in which those are applied. */
		bool			mEuler;					/**< This boolean is true if the DOFs are base on Euler rotation angles. */
		float			mMaxValue[3];			/**< Maximun value for each possible DOF. */
		float			mMinValue[3];			/**< Minimun value for each possible DOF. */
		float			mAngles[3];				/**< Internal use.. */
		MCore::Vector3	mAxis[3];				/**< Internal use.. */

		/**
		 * Returns true if these are the rotation settings.
		 * @param r1 1st rotation.
		 * @param r2 2nd rotation.
		 * @param r3 3th rotation.
		 */
		inline bool Rotation(const int r1, int r2, int r3) const;
		inline bool Rotation(const int r1, int r2) const;

		MCore::Matrix RotationMatrix(const int i, const float v) const;
};

// include inline code
#include "NodeDOF.inl"

} // namespace EMotionFX


#endif