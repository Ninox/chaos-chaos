SET(QBASE_SRC_LIST
		basedebug.c
		baseloader.c
		basemath.c
		basepacker.c
		basescript.c
   )

ADD_LIBRARY(qbase STATIC ${QBASE_SRC_LIST})
   
#	LINK LIBRARIES
INCLUDE_DIRECTORIES("../../../lib/lua52/include" "../../../lib/zlib/include")
IF(MINGW)
	TARGET_LINK_LIBRARIES(qbase "../../../lib/lua52/liblua.a" "../../../lib/zlib/libz.a")
ELSEIF(MSVC)
	TARGET_LINK_LIBRARIES(qbase "../../../lib/lua52/lua.lib" "../../../lib/zlib/zlib.lib")
ENDIF(MINGW)
