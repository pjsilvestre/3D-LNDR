/**
 * @class Octree
 * @brief 3D spatial partitioning data structure
 * @author Kevin M. Smith (CS 134 SJSU)
 * @author Patrick Silvestre
 */

#pragma once

#include "box.h"
#include "ofMain.h"
#include "ray.h"

class TreeNode {
 public:
  Box box_;
  vector<int> points_;
  vector<TreeNode> children_nodes_;
};

class Octree {
 public:
  Octree() = default;
  Octree(const ofMesh& mesh, int num_levels);

  void Draw(int num_levels, int current_level) const;
  bool Intersect(const Box& box, const TreeNode& current_node,
                 vector<Box>& terrain_collision_boxes) const;
  bool Intersect(const Ray& ray, const TreeNode& current_node,
                 TreeNode& collision_node) const;

  ofMesh mesh_;
  TreeNode root_;

 private:
  void Draw(const TreeNode& node, int num_levels, int current_level) const;
  vector<int> GetMeshPointsInBox(const ofMesh& mesh, const vector<int>& points,
                                 const Box& box);
  void Subdivide(const ofMesh& mesh, TreeNode& node, int num_levels,
                 int current_level);
  vector<Box> SubdivideBox8(const Box& box);
};