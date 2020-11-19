/*
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
  Octree(const ofMesh &mesh, int num_levels);
  void subdivide(const ofMesh &mesh, TreeNode &node, int num_levels,
                 int current_level);
  static vector<Box> SubdivideBox8(const Box &box);
  static vector<int> GetMeshFacesInBox(const ofMesh &mesh,
                                       const vector<int> &faces,
                                       const Box &box);
  static vector<int> GetMeshPointsInBox(const ofMesh &mesh,
                                        const vector<int> &points,
                                        const Box &box);
  void draw(int num_levels, int current_level) const;
  void drawLeafNodes(const TreeNode &node);
  bool intersect(const Box &, TreeNode &node, vector<Box> &box_list_rtn);
  bool intersect(const Ray &, const TreeNode &node, TreeNode &node_rtn);

  ofMesh mesh_;
  TreeNode root_;
  bool use_mesh_faces_{false};

  int debug_stray_vertices_{0};
  int number_of_leaves_{0};

 private:
  void draw(const TreeNode &node, int num_levels, int current_level) const;
};