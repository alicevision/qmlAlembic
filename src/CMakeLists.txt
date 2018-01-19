# Target srcs
file(GLOB_RECURSE TARGET_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)

# Target properties
add_library(alembicEntityQmlPlugin SHARED ${TARGET_SRCS})
target_link_libraries(alembicEntityQmlPlugin ${ABC_LIBRARIES})
QT5_USE_MODULES(alembicEntityQmlPlugin Core Qml Quick 3DCore 3DRender 3DExtras)

# Install settings
install(FILES "qmldir"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity)
install(TARGETS alembicEntityQmlPlugin
        DESTINATION "${CMAKE_INSTALL_PREFIX}/qml/AlembicEntity")
