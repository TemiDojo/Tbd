cmake_minimum_required(VERSION 4.3)

add_executable(tbd tbd.cpp)
target_link_libraries(tbd PRIVATE tbd::libtbd PkgConfig::libedit)

include(GNUInstallDirs)
install(
	TARGETS tbd
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
