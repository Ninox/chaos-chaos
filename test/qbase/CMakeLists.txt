SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test/qbase")

ADD_LIBRARY(matha SHARED plugin_math_a.c)
ADD_LIBRARY(mathb SHARED plugin_math_b.c)
ADD_LIBRARY(render SHARED plugin_render.c)
ADD_LIBRARY(str SHARED plugin_str.c)

IF(WIN32)
	INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/include/")
	LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/")
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test/qbase")

ADD_EXECUTABLE(xaloy_loader xaloy_loader.cc)
ADD_EXECUTABLE(xaloy_lua xaloy_lua.cc)
ADD_EXECUTABLE(xaloy_math xaloy_math.cc)
ADD_EXECUTABLE(xaloy_packer xaloy_packer.cc)

SET(BASIC_LIB qbase xaloy)

IF(WIN32)	
	IF(MSVC)
		SET(LUA_LIB qbase xaloy lua)
		SET(PACK_LIB qbase xaloy zlib)
	ELSE(MSVC)
		SET(LUA_LIB ${BASIC_LIB} lua)
		SET(PACK_LIB ${BASIC_LIB} z)
	ENDIF(MSVC)	
ELSE(WIN32)
	SET(LUA_LIB ${BASIC_LIB} lua dl)
	SET(PACK_LIB ${BASIC_LIB} z)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(xaloy_loader ${LUA_LIB})
TARGET_LINK_LIBRARIES(xaloy_lua ${LUA_LIB})
TARGET_LINK_LIBRARIES(xaloy_math ${BASIC_LIB})
TARGET_LINK_LIBRARIES(xaloy_packer ${PACK_LIB})
