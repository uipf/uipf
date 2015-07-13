Usage: Console Interface
========================

Usage:

./uipf <moduleName> ...options...		run a single module.
./uipf -c example.yaml							run a processing chain from a config file.


Parameter:

	-c	--config			run a specified config file

	-m --moduleName  defines the name of the module

	-i	--input				defines an input, can be used multiple times
										format: inputName:fileName
										inputName is optional if there is only one input

	-o	--output			defines an output, can be used multiple times
										format: outputName:fileName
										outputName is optional if there is only one output
										output itself is optional, if there is only one input and one output,
										the output filename will be choosen from the input name in this case.

	-p	--param				defines a parameter, format:  name:value


Concrete example:

./uipf Gaussian -i ../images/ball.png -p sigma:2 -p windowSize:5
