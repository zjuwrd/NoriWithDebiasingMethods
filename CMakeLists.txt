cmake_minimum_required (VERSION 3.8)
project(nori)

add_subdirectory(ext ext_build)

include_directories(
  # Nori include files
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  # tinyformat string formatting library
  ${TFM_INCLUDE_DIR}
  # Eigen linear algebra library
  SYSTEM ${EIGEN_INCLUDE_DIR}
  # OpenEXR high dynamic range bitmap library
  SYSTEM ${OPENEXR_INCLUDE_DIRS}
  # Intel Thread Building Blocks
  SYSTEM ${TBB_INCLUDE_DIR}
  # Pseudorandom number generator
  ${PCG32_INCLUDE_DIR}
  # PugiXML parser
  ${PUGIXML_INCLUDE_DIR}
  # Helper functions for statistical hypothesis tests
  ${HYPOTHESIS_INCLUDE_DIR}
  # GLFW library for OpenGL context creation
  SYSTEM ${GLFW_INCLUDE_DIR}
  # GLEW library for accessing OpenGL functions
  SYSTEM ${GLEW_INCLUDE_DIR}
  # NanoVG drawing library
  SYSTEM ${NANOVG_INCLUDE_DIR}
  # NanoGUI user interface library
  SYSTEM ${NANOGUI_INCLUDE_DIR}
  SYSTEM ${NANOGUI_EXTRA_INCS}
  # Portable filesystem API
  SYSTEM ${FILESYSTEM_INCLUDE_DIR}
  # STB Image Write
  SYSTEM ${STB_IMAGE_WRITE_INCLUDE_DIR}
)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

# The following lines build the main executable. If you add a source
# code file to Nori, be sure to include it in this list.
add_executable(nori

  # Header files
  include/nori/medium.h
  include/nori/bvh.h
  include/nori/bbox.h
  include/nori/bitmap.h
  include/nori/block.h
  include/nori/bsdf.h
  include/nori/accel.h
  include/nori/camera.h
  include/nori/color.h
  include/nori/common.h
  include/nori/dpdf.h
  include/nori/frame.h
  include/nori/integrator.h
  include/nori/emitter.h
  include/nori/mesh.h
  include/nori/object.h
  include/nori/parser.h
  include/nori/proplist.h
  include/nori/ray.h
  include/nori/rfilter.h
  include/nori/sampler.h
  include/nori/scene.h
  include/nori/timer.h
  include/nori/transform.h
  include/nori/vector.h
  include/nori/warp.h
  include/nori/kdtree.h
  src/integrator/photon/photon.hpp
  src/medium/phasefunction.hpp
  
  
  # Source code files
  src/integrator/debiasedPPM.cpp
  src/integrator/debiasedPM.cpp
  src/integrator/SPPM.cpp
  src/integrator/photon/photon.cpp
  src/mesh/cube.cpp
  src/mesh/rectangle.cpp
  src/medium/homogeneous.cpp
  src/integrator/PM.cpp
  src/integrator/path_mis.cpp
  src/integrator/path_ems.cpp
  src/integrator/path_mats.cpp
  src/integrator/whitted.cpp
  src/integrator/ao.cpp
  src/integrator/simple.cpp
  src/light/area.cpp
  src/BSDF/diffuse.cpp
  src/BSDF/microfacet.cpp
  src/BSDF/mirror.cpp
  src/BSDF/dielectric.cpp
  src/BSDF/disney.cpp
  src/accelerate/bvh.cpp
  src/accelerate/accel.cpp
  src/distribute/warp.cpp
  src/core/normals.cpp
  src/core/bitmap.cpp
  src/core/block.cpp
  src/core/chi2test.cpp
  src/core/common.cpp
  src/core/gui.cpp
  src/core/independent.cpp
  src/core/main.cpp
  src/core/mesh.cpp
  src/core/obj.cpp
  src/core/object.cpp
  src/core/parser.cpp
  src/core/perspective.cpp
  src/core/proplist.cpp
  src/core/rfilter.cpp
  src/core/scene.cpp
  src/core/ttest.cpp
  )

add_definitions(${NANOGUI_EXTRA_DEFS})

# The following lines build the warping test application
add_executable(warptest
  include/nori/warp.h
  src/distribute/warp.cpp
  src/core/warptest.cpp
  src/BSDF/microfacet.cpp
  src/core/object.cpp
  src/core/proplist.cpp
  src/core/common.cpp
)

if (WIN32)
  target_link_libraries(nori tbb_static pugixml IlmImf nanogui ${NANOGUI_EXTRA_LIBS} zlibstatic)
else()
  target_link_libraries(nori tbb_static pugixml IlmImf nanogui ${NANOGUI_EXTRA_LIBS})
endif()

target_link_libraries(warptest tbb_static nanogui ${NANOGUI_EXTRA_LIBS})

# Force colored output for the ninja generator
if (CMAKE_GENERATOR STREQUAL "Ninja")
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  endif()
endif()

target_compile_features(warptest PRIVATE cxx_std_20)
target_compile_features(nori PRIVATE cxx_std_20)

# vim: set et ts=2 sw=2 ft=cmake nospell:
