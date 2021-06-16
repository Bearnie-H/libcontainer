#	This file is specifically different than the other component Makefiles,
#	to ensure the version.c file is ALWAYS recompiled, to keep the version
#	and compile timestamps accurate.

.FORCE:

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

#   This file contains the necessary basic rules for compiling source files into
#   object files. This does nothing when run by itself, and is meant to be used
#   in conjuction with the DefaultComponent.mk file. These two work together
#   to allow compilation of all source files in a directory into object files
#   in the project-global build tree.

#   The compiler is always passed the -MMD and -MP options.
#       -MMD builds the Makefile rules to ensure the source file depends
#           user-specified headers it includes.
#       -MP puts a dummy target to address internal Makefile errors in the event
#           a header is removed but the Makefile isn't changed.
#   These flags essentially ensure the dependency tree is auto-generated
#   by the compiler, and it knows best what files depend on what other files.
#
#   These options are not included in the main Makefile compiler flags variable
#   as they should never be removed or modified.

-include $(RELDEPS)
-include $(DBGDEPS)
-include $(DBGRDEPS)
-include $(TESTDEPS)

$(TESTBUILDDIR)/%.o : %.c .FORCE
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<" $(QUIETMODE)
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(CC) -c $(TESTFLAGS) -o $@ $< -MMD -MP

$(DBGBUILDDIR)/%.o : %.c .FORCE
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<" $(QUIETMODE)
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(CC) -c $(DBGFLAGS) -o $@ $< -MMD -MP

$(DBGRBUILDDIR)/%.o : %.c .FORCE
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<" $(QUIETMODE)
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(CC) -c $(DBGRFLAGS) -o $@ $< -MMD -MP

$(RELBUILDDIR)/%.o : %.c .FORCE
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<" $(QUIETMODE)
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(CC) -c $(RELFLAGS) -o $@ $< -MMD -MP
