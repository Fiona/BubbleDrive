/***************************************
 ********    BUBBLE DRIVE     **********
 **       2D space action RPG         **
 ***************************************
 ****** By Stompy Blondie Games  *******
 ***************************************
 *** File started May 2012 by Fiona ****
 ***************************************/

/**
 * Rectangle bin object source file.
 */


// Includes
#include "RectangleBin.h"


/**
 * Constructor
 * Sets up a new bin of a given initial size. These bin dimensions stay fixed during
 * the whole packing process, i.e. to change the bin size, the packing must be
 * restarted again.
 */
RectangleBin::RectangleBin(int width, int height)
{

	iBin_Width = width;
	iBin_Height = height;
	root.pLeft = root.pRight = std::shared_ptr<Node>();
	root.iX = root.iY = 0;
	root.iWidth = width;
	root.iHeight = height;

}


/*
 * @return A value [0, 1] denoting the ratio of total surface area that is in use.
 *  0.0f - the bin is totally empty, 1.0f - the bin is full.
 */
float RectangleBin::Occupancy() const
{

	unsigned long iTotal_Surface_Area = iBin_Width * iBin_Height;
	unsigned long iUsed_Surface_Area = Used_Surface_Area(root);

	return (float)iUsed_Surface_Area / iTotal_Surface_Area;

}


/*
 * Recursively calls itself.
 */
unsigned long RectangleBin::Used_Surface_Area(const Node &node) const
{

	if(node.pLeft || node.pRight)
	{

		unsigned long iUsed_Surface_Area = node.iWidth * node.iHeight;
		if(node.pLeft)
			iUsed_Surface_Area += Used_Surface_Area(*node.pLeft);
		if(node.pRight)
			iUsed_Surface_Area += Used_Surface_Area(*node.pRight);

		return iUsed_Surface_Area;

	}

	// This is a leaf node, it doesn't constitute to the total surface area.
	return 0;

}



/*
 * Running time is linear to the number of rectangles already packed. Recursively calls itself.
 * @return 0 If the insertion didn't succeed.
 */
RectangleBin::Node *RectangleBin::Insert(RectangleBin::Node *node, int width, int height)
{

	// If this node is an internal node, try both leaves for possible space.
	// (The rectangle in an internal node stores used space, the leaves store free space)
	if(node->pLeft || node->pRight)
	{

		if(node->pLeft)
		{

			Node *new_node = Insert(node->pLeft.get(), width, height);
			if(new_node)
				return new_node;

		}

		if(node->pRight)
		{

			Node *new_node = Insert(node->pRight.get(), width, height);
			if(new_node)
				return new_node;

		}

		return 0; // Didn't fit into either subtree!

	}

	// This node is a leaf, but can we fit the new rectangle here?
	if(width > node->iWidth || height > node->iHeight)
		return 0; // Too bad, no space.

	// The new cell will fit, split the remaining space along the shorter axis,
	// that is probably more optimal.
	int w = node->iWidth - width;
	int h = node->iHeight - height;
	node->pLeft.reset(new Node);
    node->pRight.reset(new Node);

	// Split the remaining space in horizontal direction.
	if(w <= h)
	{

		node->pLeft->iX = node->iX + width;
		node->pLeft->iY = node->iY;
		node->pLeft->iWidth = w;
		node->pLeft->iHeight = height;

		node->pRight->iX = node->iX;
		node->pRight->iY = node->iY + height;
		node->pRight->iWidth = node->iWidth;
		node->pRight->iHeight = h;

	}
	else // Split the remaining space in vertical direction.
	{

		node->pLeft->iX = node->iX;
		node->pLeft->iY = node->iY + height;
		node->pLeft->iWidth = width;
		node->pLeft->iHeight = h;

		node->pRight->iX = node->iX + width;
		node->pRight->iY = node->iY;
		node->pRight->iWidth = w;
		node->pRight->iHeight = node->iHeight;

	}

	// Note that as a result of the above, it can happen that node->left or node->right
	// is now a degenerate (zero area) rectangle. No need to do anything about it,
	// like remove the nodes as "unnecessary" since they need to exist as children of
	// this node (this node can't be a leaf anymore).

	// This node is now a non-leaf, so shrink its area - it now denotes
	// *occupied* space instead of free space. Its children spawn the resulting
	// area of free space.
	node->iWidth = width;
	node->iHeight = height;

	return node;

}
