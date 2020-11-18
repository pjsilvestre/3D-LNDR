#include "box.h"

#include "ray.h"
#include "vector3.h"

/*
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

bool Box::intersect(const Ray &r, float t0, float t1) const {
  float tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = (parameters[r.sign_[0]].x() - r.origin_.x) * r.inverse_direction_.x;
  tmax = (parameters[1 - r.sign_[0]].x() - r.origin_.x) * r.inverse_direction_.x;
  tymin = (parameters[r.sign_[1]].y() - r.origin_.y) * r.inverse_direction_.y;
  tymax = (parameters[1 - r.sign_[1]].y() - r.origin_.y) * r.inverse_direction_.y;
  if ((tmin > tymax) || (tymin > tmax)) return false;
  if (tymin > tmin) tmin = tymin;
  if (tymax < tmax) tmax = tymax;
  tzmin = (parameters[r.sign_[2]].z() - r.origin_.z) * r.inverse_direction_.z;
  tzmax = (parameters[1 - r.sign_[2]].z() - r.origin_.z) * r.inverse_direction_.z;
  if ((tmin > tzmax) || (tzmin > tmax)) return false;
  if (tzmin > tmin) tmin = tzmin;
  if (tzmax < tmax) tmax = tzmax;
  return ((tmin < t1) && (tmax > t0));
}
