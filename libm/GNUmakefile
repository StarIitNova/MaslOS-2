include ../defaults.mk
OUTPUT = FINAL_libm.o
OBJDIR = ../objects/libm

LDFLAGS = $(LD_RELO_FLAGS)
NASMFLAGS = $(BASE_NASM_FLAGS)
CFLAGS    = $(C_RELO_FLAGS)
CPPFLAGS  = $(CPP_RELO_FLAGS)

CFILES    = $(shell find . -type f -name '*.c')
CPPFILES  = $(shell find . -type f -name '*.cpp')
NASMFILES = $(shell find . -type f -name '*.asm')

OBJ       = $(patsubst %.c,   $(OBJDIR)/%.o,               $(CFILES)) $(patsubst %.cpp,   $(OBJDIR)/%.o,               $(CPPFILES)) $(patsubst %.asm,   $(OBJDIR)/%_asm.o,               $(NASMFILES))
OBJ1      = $(addsuffix .user_module, $(OBJ))
OBJ2      = $(addsuffix .kernel_src, $(OBJ))
OBJ3      = $(addsuffix .kernel_module, $(OBJ))


.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(LD)  $(OBJ1) $(LDFLAGS) -o $(OBJDIR)/$@.user_module
	@$(LD) $(OBJ2) $(LDFLAGS) -o $(OBJDIR)/$@.kernel_src
	@$(LD) $(OBJ3) $(LDFLAGS) -o $(OBJDIR)/$@.kernel_module
	@echo foo > $@

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC)  $(CPPFLAGS) -c $^ -o $@.kernel_src -D _KERNEL_SRC
	@$(CC) $(CPPFLAGS) -c $^ -o $@.kernel_module -D _KERNEL_MODULE
	@$(CC) $(CPPFLAGS) -c $^ -o $@.user_module -D _USER_MODULE
	@echo foo > $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC)  $(CFLAGS) -c $^ -o $@.kernel_src -D _KERNEL_SRC
	@$(CC) $(CFLAGS) -c $^ -o $@.kernel_module -D _KERNEL_MODULE
	@$(CC) $(CFLAGS) -c $^ -o $@.user_module -D _USER_MODULE
	@echo foo > $@

$(OBJDIR)/%_asm.o: %.asm
	@mkdir -p $(@D)
	@nasm $(NASMFLAGS) $< -o $@
	@echo foo > $@


.PHONY: clean
clean:
	@rm -f $(OBJ) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ:.o=.d)

	@rm -rf $(OUTPUT) || true
	@rm -rf $(OUTPUT).kernel_src
	@rm -rf $(OUTPUT).kernel_module
	@rm -rf $(OUTPUT).user_module

-include $(OBJ:.o=.d)
