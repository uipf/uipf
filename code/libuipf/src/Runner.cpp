#include "Runner.hpp"

#include "data.hpp"
#include "data/list.hpp"
#include "yaml-cpp/yaml.h"

#include "logging.hpp"
#include "util.hpp"


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
				map<string, StepInput >::iterator it = itProSt->second.inputs.begin();

				int i = 1;
				for (; it!=itProSt->second.inputs.end(); ++it) {
					// skip empty references (unset optional input)
					if (it->second.sourceStep.empty()) {
						continue;
					}
					if (find(sortedChain.begin(), sortedChain.end(), it->second.sourceStep) != sortedChain.end()){
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
	map<string, map<string, Data::ptr> > stepsOutputs;

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


		try {

	//		// prepare an empty map of outputs that will be filled by the module
	//		map<string, Data::ptr >* outputs = new map<string, Data::ptr>();
	//		// inputs are references to the data pointer to allow sending one output to multiple steps
	//		map<string, Data::ptr& > inputs;

			std::string mapInput;
			data::List::ptr mapData;

			// fill the inputs of the current processing step by taking it from the stored outputs
			uipf_foreach(input, proSt.inputs) {
				// skip empty references (unset optional input)
				if (input->second.sourceStep.empty()) {
					continue;
				}

				map<string, Data::ptr> moduleOutputs = stepsOutputs[input->second.sourceStep];
				uipf_cforeach(debug, moduleOutputs) {
					UIPF_LOG_WARNING(debug->first, ": ", debug->second.use_count());
				}
				auto outputElement = moduleOutputs.find(input->second.outputName);
				if (outputElement == moduleOutputs.end()) {
					throw ErrorException(string("Output '") + input->second.outputName + string("' requested by step '") + proSt.name + string("' for input '") + input->second.sourceStep + string("' does not exist."));
				}

				if (input->second.map) {
					if (!mapInput.empty()) {
						throw InvalidConfigException("Multiple map() calls in one step detected, this is currently not supported.");
					}
					// map() input gets special handling
					mapInput = input->first;
					mapData = std::dynamic_pointer_cast<data::List>(outputElement->second);
				} else {
					module->input_.insert(pair<string, Data::ptr>(input->first, outputElement->second));
				}
			}

			// set module params
			module->params_ = proSt.params;

			if (mapInput.empty()) {

				UIPF_LOG_INFO("Running step '", proSt.name, "'...");
				module->run();
				UIPF_LOG_INFO("Done with step '", proSt.name, "'.");

				// fill the outputs of the current processing step
				stepsOutputs.insert(pair<string, map<string, Data::ptr> > (proSt.name, module->output_));

			} else {

				map< string, data::List::ptr > mapOutputs;

				UIPF_LOG_INFO("Running step '", proSt.name, "' in map() mode on ", mapData->getContent().size(), " items...");
				uipf_cforeach(mapItem, mapData->getContent()) {

					module->input_.insert(pair<string, Data::ptr>(mapInput, *mapItem));
					module->run();
					module->input_.erase(mapInput);

					for(auto mapOutput = module->output_.begin(); mapOutput != module->output_.end(); /*no ++ here*/) {

						auto pos = mapOutputs.find(mapOutput->first);
						if (pos == mapOutputs.end()) {
							auto ins = mapOutputs.insert(pair<string, data::List::ptr>(mapOutput->first, data::List::ptr(new data::List())));
							pos = ins.first;
						}

						pos->second->getContent().push_back(mapOutput->second);
						mapOutput = module->output_.erase(mapOutput);
					}

				}
				UIPF_LOG_INFO("Done with step '", proSt.name, "'.");

				// fill the outputs of the current processing step
//				stepsOutputs.insert(pair<string, map<string, data::List::ptr> > (proSt.name, mapOutputs));
				stepsOutputs.insert(pair<string, map<string, Data::ptr> > (proSt.name, map<string, Data::ptr>()));
				uipf_foreach(output, mapOutputs) {
					stepsOutputs[proSt.name].insert(pair<string, Data::ptr>( output->first, output->second ));
				}

			}

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

		// TODO GUIEventDispatcher::instance()->triggerSelectSingleNodeInGraphView(proSt.name,gui::GOOD,false);

		// check if stop button was pressed
// TODO
//		if (context_.bStopRequested_ )
//		{
//			LOG_I("processing stopped");
//			break;
//		}

		// TODO delete module, check for side effects with the data pointers first

		// free some outputs that are not needed anymore
		uipf_foreach(osit, stepsOutputs) {

			string outputStep = osit->first;

			for (auto oit = osit->second.begin(); oit!=osit->second.end(); /* no ++ here */) {

				string outputName = oit->first;

				// iterate over the future steps to see if this output is requested
				bool requested = false;
				for (unsigned int s = i + 1; s<sortedChain.size() && !requested; s++){

					ProcessingStep fstep = chain[sortedChain[s]];
					uipf_cforeach(iit, fstep.inputs) {
						if (outputStep.compare(iit->second.sourceStep) == 0 && outputName.compare(iit->second.outputName) == 0) {
							requested = true;
							UIPF_LOG_DEBUG(outputStep, ".", outputName, " is requested.");
							break;
						}
					}
				}
				if (!requested) {
					// output is not requested in any further step, delete it
					UIPF_LOG_INFO("deleted ", outputStep, ".", outputName);
					oit = osit->second.erase(oit);
				} else {
					++oit;
				}
			}
		}
	}

	// delete the ouput map
//	uipf_foreach(it, stepsOutputs) {
//		delete it->second;
//	}

	UIPF_LOG_INFO( "Finished processing chain." );
}
