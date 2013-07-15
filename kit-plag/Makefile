# compiler flags
CXX=g++
CXXSTD=-std=c++11
CXXFLAGS=$(CXXSTD) -O2 -Wall

# linker flags
LD=g++
LDFLAGS=-lboost_system -lboost_filesystem -lboost_program_options -lboost_regex

# directories
BINDIR=bin
SRCDIR=src

# input, output and intermediary files
BIN=kit-plag
SRCS:=$(wildcard $(SRCDIR)/*.cpp)
OBJS:=$(subst $(SRCDIR),$(BINDIR),$(SRCS:.cpp=.o))
DEPS:=$(OBJS:.o=.d)

# rules
all: depend $(BINDIR)/$(BIN)
	cp $(BINDIR)/$(BIN) .

$(BINDIR)/$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# make dependencies
depend: $(DEPS)
$(BINDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXSTD) -E -MM -MT"$(@:.d=.o)" $< > $@

# make objects
ifneq ($(MAKECMDGOALS),clean)
sinclude $(DEPS)
endif
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(BINDIR)/*.o $(BINDIR)/*.d $(BINDIR)/$(BIN)
