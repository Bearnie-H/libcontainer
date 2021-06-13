
#   This file drives the compilation of a "Component" of a larger project.
#   This will find the source files of the current directory, split out
#   any _test.c files from the release and debug builds
#   and compile everything into the project-global build tree.

SRCS := $(wildcard *.c)
DEPS := $(patsubst %.c,%.d,$(SRCS))


#   Filter out any files of the form: *_test.c from the
#   release and debug targets. No point in compiling files that are intended to
#   be excluded from useable output builds.

RELOBJS = $(patsubst %.c,$(RELBUILDDIR)/%.o,$(filter-out %test.c, $(SRCS)))
DBGOBJS = $(patsubst %.c,$(DBGBUILDDIR)/%.o,$(filter-out %test.c, $(SRCS)))
DBGROBJS = $(patsubst %.c,$(DBGRBUILDDIR)/%.o,$(SRCS))
TESTOBJS = $(patsubst %.c,$(TESTBUILDDIR)/%.o,$(SRCS))

RELDEPS = $(addprefix $(RELBUILDDIR)/,$(filter-out %test.d, $(DEPS)))
DBGDEPS = $(addprefix $(DBGBUILDDIR)/,$(filter-out %test.d, $(DEPS)))
DBGRDEPS = $(addprefix $(DBGRBUILDDIR)/,$(DEPS))
TESTDEPS = $(addprefix $(TESTBUILDDIR)/,$(DEPS))

.PHONY: all install uninstall test debug debugger release prep clean remake clean-hard

#   Top level general targets for a component.
#   Components should block on the "test" target
#   in case it fails. No sense in building release copies if the tests fail.
all: prep
	@$(MAKE) test
	@$(MAKE) debug debugger release

install: release
	$(error "Install target undefined!")

uninstall:
	$(error "Uninstall target undefined!")

prep:

#   Only remove the dependency-tree files on a hard clean.
clean-hard: clean
	@printf "%-8s %s\n" "(RM)" "$(RELDEPS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(RELDEPS)
	@printf "%-8s %s\n" "(RM)" "$(DBGDEPS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(DBGDEPS)
	@printf "%-8s %s\n" "(RM)" "$(DBGRDEPS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(DBGRDEPS)
	@printf "%-8s %s\n" "(RM)" "$(TESTDEPS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(TESTDEPS)

#   On a regular clean, just clean up the object files for this component.
clean:
	@printf "%-8s %s\n" "(RM)" "$(RELOBJS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(RELOBJS)
	@printf "%-8s %s\n" "(RM)" "$(DBGOBJS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(DBGOBJS)
	@printf "%-8s %s\n" "(RM)" "$(DBGROBJS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(DBGROBJS)
	@printf "%-8s %s\n" "(RM)" "$(TESTOBJS:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(RM) $(TESTOBJS)

remake:
	@$(MAKE) clean
	@$(MAKE) all

test: $(TESTOBJS)

debug: $(DBGOBJS)

debugger: $(DBGROBJS)

release: $(RELOBJS)

#   Include the general file for building .c files into object files,
#   as well as including all necessary dependency tree information the compiler emits.
include $(ROOTDIR)/Objects.mk

