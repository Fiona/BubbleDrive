/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

#pragma once
#ifndef _RECTANGLEBIN_H_
#define _RECTANGLEBIN_H_

// Includes
#include <memory>
//#include <boost/smart_ptr.hpp>

/**
 * Rectangle bins are an abstract way to keep atlas like structures.
 * Performs 'discrete online rectangle packing into a rectangular bin' by maintaining 
 * a binary tree of used and free rectangles of the bin. 
 * Code based on http://clb.demon.fi/projects/rectangle-bin-packing 
 * License is public domain.
 */
class RectangleBin
{
public:

	/*
	 * Constructor, starts a new packing process to a bin of the given dimension.
	 */
	RectangleBin(int width, int height);

	/*
	 * A node of a binary tree. Each node represents a rectangular area of the bin
	 * we surface. Internal nodes store rectangles of used data, whereas leaf nodes track 
	 * rectangles of free space. All the rectangles stored in the tree are disjoint.
	 */
	struct Node
	{
		// Left and right child. We don't really distinguish which is which, so these could
		// as well be child1 and child2.
		std::shared_ptr<Node> pLeft;
		std::shared_ptr<Node> pRight;

		// The top-left coordinate of the rectangle.
		int iX;
		int iY;

		// The dimension of the rectangle.
		int iWidth;
		int iHeight;
	};

	/*
	 * Inserts a new rectangle of the given size into the bin.
	 *
	 * Running time is linear to the number of rectangles that have been already packed.
	 * @return A pointer to the node that stores the newly added rectangle, or 0 
	 *   if it didn't fit.
	 */
	Node *Insert(int width, int height)
	{
		return Insert(&root, width, height);
	}

	/*
	 * Computes the ratio of used surface area.
	 */
	float Occupancy() const;


private:
	Node root;

	// The total size of the bin we started with.
	int iBin_Width;
	int iBin_Height;

	/*
	 * @return The surface area used by the subtree rooted at node.
	 */
	unsigned long Used_Surface_Area(const Node &node) const;

	/*
	 * Inserts a new rectangle in the subtree rooted at the given node.
	 */
	Node *Insert(Node *node, int width, int height);

};


#endif
