/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __CCDIKSOLVER_H_
#define __CCDIKSOLVER_H_

// include the Core system
#include "../Core/Source/MCore.h"

// include LM headers
#include "Controller.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Actor;
class Node;


/**
 * The CCD Inverse Kinematics solver.
 * CCD stands for Cyclic Coordinate Descent which is an iterative method for solving the Inverse Kinematics problem.
 * The nice thing about this algorithm is that you can adjust speed versus accuracy.
 * It can work on chains of an unlimited number of nodes, just like the Jacobian solver.
 * The bad thing about the CCD algorithm is that it isn't smooth. When moving the goal, the motions of the nodes
 * are not always very smooth. Since this is an iterative method, the algorithm can suddenly come up with a completely
 * different solution as the frame before. Still, this is a very usable solver when there is a need for a pretty fast
 * IK solution on chains.
 */
class CCDIKSolver : public Controller
{
	MEMORYOBJECTCATEGORY(CCDIKSolver, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_IK);

	public:
		// the unique ID of this controller
		enum { TYPE_ID = 0x12230601 };

		/**
		 * The constructor.
		 * @param actor The actor to apply the inverse kinematics on.
		 * @param startNode The start node, for example the upper arm node.
		 * @param endNode The end effector node, for example the hand node.
		 * @param cloneNodes This tells the controller whether or not to make an internal copy of the nodes it affects. 
		 *                   This can sometimes help to give better results. However it sometimes also produces less good results, so is disabled on default.
		 */
		CCDIKSolver(Actor* actor, Node* startNode, Node* endNode, bool cloneNodes=false);

		/**
		 * The destructor.
		 */
		~CCDIKSolver();

		/**
		 * Get the unique type ID of this controller.
		 * @result The unique ID.
		 */
		int GetType() const;

		/**
		 * Get the type string, containing the class name of this controller.
		 * @result A pointer to the string containing the class name.
		 */
		const char* GetTypeString() const;

		/**
		 * Set the goal position, in world space.
		 * This is the point the IK solver will try to reach.
		 * @param goal The goal position in world space.
		 */
		void SetGoal(const MCore::Vector3& goal);

		/**
		 * Get the goal position, in world space.
		 * @result The goal position, in world space coordinates.
		 */
		const MCore::Vector3& GetGoal() const;

		/**
		 * Check if we have found a solution or not.
		 * @result Returns true when the last called Update has found a solution. Otherwise false is returned.
		 */
		bool HasFoundSolution() const;

		/**
		 * Set the maximum number of iterations.
		 * On default this is set to 10, which is quite high.
		 * Reducing this number will result in more unstable solutions that can change rapidly.
		 * Lower numbers however can result in much faster processing of this controller, especially in cases
		 * where no solutions can be found (worst case scenario of this controller).
		 * @param numIterations The maximum number of iterations of the CCD algorithm.
		 */
		void SetMaxIterations(const int numIterations);

		/**
		 * Get the maximum number of iterations of the CCD algorithm.
		 * On default this is set to 10, which is quite high.
		 * Reducing this number will result in more unstable solutions that can change rapidly.
		 * Lower numbers however can result in much faster processing of this controller, especially in cases
		 * where no solutions can be found (worst case scenario of this controller).
		 * @result The maximum number of iterations of the CCD processing loop.
		 */
		int GetMaxIterations() const;

		/**
		 * Set the distance threshold.
		 * If the end node comes within the squared distance range of the specified goal
		 * the controller will stop its iterations and will consider the solution solved.
		 * @param distThreshold The squared distance of the maximum distance between the goal and the end node, before we consider the solution solved.
		 */
		void SetDistThreshold(const float distThreshold);

		/**
		 * Get the distance threshold.
		 * If the end node comes within the squared distance range of the specified goal
		 * the controller will stop it's iterations and will consider the solution solved.
		 * @result The squared distance of the maximum distance between goal and end node, before we consider the solution solved.
		 */
		float GetDistThreshold() const;

		/**
		 * Enable or disable joint limits. On default joint limits are enabled.
		 * @param doJointLimits Set to true when you like to enable to limit the rotations of the joints (nodes).
		 */
		void SetDoJointLimits(const bool doJointLimits);

		/**
		 * Check if the processing of joint limits is enabled or not.
		 * @result Returns true when joint limits are enabled, otherwise false is returned.
		 */
		bool GetDoJointLimits() const;

		/**
		 * Clone this controller.
		 * @param actor The actor to apply the cloned version on.
		 * @result A pointer to the cloned controller.
		 */
		Controller* Clone(Actor* actor);

		/**
		 * The main method that performs the actual algorithm.
		 * @param timeDeltaInSeconds The time passed in seconds since the last update.
		 */
		void Update(const float timeDeltaInSeconds);


	private:
		MCore::Vector3	mGoal;				/**< The goal position, in world space. */
		Node*			mStartNode;			/**< The start node, for example the upper arm. */
		Node*			mEndNode;			/**< The end node, for example the hand (the end effector). */
		Node*			mStartNode_Clone;	/**< The start node, for example the upper arm. */
		Node*			mEndNode_Clone;		/**< The end node, for example the hand (the end effector). */
		float			mDistThreshold;		/**< The maximum squared distance between goal and end effector that makes the solution solved. */
		int				mMaxIterations;		/**< The maximum number of iterations (tradeoff between speed and accuracy) [default value=10]. */
		bool			mHasSolution;		/**< Did we find a solution? */
		bool			mDoJointLimits;		/**< True if you want to impose joint limits, false otherwise. [default=true]. */
};

} // namespace EMotionFX

#endif