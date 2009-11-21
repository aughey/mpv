INCLUDE_DIRECTORIES(../../common)
INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})

SET( symbologyStress_SRCS 
	Diamond.cpp
	FrameRateMonitor.cpp
	InstanceIDPool.cpp
	Misc.cpp
	Pinwheel.cpp
	SerializableSymbol.cpp
	SerializableSymbolLine.cpp
	SymbolSet.cpp
	SymbologyStress.cpp
	SymbologyStressTestFunctor.cpp
)

ADD_EXECUTABLE(symbologyStress ${symbologyStress_SRCS})
TARGET_LINK_LIBRARIES(symbologyStress mpvcommon ${CCL_LIBRARY})
