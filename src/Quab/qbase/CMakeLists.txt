SET(QBASE_SRC_LIST
		basedebug.c
		baseloader.c
		basemath.c
		basepacker.c
		basescript.c
   )

SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
   
ADD_LIBRARY(qbase STATIC ${QBASE_SRC_LIST})
   
#	LINK LIBRARIES
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/include")
LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib")

IF(WIN32)
	IF(MSVC)
		TARGET_LINK_LIBRARIES(qbase lua.lib zlib.lib)
	ELSE()
		TARGET_LINK_LIBRARIES(qbase lua z)
	ENDIF(MSVC)
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(qbase lua z dl)
ENDIF(WIN32)