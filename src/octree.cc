#include "octree.h"

/**
 * @brief Creates an Octree
 * @param mesh The desired mesh to spatially partition
 * @param num_levels The total number of Octree level divisions
 */
Octree::Octree(const ofMesh& mesh, const int num_levels) {
  mesh_ = mesh;
  root_.box_ = Box::CreateMeshBoundingBox(mesh);
  auto level{0};

  if (use_mesh_faces_) {
    // TODO load face vertices (?)
  } else {
    for (auto i = 0; i < mesh.getNumVertices(); i++) {
      root_.points_.push_back(i);
    }
  }

  level++;
  Subdivide(mesh, root_, num_levels, level);
}

/**
 * @brief Subdivides the Octree recursively
 * @param mesh The desired mesh to spatially partition
 * @param node The current "parent" node being divided
 * @param num_levels The total number of Octree level divisions
 * @param current_level The current Octree level division
 */
void Octree::Subdivide(const ofMesh& mesh, TreeNode& node, const int num_levels,
                       int current_level) {
  if (current_level >= num_levels) return;

  current_level++;
  auto total_points{0};
  auto sub_boxes = SubdivideBox8(node.box_);

  for (auto& box : sub_boxes) {
    TreeNode child;

    if (use_mesh_faces_) {
      child.points_ = GetMeshFacesInBox(mesh, node.points_, box);
    } else {
      child.points_ = GetMeshPointsInBox(mesh, node.points_, box);
    }

    const auto count{child.points_.size()};
    total_points += count;

    if (count > 0) {
      child.box_ = box;
      node.children_nodes_.push_back(child);

      if (count > 1) {
        Subdivide(mesh, node.children_nodes_.back(), num_levels, current_level);
      }
    }
  }

  // debug
  const auto points_in_node{node.points_.size()};
  if (points_in_node != total_points) {
    debug_stray_vertices_ += (points_in_node - total_points);
    if (debug_stray_vertices_ > 0) {
      cerr << "number of stray vertices: " << debug_stray_vertices_ << endl;
    }
  }
}

/**
 * @brief Subdivides a Box into 8 sub-boxes
 * @param box The Box to divide
 * @return The Box's 8 sub-boxes
 */
vector<Box> Octree::SubdivideBox8(const Box& box) {
  const auto min = box.min();
  const auto max = box.max();
  const auto size = max - min;
  const auto center = size / 2 + min;
  const auto half_x_width = (max.x - min.x) / 2;
  const auto half_y_width = (max.y - min.y) / 2;
  const auto half_z_width = (max.z - min.z) / 2;
  const auto height = glm::vec3(0, half_y_width, 0);
  vector<Box> sub_boxes;

  // generate "first floor"
  Box b[8];
  b[0] = Box(min, center);
  b[1] = Box(b[0].min() + glm::vec3(half_x_width, 0, 0),
             b[0].max() + glm::vec3(half_x_width, 0, 0));
  b[2] = Box(b[1].min() + glm::vec3(0, 0, half_z_width),
             b[1].max() + glm::vec3(0, 0, half_z_width));
  b[3] = Box(b[2].min() + glm::vec3(-half_x_width, 0, 0),
             b[2].max() + glm::vec3(-half_x_width, 0, 0));

  for (auto i = 0; i < 4; i++) {
    sub_boxes.push_back(b[i]);
  }

  // generate "second floor"
  for (auto i = 4; i < 8; i++) {
    b[i] = Box(b[i - 4].min() + height, b[i - 4].max() + height);
    sub_boxes.push_back(b[i]);
  }

  return sub_boxes;
}

/**
 * @brief Gets the indices of faces in a mesh that are contained within a box
 * @param mesh The total mesh to query
 * @param faces The mesh's faces
 * @param box The Box to define the query boundary
 * @return The indices of faces in the mesh contained within the Box
 */
vector<int> Octree::GetMeshFacesInBox(const ofMesh& mesh,
                                      const vector<int>& faces,
                                      const Box& box) {
  vector<int> indices;

  for (const auto& face : faces) {
    auto mesh_face = mesh.getFace(face);
    auto points = vector<glm::vec3>{
        mesh_face.getVertex(0), mesh_face.getVertex(1), mesh_face.getVertex(2)};

    if (box.Inside(points)) {
      indices.push_back(face);
    }
  }

  return indices;
}

/**
 * @brief Gets the indices of points in a mesh contained within a Box
 * @param mesh The total mesh to query
 * @param points The mesh's points
 * @param box The Box to define the query boundary
 * @return The indices of points in the mesh contained within the Box
 */
vector<int> Octree::GetMeshPointsInBox(const ofMesh& mesh,
                                       const vector<int>& points,
                                       const Box& box) {
  vector<int> indices;

  for (const auto& point : points) {
    auto vertex = mesh.getVertex(point);

    if (box.Inside(vertex)) {
      indices.push_back(point);
    }
  }

  return indices;
}

/**
 * @brief Draws this Octree
 * @param num_levels The total number of Octree level divisions
 * @param current_level The current Octree level division
 */
void Octree::Draw(const int num_levels, const int current_level) const {
  Draw(root_, num_levels, current_level);
}

/**
 * @brief Draws only this Octree's leaf nodes
 * @param node TODO
 */
void Octree::DrawLeafNodes(const TreeNode& node) {
  // TODO optional
}

/**
 * @brief TODO
 * @param box TODO
 * @param node TODO
 * @param terrain_collision_boxes TODO
 * @return TODO
 */
bool Octree::Intersect(const Box& box, const TreeNode& node,
                       vector<Box>& terrain_collision_boxes) {
  // TODO

  return false;
}

/**
 * @brief Determines which leaf node in this Octree is intersected by a given
 * ray
 * @param ray The ray potentially intersecting this Octree
 * @param node The current node being checked for intersection, which may or may
 * not have children nodes
 * @param node_rtn (SIDE EFFECT RETURN VALUE) The final, intersected leaf node
 * @return True if the ray intersects the Octree, false otherwise
 */
bool Octree::Intersect(const Ray& ray, const TreeNode& node,
                       TreeNode& node_rtn) {
  // FIXME not all control paths return a value

  if (node.box_.Intersect(ray, -1000, 1000)) {
    if (node.children_nodes_.empty()) {
      node_rtn = node;
      return true;
    }

    for (const auto& child : node.children_nodes_) {
      Intersect(ray, child, node_rtn);
    }

  } else {
    return false;
  }
}

//-Private Methods----------------------------------------------
void Octree::Draw(const TreeNode& node, const int num_levels,
                  int current_level) const {
  if (current_level >= num_levels) return;

  current_level++;
  node.box_.Draw();

  for (const auto& child : node.children_nodes_) {
    Draw(child, num_levels, current_level);
  }
}
