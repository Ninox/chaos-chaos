SET(QBASE_SRC_LIST
		basedebug.c
		baseloader.c
		basemath.c
		basepacker.c
		basescript.c
   )

ADD_LIBRARY(qbase STATIC ${QBASE_SRC_LIST})
   
#	LINK LIBRARIES
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/include")
LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib")
IF(MINGW)
	TARGET_LINK_LIBRARIES(qbase lua z)
ELSEIF(MSVC)
	TARGET_LINK_LIBRARIES(qbase lua.lib zlib.lib)
ENDIF(MINGW)
