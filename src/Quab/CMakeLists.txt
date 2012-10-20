#-----------------------------------------------------
#			Quab Engine's Core Module
#-----------------------------------------------------

#	SET BUILD OPTIONS
IF(WIN32)
	INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib/include")
	LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../lib")
ENDIF()

ADD_DEFINITIONS(-DQUAB_LIBRARY)

SET(QUAB_ROOT ${PROJECT_SOURCE_DIR}/Quab/)
SET(QBASE_ROOT ${QUAB_ROOT}/qbase/)

SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

#	SET QBASE
SET(QBASE_SRC 
		${QBASE_ROOT}/basedebug.c
		${QBASE_ROOT}/basemath.c
		${QBASE_ROOT}/baseloader.c
		${QBASE_ROOT}/basepacker.c
		${QBASE_ROOT}/basescript.c
)
#	BUILD AI
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabIntelligent.cc
)
#	BUILD Control
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabController.cc
)
#	BUILD GUI
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabMainform.cc
		${QUAB_ROOT}/QuabTookits.cc
)

#	BUILD Math
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabMath.cc
)
#	BUILD RESOURCE
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabResource.cc
        ${QUAB_ROOT}/QuabStream.cc
		${QUAB_ROOT}/QuabSpriteFactory.cc
)
#	BUILD Render
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabPhysical.cc
		${QUAB_ROOT}/QuabRender.cc
		${QUAB_ROOT}/QuabScreen.cc
		${QUAB_ROOT}/QuabSprite.cc
)
#	BUILD Script
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabLua.cc
)
#	BUILD Sound
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabMusic.cc
)
#	build debug
SET(QUAB_SRC
		${QUAB_SRC}
		${QUAB_ROOT}/QuabLog.cc
)

#	ready to build
SET(QUAB_SRC
		${QBASE_SRC}
		${QUAB_SRC}
		${QUAB_ROOT}/QuabWorld.cc
)

ADD_LIBRARY(Quab SHARED ${QUAB_SRC})

#	link library
IF(WIN32)
	IF(MSVC)
		TARGET_LINK_LIBRARIES(Quab lua.lib zlib.lib)
	ELSE()
		TARGET_LINK_LIBRARIES(Quab lua z)
	ENDIF()
ELSE(WIN32)
	TARGET_LINK_LIBRARIES(Quab lua z dl)
ENDIF(WIN32)
