/*
 * Copyright 2019 Mathieu Nassar
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

#include <bsoncxx/exception/exception.hpp>
#include <ghost/module/GhostLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <ghost/persistence/DataManager.hpp>
#include <ghost/persistence_mongodb/DatabaseMongoDB.hpp>
#include <ghost/persistence_mongodb/PersistenceMongoDBExtension.hpp>
#include <ghost/persistence_mongodb/PersistenceMongoDBExtensionBuilder.hpp>

#include "protobuf/persistency_todo_list.pb.h"

/***************************
	TRY IT: Run this program and add or remove your own TODOs.
***************************/

class TodoListModule
{
public:
	// This method will initialize the ghost::DataManager, which is used by this example
	// to save the TODO elements between program executions.
	// The structure of the data file used in this example is simple: it will contain a single
	// data file ("Todolist.dat"), which will contain a list of Google Protobuf messages defined
	// in the "examples/protobuf" folder of this repository (the example uses the "Todo" message).
	bool initialize(const ghost::Module& module)
	{
		// First, create an instance of the ghost::SaveManager, that uses "." as its root path to
		// save the data.
		_dataManager = ghost::DataManager::create();

		auto database = module.getExtension<ghost::PersistenceMongoDBExtension>()->openDatabase(
		    TODO_LIST_NAME, {"mongodb://localhost:27017"});
		_dataManager->addDatabase(database, TODO_LIST_NAME);

		_todoList = _dataManager->addCollection(TODO_LIST_NAME, TODO_LIST_NAME);

		return true;
	}

	// The execution of the module reads the parameters provided to the program call.
	// It can list, add or remove parameters.
	bool run(const ghost::Module& module)
	{
		if (module.getProgramOptions().hasParameter("__0"))
		{
			if (module.getProgramOptions().getParameter<std::string>("__0") == "list")
				listTodo(module);
			else if (module.getProgramOptions().getParameter<std::string>("__0") == "add")
				addTodo(module);
			else if (module.getProgramOptions().getParameter<std::string>("__0") == "remove")
				removeTodo(module);
			else
			{
				GHOST_INFO(module.getLogger()) << "Usage: program [list | add | remove]";
			}
		}
		else
		{
			GHOST_INFO(module.getLogger()) << "Usage: program [list | add | remove]";
		}

		return false;
	}

	void listTodo(const ghost::Module& module)
	{
		if (_todoList->size() == 0)
		{
			GHOST_INFO(module.getLogger()) << "There are no TODOs in the list";
		}
		else
		{
			GHOST_INFO(module.getLogger()) << "Current TODOs: ";
			_todoList->get_if<ghost::examples::protobuf::Todo>(
			    [&](const ghost::examples::protobuf::Todo& todo, const std::string& id) {
				    GHOST_INFO(module.getLogger()) << "TODO #" << id << ": " << todo.title();
				    return true;
			    });
		}
	}

	void addTodo(const ghost::Module& module)
	{
		GHOST_INFO(module.getLogger()) << "Enter a title for the new TODO: ";
		auto title = module.getConsole()->getLine();
		auto todo = ghost::examples::protobuf::Todo::default_instance();
		todo.set_title(title);
		// To add a todo, this function simply requests a title from the user, creates a "Todo"
		// message and adds it to the ghost::SaveData object.
		_todoList->put(todo);
		GHOST_INFO(module.getLogger()) << "Added new TODO.";
	}

	void removeTodo(const ghost::Module& module)
	{
		GHOST_INFO(module.getLogger()) << "Enter the ID (#) of the TODO to remove: ";
		auto title = module.getConsole()->getLine();
		// Similarly to the "addTodo" method, this method requests the ID of a TODO to remove
		// and removes it from the ghost::SaveData object.
		_todoList->remove(title);
		GHOST_INFO(module.getLogger()) << "Removed TODO";
	}

private:
	std::shared_ptr<ghost::DataManager> _dataManager;
	std::shared_ptr<ghost::DataCollection> _todoList;

	const std::string TODO_LIST_NAME = "TodoList";
};

int main(int argc, char** argv)
{
	TodoListModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	// This line will provide the intialization method.
	builder->setInitializeBehavior(std::bind(&TodoListModule::initialize, &myModule, std::placeholders::_1));
	// This line will provide the run method, which will be called cyclically.
	builder->setRunningBehavior(std::bind(&TodoListModule::run, &myModule, std::placeholders::_1));
	// Since we want to use the console logger, we provide one with a console to the builder.
	auto console = builder->setConsole();
	builder->setLogger(ghost::GhostLogger::create(console));
	// Parse the program options to determine what to do:
	builder->setProgramOptions(argc, argv);

	auto mongoBuilder = ghost::PersistenceMongoDBExtensionBuilder::create();
	mongoBuilder->addConnection({"mongodb://localhost:27017"});
	builder->addExtensionBuilder(mongoBuilder);

	// The following line creates the module with all the parameters, and names it "myModuleInstance0".
	std::shared_ptr<ghost::Module> module = builder->build();

	// If the build process is successful, we can start the module.
	// If it were not successful, we would have nullptr here.
	if (module) module->start();

	// Start blocks until the module ends.
	return 0;
}
