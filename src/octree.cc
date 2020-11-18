
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Simple Octree Implementation 11/10/2020
//
//  Copyright (c) by Kevin M. Smith
//  Copying or use without permission is prohibited by law.
//

#include "octree.h"

// draw a box from a "Box" class
//
void Octree::drawBox(const Box &box) {
  glm::vec3 min = box.corners_[0];
  glm::vec3 max = box.corners_[1];
  glm::vec3 size = max - min;
  glm::vec3 center = size / 2 + min;
  ofVec3f p = ofVec3f(center.x, center.y, center.z);
  float w = size.x;
  float h = size.y;
  float d = size.z;
  ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box Octree::meshBounds(const ofMesh &mesh) {
  int n = mesh.getNumVertices();
  ofVec3f v = mesh.getVertex(0);
  ofVec3f max = v;
  ofVec3f min = v;
  for (int i = 1; i < n; i++) {
    ofVec3f v = mesh.getVertex(i);

    if (v.x > max.x)
      max.x = v.x;
    else if (v.x < min.x)
      min.x = v.x;

    if (v.y > max.y)
      max.y = v.y;
    else if (v.y < min.y)
      min.y = v.y;

    if (v.z > max.z)
      max.z = v.z;
    else if (v.z < min.z)
      min.z = v.z;
  }
  cout << "vertices: " << n << endl;
  //	cout << "min: " << min << "max: " << max << endl;
  return Box(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are
// contained
//                      inside the Box.  Return count of points found;
//
int Octree::getMeshPointsInBox(const ofMesh &mesh, const vector<int> &points,
                               Box &box, vector<int> &pointsRtn) {
  int count = 0;
  for (int i = 0; i < points.size(); i++) {
    ofVec3f v = mesh.getVertex(points[i]);
    if (box.inside(glm::vec3(v.x, v.y, v.z))) {
      count++;
      pointsRtn.push_back(points[i]);
    }
  }
  return count;
}

// getMeshFacesInBox:  return an array of indices to Faces in mesh that are
// contained
//                      inside the Box.  Return count of faces found;
//
int Octree::getMeshFacesInBox(const ofMesh &mesh, const vector<int> &faces,
                              Box &box, vector<int> &facesRtn) {
  int count = 0;
  for (int i = 0; i < faces.size(); i++) {
    ofMeshFace face = mesh.getFace(faces[i]);
    ofVec3f v[3];
    v[0] = face.getVertex(0);
    v[1] = face.getVertex(1);
    v[2] = face.getVertex(2);
    auto p = vector<glm::vec3>{v[0], v[1], v[2]};
    if (box.inside(p)) {
      count++;
      facesRtn.push_back(faces[i]);
    }
  }
  return count;
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//
void Octree::subDivideBox8(const Box &box, vector<Box> &boxList) {
  glm::vec3 min = box.corners_[0];
  glm::vec3 max = box.corners_[1];
  glm::vec3 size = max - min;
  glm::vec3 center = size / 2 + min;
  float xdist = (max.x - min.x) / 2;
  float ydist = (max.y - min.y) / 2;
  float zdist = (max.z - min.z) / 2;
  glm::vec3 h = glm::vec3(0, ydist, 0);

  //  generate ground floor
  //
  Box b[8];
  b[0] = Box(min, center);
  b[1] = Box(b[0].min() + glm::vec3(xdist, 0, 0),
             b[0].max() + glm::vec3(xdist, 0, 0));
  b[2] = Box(b[1].min() + glm::vec3(0, 0, zdist),
             b[1].max() + glm::vec3(0, 0, zdist));
  b[3] = Box(b[2].min() + glm::vec3(-xdist, 0, 0),
             b[2].max() + glm::vec3(-xdist, 0, 0));

  boxList.clear();
  for (int i = 0; i < 4; i++) boxList.push_back(b[i]);

  // generate second story
  //
  for (int i = 4; i < 8; i++) {
    b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
    boxList.push_back(b[i]);
  }
}

void Octree::create(const ofMesh &geo, int numLevels) {
  // initialize octree structure
  //
  mesh = geo;
  int level = 0;
  root.box = meshBounds(mesh);
  if (!bUseFaces) {
    for (int i = 0; i < mesh.getNumVertices(); i++) {
      root.points.push_back(i);
    }
  } else {
    // need to load face vertices here
    //
  }

  // recursively buid octree
  //
  level++;
  subdivide(mesh, root, numLevels, level);
}

void Octree::subdivide(const ofMesh &mesh, TreeNode &node, int numLevels,
                       int level) {
  if (level >= numLevels) return;
  vector<Box> boxList;
  subDivideBox8(node.box, boxList);
  level++;
  int pointsInNode = node.points.size();
  int totalPoints = 0;
  for (int i = 0; i < boxList.size(); i++) {
    TreeNode child;
    int count = 0;
    if (!bUseFaces)
      count = getMeshPointsInBox(mesh, node.points, boxList[i], child.points);
    else
      count = getMeshFacesInBox(mesh, node.points, boxList[i], child.points);
    totalPoints += count;

    if (count > 0) {
      child.box = boxList[i];
      node.children.push_back(child);
      if (count > 1) {
        subdivide(mesh, node.children.back(), numLevels, level);
      }
    }
  }
  // debug
  //
  if (pointsInNode != totalPoints) {
    strayVerts += (pointsInNode - totalPoints);
  }
}

// Implement functions below for Homework project
//

bool Octree::intersect(const Ray &ray, const TreeNode &node,
                       TreeNode &nodeRtn) {
  bool intersects = false;
  return intersects;
}

bool Octree::intersect(const Box &box, TreeNode &node,
                       vector<Box> &boxListRtn) {
  bool intersects = false;
  return intersects;
}

void Octree::draw(TreeNode &node, int numLevels, int level) {}

// Optional
//
void Octree::drawLeafNodes(TreeNode &node) {}
