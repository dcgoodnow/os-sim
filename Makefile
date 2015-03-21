IDIR = ./include
CXX = g++
CXXFLAGS = -pthread -Wall

ODIR = obj
_DEPS = OS.h Logger.h MonitorLogger.h BothLogger.h FileLogger.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = OS.o Logger.o MonitorLogger.o BothLogger.o FileLogger.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

os-sim: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	
