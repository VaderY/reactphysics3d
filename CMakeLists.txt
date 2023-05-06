# Minimum cmake version required
cmake_minimum_required(VERSION 3.10)

# Project configuration
project(RP3D VERSION 0.9.0 LANGUAGES CXX)
 
# In order to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

# Set default build type
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# CMake modules path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

# Enable testing
enable_testing()

# Options
option(RP3D_COMPILE_TESTBED "Select this if you want to build the testbed application with demos" OFF)
option(RP3D_COMPILE_TESTS "Select this if you want to build the unit tests" OFF)
option(RP3D_PROFILING_ENABLED "Select this if you want to compile for performanace profiling" OFF)
option(RP3D_CODE_COVERAGE_ENABLED "Select this if you need to build for code coverage calculation" OFF)
option(RP3D_DOUBLE_PRECISION_ENABLED "Select this if you want to compile using double precision floating values" OFF)

# Code Coverage
if(RP3D_CODE_COVERAGE_ENABLED)
    INCLUDE(CodeCoverage)
    SETUP_TARGET_FOR_COVERAGE_LCOV(NAME coverage EXECUTABLE test/tests)
    APPEND_COVERAGE_COMPILER_FLAGS()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()

# Headers files
set (REACTPHYSICS3D_HEADERS
    "include/rp3d/configuration.h"
    "include/rp3d/decimal.h"
    "include/rp3d/rp3d.h"
    "include/rp3d/body/CollisionBody.h"
    "include/rp3d/body/RigidBody.h"
    "include/rp3d/collision/ContactPointInfo.h"
    "include/rp3d/collision/ContactManifoldInfo.h"
    "include/rp3d/collision/ContactPair.h"
    "include/rp3d/collision/broadphase/DynamicAABBTree.h"
    "include/rp3d/collision/narrowphase/CollisionDispatch.h"
    "include/rp3d/collision/narrowphase/GJK/VoronoiSimplex.h"
    "include/rp3d/collision/narrowphase/GJK/GJKAlgorithm.h"
    "include/rp3d/collision/narrowphase/SAT/SATAlgorithm.h"
    "include/rp3d/collision/narrowphase/NarrowPhaseAlgorithm.h"
    "include/rp3d/collision/narrowphase/SphereVsSphereAlgorithm.h"
    "include/rp3d/collision/narrowphase/CapsuleVsCapsuleAlgorithm.h"
    "include/rp3d/collision/narrowphase/SphereVsCapsuleAlgorithm.h"
    "include/rp3d/collision/narrowphase/SphereVsConvexPolyhedronAlgorithm.h"
    "include/rp3d/collision/narrowphase/CapsuleVsConvexPolyhedronAlgorithm.h"
    "include/rp3d/collision/narrowphase/ConvexPolyhedronVsConvexPolyhedronAlgorithm.h"
    "include/rp3d/collision/narrowphase/NarrowPhaseInput.h"
    "include/rp3d/collision/narrowphase/NarrowPhaseInfoBatch.h"
    "include/rp3d/collision/shapes/AABB.h"
    "include/rp3d/collision/shapes/ConvexShape.h"
    "include/rp3d/collision/shapes/ConvexPolyhedronShape.h"
    "include/rp3d/collision/shapes/ConcaveShape.h"
    "include/rp3d/collision/shapes/BoxShape.h"
    "include/rp3d/collision/shapes/CapsuleShape.h"
    "include/rp3d/collision/shapes/CollisionShape.h"
    "include/rp3d/collision/shapes/ConvexMeshShape.h"
    "include/rp3d/collision/shapes/SphereShape.h"
    "include/rp3d/collision/shapes/TriangleShape.h"
    "include/rp3d/collision/shapes/ConcaveMeshShape.h"
    "include/rp3d/collision/shapes/HeightFieldShape.h"
    "include/rp3d/collision/RaycastInfo.h"
    "include/rp3d/collision/Collider.h"
    "include/rp3d/collision/TriangleVertexArray.h"
    "include/rp3d/collision/PolygonVertexArray.h"
    "include/rp3d/collision/VertexArray.h"
    "include/rp3d/collision/TriangleMesh.h"
    "include/rp3d/collision/HeightField.h"
    "include/rp3d/collision/ConvexMesh.h"
    "include/rp3d/collision/HalfEdgeStructure.h"
    "include/rp3d/collision/ContactManifold.h"
    "include/rp3d/constraint/BallAndSocketJoint.h"
    "include/rp3d/constraint/ContactPoint.h"
    "include/rp3d/constraint/FixedJoint.h"
    "include/rp3d/constraint/HingeJoint.h"
    "include/rp3d/constraint/Joint.h"
    "include/rp3d/constraint/SliderJoint.h"
    "include/rp3d/engine/Entity.h"
    "include/rp3d/engine/EntityManager.h"
    "include/rp3d/engine/PhysicsCommon.h"
    "include/rp3d/systems/ConstraintSolverSystem.h"
    "include/rp3d/systems/ContactSolverSystem.h"
    "include/rp3d/systems/DynamicsSystem.h"
    "include/rp3d/systems/CollisionDetectionSystem.h"
    "include/rp3d/systems/SolveBallAndSocketJointSystem.h"
    "include/rp3d/systems/SolveFixedJointSystem.h"
    "include/rp3d/systems/SolveHingeJointSystem.h"
    "include/rp3d/systems/SolveSliderJointSystem.h"
    "include/rp3d/engine/PhysicsWorld.h"
    "include/rp3d/engine/EventListener.h"
    "include/rp3d/engine/Island.h"
    "include/rp3d/engine/Islands.h"
    "include/rp3d/engine/Material.h"
    "include/rp3d/engine/OverlappingPairs.h"
    "include/rp3d/systems/BroadPhaseSystem.h"
    "include/rp3d/components/Components.h"
    "include/rp3d/components/CollisionBodyComponents.h"
    "include/rp3d/components/RigidBodyComponents.h"
    "include/rp3d/components/TransformComponents.h"
    "include/rp3d/components/ColliderComponents.h"
    "include/rp3d/components/JointComponents.h"
    "include/rp3d/components/BallAndSocketJointComponents.h"
    "include/rp3d/components/FixedJointComponents.h"
    "include/rp3d/components/HingeJointComponents.h"
    "include/rp3d/components/SliderJointComponents.h"
    "include/rp3d/collision/CollisionCallback.h"
    "include/rp3d/collision/OverlapCallback.h"
    "include/rp3d/mathematics/mathematics.h"
    "include/rp3d/mathematics/mathematics_common.h"
    "include/rp3d/mathematics/mathematics_functions.h"
    "include/rp3d/mathematics/Matrix2x2.h"
    "include/rp3d/mathematics/Matrix3x3.h"
    "include/rp3d/mathematics/Quaternion.h"
    "include/rp3d/mathematics/Transform.h"
    "include/rp3d/mathematics/Vector2.h"
    "include/rp3d/mathematics/Vector3.h"
    "include/rp3d/mathematics/Ray.h"
    "include/rp3d/memory/MemoryAllocator.h"
    "include/rp3d/memory/PoolAllocator.h"
    "include/rp3d/memory/SingleFrameAllocator.h"
    "include/rp3d/memory/HeapAllocator.h"
    "include/rp3d/memory/DefaultAllocator.h"
    "include/rp3d/memory/MemoryManager.h"
    "include/rp3d/containers/Stack.h"
    "include/rp3d/containers/LinkedList.h"
    "include/rp3d/containers/Array.h"
    "include/rp3d/containers/Map.h"
    "include/rp3d/containers/Set.h"
    "include/rp3d/containers/Pair.h"
    "include/rp3d/containers/Deque.h"
    "include/rp3d/utils/Profiler.h"
    "include/rp3d/utils/Logger.h"
    "include/rp3d/utils/Message.h"
    "include/rp3d/utils/DefaultLogger.h"
    "include/rp3d/utils/DebugRenderer.h"
    "include/rp3d/utils/quickhull/QuickHull.h"
    "include/rp3d/utils/quickhull/QHHalfEdgeStructure.h"
)

# Source files
set (REACTPHYSICS3D_SOURCES
    "src/body/CollisionBody.cpp"
    "src/body/RigidBody.cpp"
    "src/collision/broadphase/DynamicAABBTree.cpp"
    "src/collision/narrowphase/CollisionDispatch.cpp"
    "src/collision/narrowphase/GJK/VoronoiSimplex.cpp"
    "src/collision/narrowphase/GJK/GJKAlgorithm.cpp"
    "src/collision/narrowphase/SAT/SATAlgorithm.cpp"
    "src/collision/narrowphase/SphereVsSphereAlgorithm.cpp"
    "src/collision/narrowphase/CapsuleVsCapsuleAlgorithm.cpp"
    "src/collision/narrowphase/SphereVsCapsuleAlgorithm.cpp"
    "src/collision/narrowphase/SphereVsConvexPolyhedronAlgorithm.cpp"
    "src/collision/narrowphase/CapsuleVsConvexPolyhedronAlgorithm.cpp"
    "src/collision/narrowphase/ConvexPolyhedronVsConvexPolyhedronAlgorithm.cpp"
    "src/collision/narrowphase/NarrowPhaseInput.cpp"
    "src/collision/narrowphase/NarrowPhaseInfoBatch.cpp"
    "src/collision/shapes/AABB.cpp"
    "src/collision/shapes/ConvexShape.cpp"
    "src/collision/shapes/ConvexPolyhedronShape.cpp"
    "src/collision/shapes/ConcaveShape.cpp"
    "src/collision/shapes/BoxShape.cpp"
    "src/collision/shapes/CapsuleShape.cpp"
    "src/collision/shapes/CollisionShape.cpp"
    "src/collision/shapes/ConvexMeshShape.cpp"
    "src/collision/shapes/SphereShape.cpp"
    "src/collision/shapes/TriangleShape.cpp"
    "src/collision/shapes/ConcaveMeshShape.cpp"
    "src/collision/shapes/HeightFieldShape.cpp"
    "src/collision/RaycastInfo.cpp"
    "src/collision/Collider.cpp"
    "src/collision/TriangleVertexArray.cpp"
    "src/collision/PolygonVertexArray.cpp"
    "src/collision/VertexArray.cpp"
    "src/collision/TriangleMesh.cpp"
    "src/collision/HeightField.cpp"
    "src/collision/ConvexMesh.cpp"
    "src/collision/HalfEdgeStructure.cpp"
    "src/collision/ContactManifold.cpp"
    "src/constraint/BallAndSocketJoint.cpp"
    "src/constraint/ContactPoint.cpp"
    "src/constraint/FixedJoint.cpp"
    "src/constraint/HingeJoint.cpp"
    "src/constraint/Joint.cpp"
    "src/constraint/SliderJoint.cpp"
    "src/engine/PhysicsCommon.cpp"
    "src/systems/ConstraintSolverSystem.cpp"
    "src/systems/ContactSolverSystem.cpp"
    "src/systems/DynamicsSystem.cpp"
    "src/systems/CollisionDetectionSystem.cpp"
    "src/systems/SolveBallAndSocketJointSystem.cpp"
    "src/systems/SolveFixedJointSystem.cpp"
    "src/systems/SolveHingeJointSystem.cpp"
    "src/systems/SolveSliderJointSystem.cpp"
    "src/engine/PhysicsWorld.cpp"
    "src/engine/Island.cpp"
    "src/engine/Material.cpp"
    "src/engine/OverlappingPairs.cpp"
    "src/engine/Entity.cpp"
    "src/engine/EntityManager.cpp"
    "src/systems/BroadPhaseSystem.cpp"
    "src/components/Components.cpp"
    "src/components/CollisionBodyComponents.cpp"
    "src/components/RigidBodyComponents.cpp"
    "src/components/TransformComponents.cpp"
    "src/components/ColliderComponents.cpp"
    "src/components/JointComponents.cpp"
    "src/components/BallAndSocketJointComponents.cpp"
    "src/components/FixedJointComponents.cpp"
    "src/components/HingeJointComponents.cpp"
    "src/components/SliderJointComponents.cpp"
    "src/collision/CollisionCallback.cpp"
    "src/collision/OverlapCallback.cpp"
    "src/mathematics/Matrix2x2.cpp"
    "src/mathematics/Matrix3x3.cpp"
    "src/mathematics/Quaternion.cpp"
    "src/mathematics/Transform.cpp"
    "src/mathematics/Vector2.cpp"
    "src/mathematics/Vector3.cpp"
    "src/memory/PoolAllocator.cpp"
    "src/memory/SingleFrameAllocator.cpp"
    "src/memory/HeapAllocator.cpp"
    "src/memory/MemoryManager.cpp"
    "src/memory/MemoryAllocator.cpp"
    "src/utils/Profiler.cpp"
    "src/utils/DefaultLogger.cpp"
    "src/utils/DebugRenderer.cpp"
    "src/utils/quickhull/QuickHull.cpp"
    "src/utils/quickhull/QHHalfEdgeStructure.cpp"
)

# Create the library
add_library(rp3d ${REACTPHYSICS3D_HEADERS} ${REACTPHYSICS3D_SOURCES})

# Creates an Alias Target, such that "RP3D::rp3d" can be used
# to refer to "rp3d" in subsequent commands
add_library(RP3D::rp3d ALIAS rp3d)

# C++11 compiler features
target_compile_features(rp3d PUBLIC cxx_std_17)
set_target_properties(rp3d PROPERTIES CXX_EXTENSIONS OFF)

# Library headers
target_include_directories(rp3d PUBLIC
              $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
)

# If we need to compile the testbed application
if(RP3D_COMPILE_TESTBED)
   add_subdirectory(testbed/)
endif()

# If we need to compile the tests
if(RP3D_COMPILE_TESTS)
   add_subdirectory(test/)
endif()

# Enable profiling if necessary
if(RP3D_PROFILING_ENABLED)
    target_compile_definitions(rp3d PUBLIC IS_RP3D_PROFILING_ENABLED)
endif()

# Enable double precision if necessary
if(RP3D_DOUBLE_PRECISION_ENABLED)
    target_compile_definitions(rp3d PUBLIC IS_RP3D_DOUBLE_PRECISION_ENABLED)
endif()

# Version number and soname for the library
set_target_properties(rp3d  PROPERTIES
          VERSION "0.9.0" 
          SOVERSION "0.9"
)

# Install target (install library only, not headers)
install(TARGETS rp3d
        EXPORT rp3d-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install the headers separately (because INSTALL(TARGETS ... PUBLIC_HEADER DESTINATION ...) does
# not support subfolders
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This is required so that the exported target has the name JSONUtils and not jsonutils
set_target_properties(rp3d PROPERTIES EXPORT_NAME RP3D)

# Give CMake access to the version number of the library so that the user is able to use the find_package() function
# with a given version number to select the version of the library he/she wants to use. This will create a
# "RP3DConfigVersion.cmake" file that will later be copied into the install destination folder (see below)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/RP3DConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# When the user runs "make install", this will export the targets into
# a "RP3DConfig.cmake" file in the install destination
install(EXPORT rp3d-targets
    FILE
        RP3DConfig.cmake
    NAMESPACE
        RP3D::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/RP3D
)

# When the user runs "make install", this will copy the "RP3DConfigVersion.cmake" file
# we have previously created into the install destination
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RP3DConfigVersion.cmake
    DESTINATION lib/cmake/RP3D
)

