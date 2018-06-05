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

# Install settings
install(FILES "qmldir"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity)
install(TARGETS alembicEntityQmlPlugin
        DESTINATION "${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity")
