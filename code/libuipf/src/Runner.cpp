#include "Runner.hpp"

#include "yaml-cpp/yaml.h"

#include "logging.hpp"


void uipf::Runner::run() {

	using namespace std;

	//reset StopSignal
// TODO	context_.bStopRequested_ = false;
	// get processing chain
	map<string, ProcessingStep> chain = processingChain_.getProcessingSteps();

	// TODO review starting here
	map<string, ProcessingStep> chainTmp;
	chainTmp.insert(chain.begin(), chain.end());

	// contains the names of the processing steps in the correct order
	vector<string> sortedChain;

	// iterate over all processing steps and order them
	while(!chainTmp.empty()){
		map<string, ProcessingStep>::iterator itProSt = chainTmp.begin();
		while(itProSt!=chainTmp.end()) {

			// add all modules without any dependencies
			if(itProSt->second.inputs.size() == 0){
				sortedChain.push_back(itProSt->first);
				// delete and set pointer to next element
				itProSt = chainTmp.erase(itProSt);
			} else {
				// go through dependencies, and add only the modules, where module
				// on which they depend have already been added
				map<string, pair<string,string> >::iterator it = itProSt->second.inputs.begin();

				int i = 1;
				for (; it!=itProSt->second.inputs.end(); ++it) {
					// skip empty references (unset optional input)
					if (it->second.first.empty()) {
						continue;
					}
					if (find(sortedChain.begin(), sortedChain.end(), it->second.first) != sortedChain.end()){
						i *=1;
					} else{
						i *=0;
					}
				}
				if (i == 1){
					sortedChain.push_back(itProSt->first);
					// delete and set pointer to next element
					itProSt = chainTmp.erase(itProSt);
				} else {
					// try next element
					++itProSt;
				}
			}
		}
	}



	// contains the outputs of the processing steps
	map<string, map<string, Data::ptr>* > stepsOutputs;

	UIPF_LOG_INFO( "Starting processing chain." );

// TODO	GUIEventDispatcher::instance()->clearSelectionInGraphView();

	// iterate over the sortedChain and run the modules in the order given by the chain
	for (unsigned int i=0; i<sortedChain.size(); i++){

		ProcessingStep proSt = chain[sortedChain[i]];

		// TODO	GUIEventDispatcher::instance()->triggerSelectSingleNodeInGraphView(proSt.name,gui::CURRENT,false);

		// load the module
		ModuleInterface* module;
		string moduleName = proSt.module;
		if (moduleLoader_.hasModule(moduleName)) {
			module = moduleLoader_.getModuleInstance(moduleName);
			// TODO	context_.processingStepName_ = proSt.name;
			// TODO	module->setContext(&context_);

		} else {
			UIPF_LOG_ERROR( "Module '", moduleName, "' could not be found." );
			break;
		}

		// prepare an empty map of outputs that will be filled by the module
		map<string, Data::ptr >* outputs = new map<string, Data::ptr>();
		// inputs are references to the data pointer to allow sending one output to multiple steps
		map<string, Data::ptr& > inputs;

		// fill the inputs of the current processing step by taking it from the stored outputs
		map<string, pair<string, string> >::iterator it = proSt.inputs.begin();
		for (; it!=proSt.inputs.end(); ++it) {
			// skip empty references (unset optional input)
			if (it->second.first.empty()) {
				continue;
			}
			map<string, Data::ptr>* out = stepsOutputs[it->second.first];
			Data::ptr& pt = out->find(it->second.second)->second;
			inputs.insert(pair<string, Data::ptr&>(it->first, pt));
		}

		UIPF_LOG_INFO( "Running step '", proSt.name, "'..." );

		try {

// TODO
//			DataManager dataMnrg(inputs, proSt.params, *outputs);
//			module->run(dataMnrg);

		} catch (const ErrorException& e) {
			// TODO GUIEventDispatcher::instance()->triggerSelectSingleNodeInGraphView(proSt.name,gui::ERROR,false);
			UIPF_LOG_ERROR( string("Error: ") + e.what() );
			break;
		} catch (const InvalidConfigException& e) {
			// TODO GUIEventDispatcher::instance()->triggerSelectSingleNodeInGraphView(proSt.name,gui::ERROR,false);
			UIPF_LOG_ERROR( string("Invalid config: ") + e.what() );
			break;
		} catch (const std::exception& e) {
			// TODO GUIEventDispatcher::instance()->triggerSelectSingleNodeInGraphView(proSt.name,gui::ERROR,false);
			UIPF_LOG_ERROR( string("Error: module threw exception: ") + e.what() );
			break;
		}

		// update the progress bar in the GUI
		// TODO GUIEventDispatcher::instance()->triggerReportProgress(static_cast<float>(i+1)/static_cast<float>(sortedChain.size())*100.0f);

		UIPF_LOG_INFO( "Done with step '", proSt.name, "'." );

		// TODO GUIEventDispatcher::instance()->triggerSelectSingleNodeInGraphView(proSt.name,gui::GOOD,false);

		// check if stop button was pressed
// TODO
//		if (context_.bStopRequested_ )
//		{
//			LOG_I("processing stopped");
//			break;
//		}
		// fill the outputs of the current processing step
		stepsOutputs.insert(pair<string, map<string, Data::ptr>* > (proSt.name, outputs));

		// TODO delete module, check for side effects with the data pointers first

		// free some outputs that are not needed anymore
		map<string, map<string, Data::ptr>* >::iterator osit = stepsOutputs.begin();
		for (; osit!=stepsOutputs.end(); ++osit) {

			string outputStep = osit->first;

			for (auto oit = osit->second->begin(); oit!=osit->second->end(); ) {

				string outputName = oit->first;

				// iterate over the future steps to see if this output is requested
				bool requested = false;
				for (unsigned int s = i + 1; s<sortedChain.size() && !requested; s++){

					ProcessingStep fstep = chain[sortedChain[s]];
					for (auto iit=fstep.inputs.cbegin(); iit != fstep.inputs.end(); ++iit) {
						if (outputStep.compare(iit->second.first) == 0 && outputName.compare(iit->second.second) == 0) {
							requested = true;
							break;
						}
					}
				}
				if (!requested) {
					// output is not requested in any further step, delete it
					UIPF_LOG_INFO("deleted ", outputStep, ".", outputName);
					oit = osit->second->erase(oit);
				} else {
					++oit;
				}
			}
		}
	}

	// delete the ouput map
	map<string, map<string, Data::ptr>* >::iterator it = stepsOutputs.begin();
	for (; it!=stepsOutputs.end(); ++it) {
		delete it->second;
	}

	UIPF_LOG_INFO( "Finished processing chain." );
}

/*
 * TODO
std::vector<std::string> ModuleManager::loadPluginPathConfig()
{
	// search path for uipf config files
	vector<std::string> configPath;
	// search in current working directory
	configPath.push_back("./modules.yaml");
	// search in /etc
	configPath.push_back("/etc/uipf/modules.yaml");
	// search in home directory of the user
	configPath.push_back("~/.uipf-modules.yaml");

	vector<std::string> pluginPaths;
	// also add two default search paths
	pluginPaths.push_back("/usr/lib/uipf");
	pluginPaths.push_back("/usr/local/lib/uipf");

	// search for module path configuration files
	for(auto cp = configPath.cbegin(); cp != configPath.end(); ++cp) {

		try
		{
			// try to load yaml file
			YAML::Node config = YAML::LoadFile(*cp);

			// yaml file must be a sequence of module paths to search for
			if (!config.IsSequence())
				continue;

			// create a ProcessingStep object from each config element
			YAML::const_iterator it = config.begin();
			for ( ; it != config.end(); ++it) {
				pluginPaths.push_back(it->as<string>());
			}
		}
		catch(...)
		{
		}

	}

	return pluginPaths;
}
*/
