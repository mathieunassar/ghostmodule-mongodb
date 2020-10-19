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

#include "DataCollectionMongoDB.hpp"

using namespace ghost::internal;

DataCollectionMongoDB::DataCollectionMongoDB(const std::shared_ptr<mongocxx::pool>& pool, const std::string& dbname,
					     const std::string& name)
    : _pool(pool), _dbname(dbname), _name(name)
{
}

bool DataCollectionMongoDB::remove(size_t index)
{
	return false;
}

const std::string& DataCollectionMongoDB::getName() const
{
	return _name;
}

size_t DataCollectionMongoDB::size() const
{
	return 0;
}

std::map<size_t, std::shared_ptr<google::protobuf::Message>> DataCollectionMongoDB::fetch(
    const std::function<std::shared_ptr<google::protobuf::Message>()>& messageFactory, std::list<size_t> idFilter)
{
	return {};
}

bool DataCollectionMongoDB::push(const google::protobuf::Message& data, size_t id)
{
	return false;
}
