
CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -g

LDFLAGS =

SRCDIR = src

TARGET = dendron

SRCS = $(wildcard $(SRCDIR)/*.cpp)

OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "Build finished. Executable is '$(TARGET)'"

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/tree.h $(SRCDIR)/main.h
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) $(OBJS)

