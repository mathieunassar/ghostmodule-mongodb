##########################################################################################################################################
######################################################## PERSISTENCE_MONGODB MODULE ######################################################
##########################################################################################################################################

file(GLOB header_persistencemongodb_lib
	${GHOST_MODULE_MONGODB_ROOT_DIR}/include/ghost/persistence_mongodb/DatabaseMongoDB.hpp
)

file(GLOB header_persistencemongodb_internal_lib
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DatabaseMongoDB.hpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DataCollectionMongoDB.hpp
)

file(GLOB source_persistencemongodb_lib
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DatabaseMongoDB.cpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DataCollectionMongoDB.cpp
)

source_group("API" FILES ${header_persistencemongodb_lib})
source_group("Header Files" FILES ${header_persistencemongodb_internal_lib})
source_group("Source Files" FILES ${source_persistencemongodb_lib})

# module_minimum module
add_library(ghost_persistence_mongodb
	${header_persistencemongodb_lib}
	${header_persistencemongodb_internal_lib}
	${source_persistencemongodb_lib})
target_link_libraries(ghost_persistence_mongodb CONAN_PKG::ghostmodule CONAN_PKG::mongo-cxx-driver)

##########################################################################################################################################
