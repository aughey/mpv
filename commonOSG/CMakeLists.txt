INCLUDE_DIRECTORIES(../common)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})

SET(commonOSG_PUBLIC_HDRS
	EntityElement.h
	EntityElementFactory.h
	EntityImpOSG.h
	MiscOSG.h
	SwitchNodeComponentImp.h
	SymbolImpOSG.h
	SymbolSurfaceImpOSG.h
	TransformNodeArticulationImp.h
)
SET(commonOSG_SRCS
	EntityElement.cpp
	EntityElementFactory.cpp
	EntityImpOSG.cpp
	MiscOSG.cpp
	SwitchNodeComponentImp.cpp
	SymbolImpOSG.cpp
	SymbolSurfaceImpOSG.cpp
	TransformNodeArticulationImp.cpp
)

ADD_LIBRARY(mpvcommonosg SHARED
    ${commonOSG_PUBLIC_HDRS} ${commonOSG_PRIVATE_HDRS} ${commonOSG_SRCS})
IF(CMAKE_MAJOR_VERSION EQUAL 2 AND
   CMAKE_MINOR_VERSION LESS 6)
   MPV_COPY_DLL_OUTPUT_TO_BINARY_DIR(mpvcommonosg)
ENDIF()

SET_TARGET_PROPERTIES(mpvcommonosg PROPERTIES
    DEFINE_SYMBOL _EXPORT_MPVCMNOSG_)

TARGET_LINK_LIBRARIES(mpvcommonosg mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(mpvcommonosg 
	${OSGDB_LIBRARY} ${OSGSIM_LIBRARY} ${OSG_LIBRARY})

INSTALL(TARGETS mpvcommonosg
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
