# Each app gets one and only one "main" cpp file in this dir, <app_name>.cpp.
# Each app also (optionally) gets one subfolder at ./lib_<app_name> to contain its personal 
# not-intended-for-re-use library code which will automatically get detected
# and compiled using file globs as long as you follow the naming convention. 
# No need for a CMakeLists.txt in the subdirectories as we don't add a lib or 
# executable there etc.

# CMake 3.12 or later:
file(GLOB APP_LIST "./*.cpp")

foreach(FILE_NAME ${APP_LIST})
    get_filename_component(APP_NAME ${FILE_NAME} NAME_WLE)
	file(GLOB IMPL_LIST "lib_${APP_NAME}/*.cpp")
	add_executable(${APP_NAME} "${APP_NAME}.cpp" "${IMPL_LIST}")
	target_compile_features(${APP_NAME} PRIVATE cxx_std_17)
	target_link_libraries(${APP_NAME} PRIVATE algo_lib)
	if(WIN32)
    	set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
	endif()
endforeach()
