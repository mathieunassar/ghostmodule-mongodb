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

#include "PersistenceMongoDBExtensionBuilder.hpp"
#include "PersistenceMongoDBExtension.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::PersistenceMongoDBExtensionBuilder> ghost::PersistenceMongoDBExtensionBuilder::create()
{
	return std::make_shared<ghost::internal::PersistenceMongoDBExtensionBuilder>();
}

ghost::PersistenceMongoDBExtensionBuilder& PersistenceMongoDBExtensionBuilder::addConnection(const mongocxx::uri& uri)
{
	_connections.push_back(uri.to_string());
	return *this;
}

std::shared_ptr<ghost::ModuleExtension> PersistenceMongoDBExtensionBuilder::build()
{
	auto extension = std::make_shared<ghost::internal::PersistenceMongoDBExtension>();
	for (const auto& uri : _connections) extension->addConnection({uri});

	return extension;
}
