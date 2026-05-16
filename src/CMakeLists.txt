#[[
we tell CMake to create a target called libtbd with a single source efile
libtbd.cpp
]]
add_library(libtbd process.cpp)

#[[
Its best practice to use namespaced library targets in CMake, as it
can result in more understandable errors if we make mistakes. we do
this by creating an alias
]]
add_library(tbd::libtbd ALIAS libtbd)

#[[
By default, CMake creates libraries called lib<target name> on linux.
since we don't want our cimpiled library to be called liblibtbd.a, we
tell CMake that our output library should be called tbd. Now it should
generate libtbd.a
]]
set_target_properties(
	libtbd
	PROPERTIES OUTPUT_NAME tbd
)
# We specify that the compiler should compile libtbd in C++17 mode.
target_compile_features(libtbd PUBLIC cxx_std_17)

target_include_directories(libtbd
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	PRIVATE 
		${CMAKE_SOURCE_DIR}/include
)	

include(GNUInstallDirs)
install(TARGETS libtbd
	EXPORT tbd-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
	EXPORT tbd-targets
	FILE tbd-config.cmake
	NAMESPACE tbd::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tbd
)

