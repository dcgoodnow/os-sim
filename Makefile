IDIR = include
SDIR = src
CXX = g++
CXXFLAGS = -g -pthread -Wall -I$(IDIR)

ODIR = obj
_DEPS = OS.h MonitorLogger.h BothLogger.h FileLogger.h ProcessControlBlock.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = OS.o MonitorLogger.o BothLogger.o FileLogger.o main.o ProcessControlBlock.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	mkdir -p $(ODIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

simulator: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ simulator
	
