SET(QBASE_SRC_LIST
		basedebug.c
		baseloader.c
		basemath.c
		basepacker.c
		basescript.c
   )

IF(WIN32)
	INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/include")
	LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib")
ENDIF()
   
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
   
ADD_LIBRARY(qbase STATIC ${QBASE_SRC_LIST})
   
#	LINK LIBRARIES
IF(WIN32)
	IF(MSVC)
		TARGET_LINK_LIBRARIES(qbase lua.lib zlib.lib)
	ELSE()
		TARGET_LINK_LIBRARIES(qbase lua z)
	ENDIF()
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(qbase lua z dl)
ENDIF(WIN32)