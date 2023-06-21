SRC = mapped_file.cpp stringprintf.cpp strings.cpp
OBJS := $(patsubst %.cpp,$(OBJ)/base/%.o,$(SRC))

all: $(LIB_OUT)/libbase$(LIB_EXT)

$(SLIB)/libbase.dll: $(LIB)/libbase.a
	@echo -e "  LDXX\t    `basename $@`"
	@$(LDXX) -shared -o $(SLIB)/svbbase.dll \
		-Wl,--export-all-symbols \
		-Wl,--enable-auto-import \
		-Wl,--whole-archive $(LIB)/libbase.a \
		-Wl,--no-whole-archive $(LIBS) $(DLL_RES)

$(LIB)/libbase.a: $(OBJS)
	@$(MKDIR) -p "`dirname $@`"
	@echo -e "  AR\t    `basename $@`"
	@$(AR) $(ARFLAGS) $@ $^

$(OBJ)/base/%.o: %.cpp
	@$(MKDIR) -p `dirname $@`
	@echo -e "  CXX\t    `basename $@`"
	@$(CXX) -static $(CXXFLAGS) -Iinclude -c $< -o $@
