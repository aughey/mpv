INCLUDE_DIRECTORIES(../common)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})

SET(mpvhps_PUBLIC_HDRS
    AccelOperator.h
    AngularAccelOperator.h
    CenteredPlacer.h
    Counter.h
    Emitter.h
    Export.h
    FiniteCounter.h
    FluidFrictionOperator.h
    FollowOperator.h
    HPSFactory.h
    ImageCache.h
    ForceOperator.h
    Interpolator.h
    LinearInterpolator.h
    ModularEmitter.h
    MultiEmitter.h
    ModularProgram.h
    MultiSegmentPlacer.h
    Operator.h
    Particle.h
    ParticleProcessor.h
    ParticleSystem.h
    ParticleSystemUpdater.h
    Placer.h
    PointPlacer.h
    Program.h
    RadialShooter.h
    RandomRateCounter.h
    SectorPlacer.h
    SegmentPlacer.h
    Shooter.h
    SpherePlacer.h
    TextureAnimOperator.h
    ThermalUpdraftOperator.h
    VariableRateCounter.h
    Version.h
    range.h
)
SET(mpvhps_SRCS
    Emitter.cpp
    FluidFrictionOperator.cpp
    FollowOperator.cpp
    HPSFactory.cpp
    ImageCache.cpp
    ModularEmitter.cpp
    ModularProgram.cpp
    MultiEmitter.cpp
    MultiSegmentPlacer.cpp
    Particle.cpp
    ParticleIntersectingQuads.cpp
    ParticleProcessor.cpp
    ParticleSystem.cpp
    ParticleSystemUpdater.cpp
    ParticleTrail.cpp
    Program.cpp
    TextureAnimOperator.cpp
    ThermalUpdraftOperator.cpp
    Version.cpp
    range.cpp
)

ADD_LIBRARY(mpvhps SHARED
    ${mpvhps_PUBLIC_HDRS} ${mpvhps_PRIVATE_HDRS} ${mpvhps_SRCS})

IF(CMAKE_MAJOR_VERSION EQUAL 2 AND
   CMAKE_MINOR_VERSION LESS 6)
MPV_COPY_DLL_OUTPUT_TO_BINARY_DIR(mpvhps)
ENDIF()

SET_TARGET_PROPERTIES(mpvhps PROPERTIES DEFINE_SYMBOL HPS_EXPORTS)
IF(WIN32)
    # Also define OSGPARTICLE_LIBRARY to control DLL exports
    ADD_DEFINITIONS(-DOSGPARTICLE_LIBRARY)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(mpvhps mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(mpvhps
    ${OSGDB_LIBRARY}
    ${OSGUTIL_LIBRARY}
    ${OSG_LIBRARY})
TARGET_LINK_LIBRARIES(mpvhps ${OPENGL_LIBRARIES})

INSTALL(TARGETS mpvhps
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
