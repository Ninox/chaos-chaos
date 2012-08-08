SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/qbase)

ADD_LIBRARY(matha SHARED plugin_math_a.c)
ADD_LIBRARY(mathb SHARED plugin_math_b.c)
ADD_LIBRARY(render SHARED plugin_render.c)
ADD_LIBRARY(str SHARED plugin_str.c)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/include/")
LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/")

ADD_EXECUTABLE(xaloy_loader xaloy_loader.cc)
ADD_EXECUTABLE(xaloy_lua xaloy_lua.cc)
ADD_EXECUTABLE(xaloy_math xaloy_math.cc)
ADD_EXECUTABLE(xaloy_packer xaloy_packer.cc)

TARGET_LINK_LIBRARIES(xaloy_loader qbase xaloy lua)
TARGET_LINK_LIBRARIES(xaloy_lua qbase xaloy lua)
TARGET_LINK_LIBRARIES(xaloy_math qbase xaloy)
TARGET_LINK_LIBRARIES(xaloy_packer qbase xaloy z)
