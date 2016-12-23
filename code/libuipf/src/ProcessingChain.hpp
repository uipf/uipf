#ifndef LIBUIPF_PROCESSIN_CHAIN_HPP
#define LIBUIPF_PROCESSIN_CHAIN_HPP


#include <string>
#include <map>
#include <vector>
#include "ModuleMetaData.hpp"
#include "exceptions.hpp"

namespace uipf{

	class ProcessingStep{

	public:

		// name of this processing step in this configuration e.g. output
		std::string name;

		// name of the module that is used in this step e.g. storeImage
		std::string module;

		// a map of parameters configured for this module e.g. filename => image.png
		std::map<std::string, std::string> params;

		// inputs from other modules <inputName> => <nameOfOtherModule, outputOfOtherModule>
		std::map<std::string, std::pair<std::string, std::string> > inputs;

	};

	class ProcessingChain{

	public:
		// constructor
		ProcessingChain(void){};
		// destructor
		~ProcessingChain(void){};

		// loads the processing chain from the config file
		void load(std::string);

		// stores the processing chain into a config file
		void store(std::string);

		// prints the yaml representation of the config
		void print();

		// validates the config by checking for logical errors
		// returns a set of error messages and a set of affected steps, config is valid if messages are empty
		std::pair< std::vector<std::string>, std::vector<std::string> > validate(std::map<std::string, ModuleMetaData>);

		// return processing chain  name => step
		std::map<std::string, ProcessingStep> getProcessingChain() const;

		// returns the named processing step
		ProcessingStep getProcessingStep(std::string) const;

		// returns a value indicating whether a named processing step exists
		bool hasProcessingStep(std::string name) const;


		// methods to manipulate the config:

		// adds a ProcessingStep to the chain
		void addProcessingStep(ProcessingStep);

		// removes a ProcessingStep from the chain
		void removeProcessingStep(std::string);

		// rename a ProcessingStep
		bool renameProcessingStep(std::string, std::string);

		// sets the module name for a named processing step
		void setProcessingStepModule(std::string, std::string, ModuleMetaData);

		// sets the parameters for a named processing step
		void setProcessingStepParams(std::string, std::map<std::string, std::string>);

		// sets the inputs for a named processing step
		void setProcessingStepInputs(std::string, std::map<std::string, std::pair<std::string, std::string> >);

	private:
		// chain of ProcessingSteps name => step
		std::map<std::string, ProcessingStep> chain_;

		// returns the YAML representation of the config, used for storing and printing
		std::string getYAML();

		// detect circular dependencies in the inputs
		std::vector<std::string> detectCircularDependencies();

};

} // namespace

#endif // LIBUIPF_PROCESSIN_CHAIN_HPP
