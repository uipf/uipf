
UIPF_BIN=@libuipf_BINARY_DIR@
UIPF_SRC=@libuipf_SOURCE_DIR@

CPP_FLAGS=-std=gnu++0x
INCLUDES=-I$(UIPF_SRC)/src/include
LIBS=-L$(UIPF_BIN) -fPIC -luipf-module

libGccModule.so: GccModule.cpp $(UIPF_BIN)/libuipf-module.so
	g++ $(CPP_FLAGS) -shared -o $@ $^ $(INCLUDES) $(LIBS)
