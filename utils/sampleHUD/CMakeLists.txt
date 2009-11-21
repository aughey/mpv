INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})

SET( sampleHUD_SRCS 
	AirspeedBox.cpp
	AltitudeBox.cpp
	FrameRateMonitor.cpp
	HeadingCluster.cpp
	HUDDefaultMode.cpp
	HUDState.cpp
	InstanceIDPool.cpp
	LowerLeftCluster.cpp
	Misc.cpp
	PitchLadder.cpp
	SampleHUD.cpp
	SerializableSymbol.cpp
	SerializableSymbolCircle.cpp
	SerializableSymbolLine.cpp
	SerializableSymbolText.cpp
	SymbolSet.cpp
	VelocityVector.cpp
)

ADD_EXECUTABLE(sampleHUD ${sampleHUD_SRCS})
TARGET_LINK_LIBRARIES(sampleHUD mpvcommon ${CCL_LIBRARY})
