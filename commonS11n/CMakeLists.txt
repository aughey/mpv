INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})

SET(commonS11n_PUBLIC_HDRS
	DoubleNode.h
	DoubleVectorNode.h
	FloatNode.h
	GroupNode.h
	IntNode.h
	Node.h
	NodeVisitor.h
	StringNode.h
	SymbolImpS11n.h
	SymbolSurfaceImpS11n.h
)

SET(commonS11n_SRCS
	DoubleNode.cpp
	DoubleVectorNode.cpp
	FloatNode.cpp
	GroupNode.cpp
	IntNode.cpp
	Node.cpp
	NodeVisitor.cpp
	StringNode.cpp
	SymbolImpS11n.cpp
	SymbolSurfaceImpS11n.cpp
)

ADD_LIBRARY(mpvcommons11n SHARED
	${commonS11n_PUBLIC_HDRS} ${commonS11n_PRIVATE_HDRS} ${commonS11n_SRCS})
IF(CMAKE_MAJOR_VERSION EQUAL 2 AND
	CMAKE_MINOR_VERSION LESS 6)
	MPV_COPY_DLL_OUTPUT_TO_BINARY_DIR(mpvcommons11n)
ENDIF()

SET_TARGET_PROPERTIES(mpvcommons11n PROPERTIES
	DEFINE_SYMBOL _EXPORT_MPVCMNOSG_)

TARGET_LINK_LIBRARIES(mpvcommons11n mpvcommon)
#MPV_TARGET_LINK_OSG_LIBRARIES(mpvcommons11n 
#	${OSGDB_LIBRARY} ${OSGSIM_LIBRARY} ${OSG_LIBRARY})

INSTALL(TARGETS mpvcommons11n
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
