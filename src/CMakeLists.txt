# Target srcs
file(GLOB_RECURSE TARGET_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)

# Target properties
add_library(alembicEntityQmlPlugin SHARED ${TARGET_SRCS})

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
