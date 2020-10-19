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

#include "PersistenceMongoDBExtension.hpp"
#include "DatabaseMongoDB.hpp"

using namespace ghost::internal;

bool PersistenceMongoDBExtension::start()
{
	for (auto& connection : _connections)
		connection.second = std::make_shared<mongocxx::pool>(mongocxx::uri{connection.first});
	return true;
}

void PersistenceMongoDBExtension::stop()
{
}

std::string PersistenceMongoDBExtension::getName() const
{
	return "PersistenceMongoDBExtension";
}

void PersistenceMongoDBExtension::addConnection(const mongocxx::uri& uri)
{
	_connections[uri.to_string()] = nullptr;
}

std::shared_ptr<ghost::DatabaseMongoDB> PersistenceMongoDBExtension::openDatabase(const std::string& name,
										  const mongocxx::uri& uri)
{
	// URI not registered, return nullptr
	if (_connections.find(uri.to_string()) == _connections.end()) return nullptr;

	// Pool not started, start it
	if (!_connections.at(uri.to_string())) _connections.at(uri.to_string()) = std::make_shared<mongocxx::pool>(uri);

	auto database = std::make_shared<DatabaseMongoDB>(_connections.at(uri.to_string()), name);
	database->open();
	return database;
}

std::list<std::string> PersistenceMongoDBExtension::getDatabaseNames() const
{
	return {};
}
