TARGET:=simple_csg

SRCS:=$(foreach cpp,$(wildcard ../csg/*.cpp),$(notdir $(cpp)))
SRCS+=$(foreach cpp,$(wildcard ../gui/*.cpp),$(notdir $(cpp)))
SRCS+=$(foreach cpp,$(wildcard ../math/*.cpp),$(notdir $(cpp)))

OBJS:=$(SRCS:.cpp=.o)
DEPS:=$(SRCS:.cpp=.d)

LINKED_LIBS:=glew glfw3 gl

CXX:=g++
LD:=g++
CPPFLAGS:=$(shell pkg-config --cflags $(LINKED_LIBS)) -I..
CXXFLAGS:=-Wall -Werror -g -std=c++11
LDFLAGS:=$(shell pkg-config --libs $(LINKED_LIBS)) -lm

vpath %.cpp ../csg
vpath %.cpp ../gui
vpath %.cpp ../math

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

ifneq ($(MAKECMDGOALS),clean)
include $(DEPS)

%.d: %.cpp
	$(CC) $(CPPFLAGS) -M -MG $< | sed 's|\($*.o\):|\1 $@: Makefile |g' > $@
endif

.PHONY: clean
clean:
	rm -f *.o *.d $(TARGET)
