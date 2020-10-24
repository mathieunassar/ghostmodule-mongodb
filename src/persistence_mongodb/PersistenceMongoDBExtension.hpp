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

#ifndef GHOST_INTERNAL_PERSISTENCEMONGODBEXTENSION_HPP
#define GHOST_INTERNAL_PERSISTENCEMONGODBEXTENSION_HPP

#include <ghost/persistence_mongodb/PersistenceMongoDBExtension.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <string>
#include <list>
#include <memory>
#include <map>

namespace ghost
{
namespace internal
{
class PersistenceMongoDBExtension : public ghost::PersistenceMongoDBExtension
{
public:
	// From ghost::ModuleExtension
	bool start() override;
	void stop() override;
	std::string getName() const override;

	// From ghost::PersistenceMongoDBExtension
	void addConnection(const mongocxx::uri& uri) override;
	std::shared_ptr<ghost::DatabaseMongoDB> openDatabase(const std::string& name,
							     const mongocxx::uri& uri) override;
	std::list<std::string> getDatabaseNames() const override;

private:
	mongocxx::instance _instance;
	std::map<std::string, std::shared_ptr<mongocxx::pool>> _connections;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_PERSISTENCEMONGODBEXTENSION_HPP
