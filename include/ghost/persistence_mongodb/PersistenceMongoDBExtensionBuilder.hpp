/*
 * Copyright 2020 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GHOST_PERSISTENCEMONGODBEXTENSIONBUILDER_HPP
#define GHOST_PERSISTENCEMONGODBEXTENSIONBUILDER_HPP

#include <ghost/module/ModuleExtensionBuilder.hpp>
#include <mongocxx/uri.hpp>

namespace ghost
{
class PersistenceMongoDBExtensionBuilder : public ghost::ModuleExtensionBuilder
{
public:
	/**
	 *	Registers a mongoDB URI to set up a client pool and optimize the resource usage
	 *	when accessing the database.
	 *	@param uri a mongoDB URI object containing the connection parameters to a MongoDB
	 *		node.
	 */
	virtual void addConnection(const mongocxx::uri& uri) = 0;
};
} // namespace ghost

#endif // GHOST_PERSISTENCEMONGODBEXTENSIONBUILDER_HPP
