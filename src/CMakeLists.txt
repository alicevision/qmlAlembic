# Target srcs
set(PLUGIN_SOURCES AlembicEntity.cpp BaseAlembicObject.cpp CameraLocatorEntity.cpp IOThread.cpp PointCloudEntity.cpp)
set(PLUGIN_HEADERS AlembicEntity.hpp BaseAlembicObject.hpp CameraLocatorEntity.hpp IOThread.hpp PointCloudEntity.hpp plugin.hpp)

# Target properties
add_library(alembicEntityQmlPlugin SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

target_link_libraries(alembicEntityQmlPlugin
  PUBLIC
    Qt5::Core
    Qt5::Qml
    Qt5::3DCore
    Qt5::3DRender
    Alembic::Alembic
  PRIVATE
    Qt5::3DExtras
)

# CMake Find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Add missing include path to IlmBase/OpenEXR include folder
find_package(IlmBase REQUIRED)
target_include_directories(alembicEntityQmlPlugin PUBLIC ${ILMBASE_INCLUDE_DIR})

# Install settings
install(FILES "qmldir"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity)
install(TARGETS alembicEntityQmlPlugin
        DESTINATION "${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity")
