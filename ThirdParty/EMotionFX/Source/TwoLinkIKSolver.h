/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __TWOLINKIKSOLVER_H
#define __TWOLINKIKSOLVER_H

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
 * The two link (bones) Inverse Kinematics (IK) solver.
 * This can be used to quickly calculate an IK solution for systems involving two bones, such as limbs.
 * The following ascii 'art' will show an example:
 *
 * <pre>
 *      B
 *     / \
 *    /   C 
 *   A   (goal) 
 *
 * where:
 * A = start node
 * B = mid node
 * C = end node (end effector)
 * (goal) = The goal we try to reach.
 * </pre>
 *
 * Given the worldspace positions of the nodes A, B and C we can calculate their lenghts.
 * The problem is now defined as: "What is the position of B so that C touches the goal, while we remain
 * the lenghts of the bones unchanged?".
 *
 * The implementation of this controller uses the solution described by Ken Perlin.
 * 
 * <pre>
 * <b>Description by Ken Perlin:</b>
 * Given a two link joint from [0,0,0] to end effector position P,
 * let link lengths be a and b, and let norm |P| = c.  Clearly a+b >= c.
 *
 * Problem: find a "knee" position Q such that |Q| = a and |P-Q| = b.
 *
 * In the case of a point on the x axis R = [c,0,0], there is a
 * closed form solution S = [d,e,0], where |S| = a and |R-S| = b:
 *
 *    d2+e2 = a2                  -- because |S| = a
 *    (c-d)2+e2 = b2              -- because |R-S| = b
 *
 *    c2-2cd+d2+e2 = b2           -- combine the two equations
 *    c2-2cd = b2 - a2
 *    c-2d = (b2-a2)/c
 *    d - c/2 = (a2-b2)/c / 2
 *
 *    d = (c + (a2-b2/c) / 2      -- to solve for d and e.
 *    e = Sqrt(a2-d2)
 *
 * This leads to a solution to the more general problem:
 *
 *   (1) R = Mfwd(P)         -- rotate P onto the x axis
 *   (2) Solve for S
 *   (3) Q = Minv(S)         -- rotate back again
 *
 * If "knee" position Q needs to be as close as possible to some point D,
 * then choose M such that M(D) is in the y>0 half of the z=0 plane.
 * </pre>
 *
 * After you have created the controller you also have to set the desired bend direction.
 * This bend direction is a unit vector that points into the direction (in worldspace) of where the 'elbow' or 'knee'
 * should point inside the solution. An example would be if you put your fists on top of eachother and keep your arms stretched
 * pointing forward, away from you. Now if you move your fists closer to your chest, you see that the elbow of your right arm
 * points to the right (and maybe a bit downwards) and your left elbow points to the left. These directions are the bend directions
 * that you specify to the controller. Please note that these are in worldspace. This means that if you rotate your character
 * you also have to rotate the direction vectors with the same rotation as you rotate the character.
 */
class TwoLinkIKSolver : public Controller
{
	MEMORYOBJECTCATEGORY(TwoLinkIKSolver, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_IK);

	public:
		// the unique type ID of this IK controller
		enum { TYPE_ID = 0x00123001 };

		/**
		 * The constructor.
		 * @param actor The actor to apply the inverse kinematics on.
		 * @param startNode The start node, for example the upper arm.
		 * @param midNode The middle node, for example the lower arm.
		 * @param endNode The end effector node, for example the hand.
		 */
		TwoLinkIKSolver(Actor* actor, Node* startNode, Node* midNode, Node* endNode);

		/**
		 * Update the controller. This is the main method that performs the IK and updates the nodes.
		 * @param timeDeltaInSeconds The time passed in seconds, since the last update call.
		 */
		void Update(const float timeDeltaInSeconds);

		/**
		 * Get the goal node, which represents the worldspace position of the goal.
		 * When this goal node is set to NULL, so when this method returns NULL, this means the solver
		 * will NOT use the goal node, but the goal position returned by GetGoal().
		 * If the goal node is set to a non-null pointer, the goal node's worldspace position will be used.
		 * This makes it possible to for example hold a gun with two arms, by making IK anchor nodes on the gun.
		 * @result A pointer to the node to use as goal, or NULL when the goal node shouldn't be used.
		 */
		Node* GetGoalNode() const;

		/**
		 * Set the goal node, from which the world space position will be used as goal.
		 * This is useful when you want to grab a gun with two arms using two IK controllers.
		 * The first controller would adjust the arm for aiming, which adjusts the gun as welll, since it
		 * is attached to for example the hand, which is influenced by IK. This means that you couldn't use
		 * a simple goal position, because this position is now changed by the first IK controller.
		 * By using a node's world space position instead, this problem will be solved.
		 * If you do not want to specify a goal node, but want to specify a 3D position (MCore::Vector3) instead, 
		 * you have to set the goal node to NULL, which is also the default after creating the controller.
		 * @param goal The goal node, or NULL when you want to use the SetGoal and GetGoal methods.
		 */
		void SetGoalNode(Node* goal);

		/**
		 * Set the goal position, in world space.
		 * This is the point the IK solver will try to reach.
		 * Please note that this goal position is only used when the goal node is set to NULL.
		 * You can do this with the SetGoalNode method.
		 * @param goal The goal position in world space.
		 */
		void SetGoal(const MCore::Vector3& goal);

		/**
		 * Get the goal position, in world space.
		 * @result The goal position, in world space coordinates.
		 */
		const MCore::Vector3& GetGoal() const;

		/**
		 * Get the bend direction in world space.
		 * This bend direction is a vector that points into the direction where the midNode joint
		 * should point towards in the solution. If the character is facing the camera which is at the origin and
		 * looking forward into depth, the bend direction for the knee of a human character would point towards
		 * the camera, so would be (0, 0, -1) in this case.
		 * Since the bend direction is in world space, you will have to rotate it with the characters worldspace rotation.
		 * @result The bend direction of the 'elbow' or 'knee'.
		 */
		const MCore::Vector3& GetBendDirection() const;

		/**
		 * Set the bend direction in world space.
		 * The bend direction must be a unit vector. So a vector of a length of 1 (normalized vector).
		 * This bend direction is a vector that points into the direction where the midNode joint
		 * should point towards in the solution. If the character is facing the camera which is at the origin and
		 * looking forward into depth, the bend direction for the knee of a human character would point towards
		 * the camera, so would be (0, 0, -1) in this case.
		 * Since the bend direction is in world space, you will have to rotate it with the characters worldspace rotation.
		 * @param bendDir The direction vector where the knee or elbow should point to. This vector must be normalized.
		 */
		void SetBendDirection(const MCore::Vector3& bendDir);

		/**
		 * Set the start node, for example the upper arm or upper leg.
		 * @param node A pointer to the start node.
		 */
		void SetStartNode(Node* node);

		/**
		 * Set the middle node. This basically represents the joint node.
		 * This could for example be the lower arm or lower leg node.
		 * @param node A pointer to the middle node.
		 */
		void SetMidNode(Node* node);

		/**
		 * Set the end effector node. When the position of this node is at the goal in worldspace
		 * a solution has been found. An example of this node could be the hand or the foot.
		 * @param node A pointer to the end effector node.
		 */
		void SetEndEffector(Node* node);

		/**
		 * Get the start node, for example the upper arm or upper leg.
		 * @result A pointer to the start node.
		 */
		Node* GetStartNode() const;

		/**
		 * Get the middle node. This basically represents the joint node.
		 * This could for example be the lower arm or lower leg node.
		 * @result A pointer to the middle node.
		 */
		Node* GetMidNode() const;

		/**
		 * Get the end effector node. When the position of this node is at the goal in worldspace
		 * a solution has been found. An example of this node could be the hand or the foot.
		 * @result A pointer to the end effector node.
		 */
		Node* GetEndEffector() const;

		/**
		 * Clone this controller.
		 * @param actor The actor to apply the cloned version on.
		 * @result A pointer to the cloned controller.
		 */
		Controller* Clone(Actor* actor);

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
		 * Check if we have found a solution or not.
		 * @result Returns true when the last called Update has found a solution. Otherwise false is returned.
		 */
		bool HasFoundSolution() const;

		/**
		 * Solve the two bone IK problem.
		 * This function works with positions only, and does not solve the rotations of nodes.
		 * Actually this function has nothing to do with nodes. It is however the core of the algorithm.
		 * Please keep in mind that all parameters should be in the same space, for example world space.
		 * The output of this method is a vector that contains the position of the middle joint.
		 * If you draw a line between the specified posA and this output middle joint pos and a line starting at
		 * the output middle pos in direction of the goal, with a length of (posC - posB).Length() you are drawing
		 * the solution of this solver.
		 * @param posA The position of the first joint (for example the upper arm / shoulder).
		 * @param posB The position of the middle joint (for example the elbow / lower arm).
		 * @param posC The position of the end effector (for example the hand / wrist).
		 * @param goal The goal the solver should try to reach.
		 * @param bendDir The desired middle joint bend direction (as unit vector). So the direction the knee or elbow should point to.
		 * @param outMidPos A pointer to the vector to which the function will write the new middle joint position.
		 * @result Returns true when a solution has been found, or false when the goal is not reachable.
		 */
		static bool Solve2LinkIK(const MCore::Vector3& posA, const MCore::Vector3& posB, const MCore::Vector3& posC, const MCore::Vector3& goal, const MCore::Vector3& bendDir, MCore::Vector3* outMidPos);


	private:
		MCore::Vector3	mGoal;			/**< The goal in worldspace. */
		MCore::Vector3	mBendDirection;	/**< The prefered bend direction, in worldspace. */
		Node*			mStartNode;		/**< The start node, for example the upper arm node). */
		Node*			mMidNode;		/**< The middle node, basically the joint that will bend. For example the lower arm node. */
		Node*			mEndNode;		/**< The end effector node. For example the hand node. */
		Node*			mGoalNode;		/**< The goal node, which is set to NULL on default. */
		bool			mHasSolution;	/**< Have we found a solution? */

		/**
		 * Calculates the matrix that rotates the object from solve space back into the original space.
		 * The inverse (which equals the transpose) of this matrix is used to convert the specified parameters into solve space.
		 * @param goal The goal we try to reach.
		 * @param bendDir The desired middle joint bend direction (as unit vector). So the direction the knee or elbow should point to.
		 * @param outForward The output matrix which will contain the matrix that rotates from solve space into the space of the goal.
		 */
		static void CalculateMatrix(const MCore::Vector3& goal, const MCore::Vector3& bendDir, MCore::Matrix* outForward);
};


} // namespace EMotionFX

#endif