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
#include <google/protobuf/util/json_util.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/model/replace_one.hpp>

using bsoncxx::builder::basic::kvp;
using namespace ghost::internal;

DataCollectionMongoDB::DataCollectionMongoDB(const std::shared_ptr<mongocxx::pool>& pool, const std::string& dbname,
					     const std::string& name)
    : _pool(pool), _dbname(dbname), _name(name)
{
}

bool DataCollectionMongoDB::remove(const std::string& id)
{
	if (id.empty()) return false;

	// Get a client from the pool and find the collection
	auto client = _pool->acquire();
	auto collection = (*client)[_dbname][_name];

	// Prepare the filter for the delete operation
	auto filter = bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid(id)
							   << bsoncxx::builder::stream::finalize;

	auto mongoResponse = collection.delete_one(filter.view());
	return mongoResponse && mongoResponse->deleted_count() > 0;
}

const std::string& DataCollectionMongoDB::getName() const
{
	return _name;
}

size_t DataCollectionMongoDB::size() const
{
	// Get a client from the pool and find the collection
	auto client = _pool->acquire();
	auto collection = (*client)[_dbname][_name];

	return collection.count({});
}

std::map<std::string, std::shared_ptr<google::protobuf::Message>> DataCollectionMongoDB::fetch(
    const std::function<std::shared_ptr<google::protobuf::Message>()>& messageFactory, std::list<std::string> idFilter)
{
	std::map<std::string, std::shared_ptr<google::protobuf::Message>> result;

	// Get a client from the pool and find the collection
	auto client = _pool->acquire();
	auto collection = (*client)[_dbname][_name];

	auto message = messageFactory();

	// Prepare the filter for the find operation
	auto filter = bsoncxx::builder::stream::document{};
	filter << "g#type" << message->GetTypeName();
	if (!idFilter.empty())
	{
		auto bsonFilter = filter << "_id" << bsoncxx::builder::stream::open_document;
		auto bsonArray = bsonFilter << "$in" << bsoncxx::builder::stream::open_array;
		for (const auto& id : idFilter)
		{
			if (id.empty()) continue;
			bsonArray << bsoncxx::oid(id);
		}
		bsonArray << bsoncxx::builder::stream::close_array;
		bsonFilter << bsoncxx::builder::stream::close_document;
	}
	auto finalized = filter << bsoncxx::builder::stream::finalize;

	// Execute the operation
	mongocxx::cursor cursor = collection.find(finalized.view());

	// Loop over the results and populate the result map
	for (auto doc : cursor)
	{
		google::protobuf::util::JsonParseOptions opt;
		opt.ignore_unknown_fields = true;
		auto status = google::protobuf::util::JsonStringToMessage(bsoncxx::to_json(doc), message.get(), opt);
		if (!status.ok()) continue;

		result[doc["_id"].get_oid().value.to_string()] = message;
		message = messageFactory();
	}

	return result;
}

std::string DataCollectionMongoDB::push(const google::protobuf::Message& data, const std::string& id)
{
	// Get a client from the pool and find the collection
	auto client = _pool->acquire();
	auto collection = (*client)[_dbname][_name];

	bsoncxx::oid oid;
	if (id.empty())
		oid = bsoncxx::oid();
	else
		oid = bsoncxx::oid(id);

	// Prepare the filter for the upsert operation. If id is empty, nothing will match and the data will be inserted
	auto filter = bsoncxx::builder::stream::document{} << "_id" << oid << bsoncxx::builder::stream::finalize;

	// The "replacement" part (or insert) is the Protobuf message concatenated to its type name
	auto builder = bsoncxx::builder::stream::document{};
	builder << "g#type" << data.GetTypeName();

	std::string json;
	auto jsonConversion = google::protobuf::util::MessageToJsonString(data, &json);
	if (!jsonConversion.ok()) return false;
	builder << bsoncxx::builder::concatenate(bsoncxx::from_json(json));

	// Create the upsert mongo command from the filter and the builder
	auto replaceCommand = mongocxx::model::replace_one(filter.view(), builder.view());
	replaceCommand.upsert(true);

	// Execute the command
	auto mongoResponse = collection.bulk_write(mongocxx::bulk_write().append(replaceCommand));
	if (!mongoResponse || mongoResponse->upserted_count() == 0) return "";

	// Return the id of the first element of the response (there should be only one)
	return mongoResponse->upserted_ids().begin()->second.get_oid().value.to_string();
}
