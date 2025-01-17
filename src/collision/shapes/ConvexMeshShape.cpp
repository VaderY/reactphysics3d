/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2022 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

// Libraries
#include <rp3d/configuration.h>
#include <rp3d/collision/shapes/ConvexMeshShape.h>
#include <rp3d/engine/PhysicsWorld.h>
#include <rp3d/collision/RaycastInfo.h>

using namespace rp3d;

// Constructor to initialize with an array of 3D vertices.
/// This method creates an internal copy of the input vertices.
/**
 * @param arrayVertices Array with the vertices of the convex mesh
 * @param nbVertices Number of vertices in the convex mesh
 * @param stride Stride between the beginning of two elements in the vertices array
 * @param margin Collision margin (in meters) around the collision shape
 */
ConvexMeshShape::ConvexMeshShape(ConvexMesh* convexMesh, MemoryAllocator& allocator, const Vector3& scale)
                : ConvexPolyhedronShape(CollisionShapeName::CONVEX_MESH, allocator), mConvexMesh(convexMesh),
                  mScale(scale) {

}

// Return a local support point in a given direction without the object margin.
/// If the edges information is not used for collision detection, this method will go through
/// the whole vertices array and pick up the vertex with the largest dot product in the support
/// direction. This is an O(n) process with "n" being the number of vertices in the mesh.
/// However, if the edges information is used, we can cache the previous support vertex and use
/// it as a start in a hill-climbing (local search) process to find the new support vertex which
/// will be in most of the cases very close to the previous one. Using hill-climbing, this method
/// runs in almost constant time.
Vector3 ConvexMeshShape::getLocalSupportPointWithoutMargin(const Vector3& direction) const {

    decimal maxDotProduct = DECIMAL_SMALLEST;
    uint32 indexMaxDotProduct = 0;

    // For each vertex of the mesh
    for (uint32 i=0; i<mConvexMesh->getNbVertices(); i++) {

        // Compute the dot product of the current vertex
        decimal dotProduct = direction.dot(mConvexMesh->getVertex(i));

        // If the current dot product is larger than the maximum one
        if (dotProduct > maxDotProduct) {
            indexMaxDotProduct = i;
            maxDotProduct = dotProduct;
        }
    }

    assert(maxDotProduct >= decimal(0.0));

    // Return the vertex with the largest dot product in the support direction
    return mConvexMesh->getVertex(indexMaxDotProduct) * mScale;
}

// Raycast method with feedback information
/// This method implements the technique in the book "Real-time Collision Detection" by
/// Christer Ericson.
bool ConvexMeshShape::raycast(const Ray& ray, RaycastInfo& raycastInfo, Collider* collider, MemoryAllocator& /*allocator*/) const {

    // Ray direction
    Vector3 direction = ray.point2 - ray.point1;

    decimal tMin = decimal(0.0);
    decimal tMax = ray.maxFraction;
    Vector3 currentFaceNormal;
    bool isIntersectionFound = false;

    const HalfEdgeStructure& halfEdgeStructure = mConvexMesh->getHalfEdgeStructure();

    // For each face of the convex mesh
    for (uint32 f=0; f < mConvexMesh->getNbFaces(); f++) {

        const HalfEdgeStructure::Face& face = halfEdgeStructure.getFace(f);
        const Vector3& faceNormal = mConvexMesh->getFaceNormal(f);
        const HalfEdgeStructure::Vertex& faceVertex = halfEdgeStructure.getVertex(face.faceVertices[0]);
        const Vector3& facePoint = mConvexMesh->getVertex(faceVertex.vertexPointIndex);
        decimal denom = faceNormal.dot(direction);
        decimal planeD = faceNormal.dot(facePoint);
        decimal dist = planeD -  faceNormal.dot(ray.point1);

        // If ray is parallel to the face
        if (denom == decimal(0.0)) {

            // If ray is outside the clipping face, we return no intersection
            if (dist < decimal(0.0)) return false;
        }
        else {

            // Compute the intersection between the ray and the current face plane
            decimal t = dist / denom;

            // Update the current ray intersection by clipping it with the current face plane
            // If the place faces the ray
            if (denom < decimal(0.0)) {
                // Clip the current ray intersection as it enters the convex mesh
                if (t > tMin) {
                    tMin = t;
                    currentFaceNormal = faceNormal;
                    isIntersectionFound = true;
                }
            }
            else {
                // Clip the current ray intersection as it exits the convex mesh
                if (t < tMax) tMax = t;
            }

            // If the ray intersection with the convex mesh becomes empty, report no intersection
            if (tMin > tMax) return false;
        }
    }

    if (isIntersectionFound) {

        // The ray intersects with the convex mesh
        assert(tMin >= decimal(0.0));
        assert(tMax <= ray.maxFraction);
        assert(tMin <= tMax);
        assert(currentFaceNormal.lengthSquare() > decimal(0.0));

        // The ray intersects the three slabs, we compute the hit point
        Vector3 localHitPoint = ray.point1 + tMin * direction;

        raycastInfo.hitFraction = tMin;
        raycastInfo.body = collider->getBody();
        raycastInfo.collider = collider;
        raycastInfo.worldPoint = localHitPoint;
        raycastInfo.worldNormal = currentFaceNormal;

        return true;
    }

    return false;
}

// Return true if a point is inside the collision shape
bool ConvexMeshShape::testPointInside(const Vector3& localPoint, Collider* /*collider*/) const {

    const HalfEdgeStructure& halfEdgeStructure = mConvexMesh->getHalfEdgeStructure();

    // For each face plane of the convex mesh
    for (uint32 f=0; f < mConvexMesh->getNbFaces(); f++) {

        const HalfEdgeStructure::Face& face = halfEdgeStructure.getFace(f);
        const Vector3& faceNormal = mConvexMesh->getFaceNormal(f);
        const HalfEdgeStructure::Vertex& faceVertex = halfEdgeStructure.getVertex(face.faceVertices[0]);
        const Vector3& facePoint = mConvexMesh->getVertex(faceVertex.vertexPointIndex);

        // If the point is out of the face plane, it is outside of the convex mesh
        if (computePointToPlaneDistance(localPoint, faceNormal, facePoint) > decimal(0.0)) return false;
    }

    return true;
}

// Return the local bounds of the shape in x, y and z directions
/**
 * @return The AABB with the min/max bounds
 */
AABB ConvexMeshShape::getLocalBounds() const {
    AABB aabb = mConvexMesh->getBounds();
    aabb.applyScale(mScale);
    return aabb;
}

// Return the string representation of the shape
std::string ConvexMeshShape::to_string() const {

    std::stringstream ss;
    ss << "ConvexMeshShape{" << std::endl;
    ss << "nbVertices=" << mConvexMesh->getNbVertices() << std::endl;
    ss << "nbFaces=" << mConvexMesh->getNbFaces() << std::endl;

    ss << "vertices=[";

    for (uint32 v=0; v < mConvexMesh->getNbVertices(); v++) {

        const Vector3& vertex = mConvexMesh->getVertex(v);
        ss << vertex.to_string();
        if (v != mConvexMesh->getNbVertices() - 1) {
            ss << ", ";
        }
    }

    ss << "], faces=[";

    HalfEdgeStructure halfEdgeStruct = mConvexMesh->getHalfEdgeStructure();
    for (uint32 f=0; f < mConvexMesh->getNbFaces(); f++) {

        const HalfEdgeStructure::Face& face = halfEdgeStruct.getFace(f);

        ss << "[";

        for (uint32 v=0; v < face.faceVertices.size(); v++) {

            ss << face.faceVertices[v];
            if (v != face.faceVertices.size() - 1) {
               ss << ",";
            }
        }

        ss << "]";
    }

    ss << "]}";

    return ss.str();
}

