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

#ifndef GHOST_INTERNAL_PERSISTENCEMONGODBEXTENSIONBUILDER_HPP
#define GHOST_INTERNAL_PERSISTENCEMONGODBEXTENSIONBUILDER_HPP

#include <ghost/persistence_mongodb/PersistenceMongoDBExtensionBuilder.hpp>
#include <list>
#include <memory>
#include <mongocxx/uri.hpp>

namespace ghost
{
namespace internal
{
class PersistenceMongoDBExtensionBuilder : public ghost::PersistenceMongoDBExtensionBuilder
{
public:
	ghost::PersistenceMongoDBExtensionBuilder& addConnection(const mongocxx::uri& uri) override;
	std::shared_ptr<ghost::ModuleExtension> build() override;

private:
	std::list<std::string> _connections;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_PERSISTENCEMONGODBEXTENSIONBUILDER_HPP
