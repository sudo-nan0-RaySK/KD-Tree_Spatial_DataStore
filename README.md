# Spatial Database using K-D Trees

## What are K-D Trees?

A K-D Tree(also called as K-Dimensional Tree) is a binary search tree where data in each node is a K-Dimensional point in space. In short, it is a space partitioning(details below) data structure for organizing points in a K-Dimensional space.

A non-leaf node in K-D tree divides the space into two parts, called as half-spaces.

Points to the left of this space are represented by the left subtree of that node and points to the right of the space are represented by the right subtree. We will soon be explaining the concept on how the space is divided and tree is formed.

## Creation of a 2-D Tree

Consider following points in a 2-D plane:
(3, 6), (17, 15), (13, 15), (6, 12), (9, 1), (2, 7), (10, 19)

- Insert (3, 6): Since tree is empty, make it the root node.
- Insert (17, 15): Compare it with root node point. Since root node is X-aligned, the X-coordinate value will be compared to determine if it lies in the rightsubtree or in the right subtree. This point will be Y-aligned.
- Insert (13, 15): X-value of this point is greater than X-value of point in root node. So, this will lie in the right subtree of (3, 6). 
- Again Compare Y-value of this point with the Y-value of point (17, 15) (Why?). Since, they are equal, this point will lie in the right subtree of (17, 15). This point will be X-aligned.
- Insert (6, 12): X-value of this point is greater than X-value of point in root node. So, this will lie in the right subtree of (3, 6). Again Compare Y-value of this point with the Y-value of point (17, 15) (Why?). Since, 12 < 15, this point will lie in the left subtree of (17, 15). This point will be X-aligned.
- Insert (9, 1):Similarly, this point will lie in the right of (6, 12).
- Insert (2, 7):Similarly, this point will lie in the left of (3, 6).
Insert (10, 19): Similarly, this point will lie in the left of (13, 15).


## Dividing Subspaces

### Nearest neighbour search

Example of a nearest neighbor search in a 2-d tree. Here, the tree is already built, each node corresponds to a rectangle, each rectangle is split in two equal subrectangles, and leaves correspond to rectangles containing a single point
The nearest neighbour search (NN) algorithm aims to find the point in the tree that is nearest to a given input point. This search can be done efficiently by using the tree properties to quickly eliminate large portions of the search space.

Searching for a nearest neighbour in a k-d tree proceeds as follows:

Starting with the root node, the algorithm moves down the tree recursively, in the same way that it would if the search point were being inserted (i.e. it goes left or right depending on whether the point is lesser than or greater than the current node in the split dimension).
Once the algorithm reaches a node, it checks that node point and if the distance is better, that node point is saved as the "current best".
The algorithm unwinds the recursion of the tree, performing the following steps at each node:
If the current node is closer than the current best, then it becomes the current best.
The algorithm checks whether there could be any points on the other side of the splitting plane that are closer to the search point than the current best. In concept, this is done by intersecting the splitting hyperplane with a hypersphere around the search point that has a radius equal to the current nearest distance. Since the hyperplanes are all axis-aligned this is implemented as a simple comparison to see whether the distance between the splitting coordinate of the search point and current node is lesser than the distance (overall coordinates) from the search point to the current best.
If the hypersphere crosses the plane, there could be nearer points on the other side of the plane, so the algorithm must move down the other branch of the tree from the current node looking for closer points, following the same recursive process as the entire search.
If the hypersphere doesn't intersect the splitting plane, then the algorithm continues walking up the tree, and the entire branch on the other side of that node is eliminated.
When the algorithm finishes this process for the root node, then the search is complete.
Generally the algorithm uses squared distances for comparison to avoid computing square roots. Additionally, it can save computation by holding the squared current best distance in a variable for comparison.

The algorithm can be extended in several ways by simple modifications. It can provide the k nearest neighbours to a point by maintaining k current bests instead of just one. A branch is only eliminated when k points have been found and the branch cannot have points closer than any of the k current bests.

It can also be converted to an approximation algorithm to run faster. For example, approximate nearest neighbour searching can be achieved by simply setting an upper bound on the number points to examine in the tree, or by interrupting the search process based upon a real time clock (which may be more appropriate in hardware implementations). Nearest neighbour for points that are in the tree already can be achieved by not updating the refinement for nodes that give zero distance as the result, this has the downside of discarding points that are not unique, but are co-located with the original search point.

Approximate nearest neighbour is useful in real-time applications such as robotics due to the significant speed increase gained by not searching for the best point exhaustively. One of its implementations is best-bin-first search.

## Range search

A range search searches for ranges of parameters. For example, if a tree is storing values corresponding to income and age, then a range search might be something like looking for all members of the tree which have an age between 20 and 50 years and an income between 50,000 and 80,000. Since k-d trees divide the range of a domain in half at each level of the tree, they are useful for performing range searches.

Analyses of binary search trees has found that the worst case time for range search in a k-dimensional KD tree containing n nodes is given by the following equation.

<a href="https://www.codecogs.com/eqnedit.php?latex={\displaystyle&space;t_{\text{worst}}=O(k\cdot&space;n^{1-{\frac&space;{1}{k}}})}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?{\displaystyle&space;t_{\text{worst}}=O(k\cdot&space;n^{1-{\frac&space;{1}{k}}})}" title="{\displaystyle t_{\text{worst}}=O(k\cdot n^{1-{\frac {1}{k}}})}" /></a>


## Use Case - A spatial database

The property of dividing a higher dimesional search space in each level makes K-D tree an excellent data structure to perform queries on spatial data. We can find shortest routes, neighbour search in radius etc. , quite easily.

In this project we use this data structure to do the same. We supply 2D latitude/longitude data of different places and then perform queries upon same.

Further there is also a performace logging to see how many comparisons it took for each operation