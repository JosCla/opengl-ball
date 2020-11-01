// octree.cpp

// stdlib
#include <iostream> // for cout
#include <vector> // for vector
using namespace std;

// our files
#include "octree.h" // for class declaration
#include "shapes.h" // for shape classes

// Octree constructor
Octree::Octree(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) : subdivided(false) {
	xMin = xmin;
	xMax = xmax;
	yMin = ymin;
	yMax = ymax;
	zMin = zmin;
	zMax = zmax;
}

// Octree constructor
Octree::Octree(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, vector<Triangle> tris) : subdivided(false) {
	xMin = xmin;
	xMax = xmax;
	yMin = ymin;
	yMax = ymax;
	zMin = zmin;
	zMax = zmax;

	Insert(tris);
}

// Inserts a single triangle into the octree
// Returns whether or not the triangle could be inserted
bool Octree::Insert(Triangle tri) {
	octreeTris.push_back(tri);
	// general plan:
	// firstly, check if the triangle even fits in the octree bounding box
	//   if not then just return false to indicate failed insertion
	//   but if you can feasibly Insert it, remember to return true at the end!
	// then, if there are no sub-octrees:
	//   if there are no triangles in the octreeTris array, just push the triangle into that array
	//   else, subdivide into sub-octrees, try to push each triangle into those
	// else, if there are sub-octrees:
	//   just try to Insert into each sub-octree until one returns true
	//   and if that doesn't work just push to the octreeTris array
	return true;
}

// Inserts a vector of triangles into the octree
// Returns whether or not all triangles could be inserted
bool Octree::Insert(vector<Triangle> tris) {
	bool allInserted = true;
	for (unsigned int i = 0; i < tris.size(); ++i) {
		allInserted = allInserted && Insert(tris.at(i));
	}
	return allInserted;
}

