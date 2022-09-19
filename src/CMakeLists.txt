# Target srcs
set(PLUGIN_SOURCES AlembicEntity.cpp BaseAlembicObject.cpp CameraLocatorEntity.cpp IOThread.cpp PointCloudEntity.cpp)
set(PLUGIN_HEADERS AlembicEntity.hpp BaseAlembicObject.hpp CameraLocatorEntity.hpp IOThread.hpp PointCloudEntity.hpp plugin.hpp)

# Target properties
add_library(alembicEntityQmlPlugin SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

target_link_libraries(alembicEntityQmlPlugin
  PUBLIC
    Qt6::Core
    Qt6::Qml
    Qt6::3DCore
    Qt6::3DRender
    Alembic::Alembic
  PRIVATE
    Qt6::3DExtras
)

set_target_properties(alembicEntityQmlPlugin
        PROPERTIES
        DEBUG_POSTFIX "d"
        FOLDER "alembicEntityQmlPlugin"
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
        )

target_include_directories(alembicEntityQmlPlugin PUBLIC ${ILMBASE_INCLUDE_DIR})

# Install settings
install(FILES "qmldir"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity)
install(TARGETS alembicEntityQmlPlugin
        DESTINATION "${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity")
