INCLUDE_DIRECTORIES(../../common)
INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})

ADD_EXECUTABLE(symbologyTest symbologyTest.cpp)
TARGET_LINK_LIBRARIES(symbologyTest mpvcommon ${CCL_LIBRARY})
