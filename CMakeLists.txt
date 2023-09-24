cmake_minimum_required(VERSION 3.21.0)

set( CMAKE_PROJECT_NAME ndMeshDecomposing )
set( CMAKE_CXX_STANDARD 14 )
project(ndMeshDecomposing)
set( ND_LIBDIR "lib/release")

include_directories( inc )

if( MSVC )
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_SCL_SECURE_NO_WARNINGS=1" )
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -D_SECURE_STL=0" )
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_SECURE_STL=0 -D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0" )
  add_definitions( "/Oy-" )
endif( MSVC )

set (SOURCE_FILES hacd.cpp ndConvexDecomposition.cpp
  nd_EnterExitTracer.cpp
  nd_hacdUtils.cpp
  nd_hacdStructs.cpp
  nd_StructTracer.cpp
  nd_hacdConvexDecomposition.cpp
  )

file(GLOB HEADER_FILES *.h)

add_library( ndMeshDecomposing STATIC ${SOURCE_FILES} ${HEADER_FILES})

install( TARGETS ndMeshDecomposing DESTINATION ${ND_LIBDIR} )
install( FILES llconvexdecomposition.h DESTINATION include )
install( FILES ndConvexDecomposition.h DESTINATION include )

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_GENERATOR "TBZ2")
set(CPACK_STRIP_FILES TRUE )
include(CPack)
