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

#include "DatabaseMongoDB.hpp"
#include "DataCollectionMongoDB.hpp"

using namespace ghost::internal;

DatabaseMongoDB::DatabaseMongoDB(const std::shared_ptr<mongocxx::pool>& pool, const std::string& name)
    : _pool(pool), _name(name)
{
}

bool DatabaseMongoDB::open()
{
	return true;
}

void DatabaseMongoDB::close()
{
}

std::shared_ptr<ghost::DataCollection> DatabaseMongoDB::addCollection(const std::string& name)
{
	return std::make_shared<DataCollectionMongoDB>(_pool, _name, name);
}

bool DatabaseMongoDB::removeCollection(const std::string& name)
{
	return false;
}

std::list<std::shared_ptr<ghost::DataCollection>> DatabaseMongoDB::getCollection(const std::string& name) const
{
	return {};
}
