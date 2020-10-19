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

#ifndef GHOST_PERSISTENCEMONGODBEXTENSION_HPP
#define GHOST_PERSISTENCEMONGODBEXTENSION_HPP

#include <ghost/module/ModuleExtension.hpp>
#include <ghost/persistence_mongodb/DatabaseMongoDB.hpp>
#include <mongocxx/uri.hpp>

namespace ghost
{
class PersistenceMongoDBExtension : public ghost::ModuleExtension
{
public:
	/**
	 *	Registers a mongoDB URI to set up a client pool and optimize the resource usage
	 *	when accessing the database.
	 *	@param uri a mongoDB URI object containing the connection parameters to a MongoDB
	 *		node.
	 */
	virtual void addConnection(const mongocxx::uri& uri) = 0;

	virtual std::shared_ptr<ghost::DatabaseMongoDB> openDatabase(const std::string& name,
								    const mongocxx::uri& uri) = 0;

	virtual std::list<std::string> getDatabaseNames() const = 0;
};
} // namespace ghost

#endif // GHOST_PERSISTENCEMONGODBEXTENSION_HPP
