##########################################################################################################################################
######################################################## PERSISTENCE_MONGODB MODULE ######################################################
##########################################################################################################################################

file(GLOB header_persistencemongodb_lib
	${GHOST_MODULE_MONGODB_ROOT_DIR}/include/ghost/persistence_mongodb/DatabaseMongoDB.hpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/include/ghost/persistence_mongodb/PersistenceMongoDBExtension.hpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/include/ghost/persistence_mongodb/PersistenceMongoDBExtensionBuilder.hpp
)

file(GLOB header_persistencemongodb_internal_lib
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DatabaseMongoDB.hpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DataCollectionMongoDB.hpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/PersistenceMongoDBExtension.hpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/PersistenceMongoDBExtensionBuilder.hpp
)

file(GLOB source_persistencemongodb_lib
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DatabaseMongoDB.cpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/DataCollectionMongoDB.cpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/PersistenceMongoDBExtension.cpp
	${GHOST_MODULE_MONGODB_ROOT_DIR}/src/persistence_mongodb/PersistenceMongoDBExtensionBuilder.cpp
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
##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_MONGODB_ROOT_DIR}/examples/protobuf/)
	set(PROTOC_SOURCE_FILE ${GHOST_MODULE_MONGODB_ROOT_DIR}/examples/protobuf/persistency_todo_list.proto)
	set(PROTOC_OUTPUT ${GHOST_MODULE_MONGODB_ROOT_DIR}/examples/protobuf/)

	generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

	# minimum code to create a module
	add_executable(persistence_todo_list
		${GHOST_MODULE_MONGODB_ROOT_DIR}/examples/persistence_todo_list.cpp
		${GHOST_MODULE_MONGODB_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.h
		${GHOST_MODULE_MONGODB_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.cc)
	target_link_libraries(persistence_todo_list ghost_persistence_mongodb)

	set_property(TARGET persistence_todo_list PROPERTY FOLDER "examples")
endif()
