#ifndef OCTREE_H
#define OCTREE_H
// octree.h
// Defines the octree class, used to create an octree for efficient collision handling

// stdlib
#include <vector> // for vector

// our files
#include "shapes.h" // for shape classes

class Octree {
	public:
		Octree(float, float, float, float, float, float);
		Octree(float, float, float, float, float, float, std::vector<Triangle>);

		bool Insert(Triangle);
		bool Insert(std::vector<Triangle>);

	private:
		float xMin, xMax;
		float yMin, yMax;
		float zMin, zMax;

		std::vector<Octree> subOctrees;
		bool subdivided;

		std::vector<Triangle> octreeTris;
};

#endif
