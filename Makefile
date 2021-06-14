#	Common Makefile variables and recipes for all sub-components of the application

#   This Makefile will generally respect the best-practice that re-running following a successful
#   Make will be a no-op, with a small number of exceptions.
#
#       1)  If the "test" target is implied, this will still execute the test target.
#               This is mainly deemed acceptable, as it allows for building a larger
#               build system which relies on being able to always get the number of
#               failing tests.
#       2)  If any of the "test", "debugger", "debug", or "release" targets are implied,
#           this will relink the set of object files into the final executable.
#               This is expected, as it allows this top-level Makefile to not need
#               to know all of the object files it depends on a priori. I am not sure
#               how to implement the sub-component type logic without this.
#
#   Generally though, this will follow the best-practice, and the steps still performed
#   should be small and quick enough to not be problematic.


#   This Makefile expects the naming convention for all test source files to be of the form:
#
#       *_test.c
#
#   If this convention is not held, update the "filter-out" operation used in the release and debug builds
#   to keep the empty object files from being operated on and reporting warnings about their emptiness.
#   You will also need to update the DefaultComponent.mk file to use your test-file naming convention as well.

#   Version Settings
MAJOR_VERSION := 0
MINOR_VERSION := 6
PATCH_VERSION := 2
VERSION       := v$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION)

ifdef QUIET
QUIETMODE := 2> /dev/null 1> /dev/null
else
QUIETMODE :=
endif

export QUIETMODE

#   Standard Compiler Settings
CC              := gcc
WARNINGS        := -Wall -Wpedantic -Wextra -Wsign-conversion -Wunreachable-code -Wcast-align -Wcast-qual
WARNINGS_AS_ERRORS ?= -Werror
WARNINGS_IGNORE := -Wno-padded
CFLAGS          := -std=c99 $(WARNINGS) $(WARNINGS_IGNORE) $(WARNINGS_AS_ERRORS)
LIBTOOL         := ar -rcs
FMTTOOL         ?=$(shell which clang-format)

include Build_Tuning.mk

#   Define the flags for the various different end-result build targets
RELFLAGS  := $(CFLAGS) -O2 -DNDEBUG
DBGFLAGS  := $(CFLAGS) -g -O0 -DDEBUG
DBGRFLAGS := $(CFLAGS) -g -O0 -DDEBUG -DDEBUGGER
TESTFLAGS := $(CFLAGS) -g -O0 -DDEBUG -DDEBUGGER -DTESTING

#   Other shell command settings
RM    := rm -f
STRIP := strip -S -X
MKDIR := mkdir -p
CP    := cp -rp
LN    := ln -sf

#   Common Variables
ROOTDIR      := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SRCROOTDIR   := $(ROOTDIR)/src
BUILDROOTDIR := $(ROOTDIR)/build
OUTROOTDIR   := $(ROOTDIR)/products
LIBDIR       := $(ROOTDIR)/lib
TARGETNAME   := libcontainer
TARGET       := $(TARGETNAME)-$(VERSION)
TARGETSUFFIX := .a
TESTSUFFIX   := .out

#   Applicaton sub-components
#   Add in and subfolders required to make the full application here.
#
#   The COMPONENTS will be built into all targets, while the TESTCOMPONENTS
#    will only be built into the "test" and "debugger" targets.
#   For something like a library, the "src" should be moved to the TESTCOMPONENTS
#    to allow it to be used as driver code for running something like an internal test suite.
COMPONENTS     := src/array src/list src/hashmap src/tree/binary-tree src/stack src/string
TESTCOMPONENTS := src

#   Additional Libraries to include when building the final applications
SLIBS          := $(wildcard $(LIBDIR)/*.a)     #   Static Libraries
DLIBS          := $(wildcard $(LIBDIR)/*.so)    #   Dynamic Libraries
RELLIBS        := $(filter-out %debug %debug.a %debug.so, $(SLIBS) $(DLIBS))
ifdef USE_DEBUG_LIBS
DBGLIBS        := $(filter-out $(RELLIBS), $(SLIBS) $(DLIBS))
else
DBGLIBS        := $(RELLIBS)
endif

#   Note, in the following build settings, the *OBJS target is not fully defined here.
#   The goal is to allow all of the sub-components to build and leave their respective
#   object files in their specific build directory, and then the final target just links
#   together all of the components without needing to know before-hand what they all are.

#   Release Build Settings
RELBUILDDIR := $(BUILDROOTDIR)/release
RELOUTDIR   := $(OUTROOTDIR)/release
RELOBJS      = $(wildcard $(RELBUILDDIR)/*.o)
RELTARGET   := $(TARGET)$(TARGETSUFFIX)
RELBIN      := $(RELOUTDIR)/$(RELTARGET)

#   Debug Build Settings
DBGBUILDDIR := $(BUILDROOTDIR)/debug
DBGOUTDIR   := $(OUTROOTDIR)/debug
DBGOBJS      = $(wildcard $(DBGBUILDDIR)/*.o)
DBGTARGET   := $(TARGET)-debug$(TARGETSUFFIX)
DBGBIN      := $(DBGOUTDIR)/$(DBGTARGET)

#   Debugger Build Settings
DBGRBUILDDIR := $(BUILDROOTDIR)/debugger
DBGROUTDIR   := $(OUTROOTDIR)/debugger
DBGROBJS      = $(wildcard $(DBGRBUILDDIR)/*.o)
DBGRTARGET   := $(TARGET)-debugger$(TESTSUFFIX)
DBGRBIN      := $(DBGROUTDIR)/$(DBGRTARGET)

#   Test Build Settings
TESTBUILDDIR := $(BUILDROOTDIR)/testing
TESTOUTDIR   := $(OUTROOTDIR)/testing
TESTOBJS      = $(wildcard $(TESTBUILDDIR)/*.o)
TESTTARGET   := $(TARGET)-test$(TESTSUFFIX)
TESTBIN      := $(TESTOUTDIR)/$(TESTTARGET)

#   Install Build Settings - Define to allow building the install target
INSTALL_PATH             ?= /usr/local
TARGET_INSTALL_PATH      := $(INSTALL_PATH)/$(TARGETNAME)
TARGET_INSTALL_BINDIR    := $(TARGET_INSTALL_PATH)/lib
TARGET_INSTALL_HEADERDIR := $(TARGET_INSTALL_PATH)/include
INSTALL_BINDIR           := $(INSTALL_PATH)/lib
INSTALL_HEADERDIR        := $(INSTALL_PATH)/include
INSTALLMANDIR            ?= /usr/local/share/man/man1
INSTALLDIRS              := $(TARGET_INSTALL_BINDIR) $(TARGET_INSTALL_HEADERDIR) $(INTALL_BINDIR) $(INSTALL_HEADERDIR) $(INSTALLMANDIR)

INSTALLHEADERS   := $(wildcard $(ROOTDIR)/include/*.h)
INSTALLMANPAGES  := $(wildcard $(ROOTDIR)/man/*.1)
# Other installation targets and files go here

#   Export the specific compiler to use.
export CC FMTTOOL

#   Export the various directories so any and all sub-makes can put their objects in the correct directories.
export ROOTDIR RELBUILDDIR DBGBUILDDIR DBGRBUILDDIR TESTBUILDDIR

#   Export the compiler flags to use
export RELFLAGS DBGFLAGS DBGRFLAGS TESTFLAGS

#   Set up the .PHONY targets
.PHONY: all help install uninstall prep release test debug debugger clean clean-hard remake $(COMPONENTS) $(TESTCOMPONENTS)

#   Top level general targets
#   Ensure test completes before attempting any other targets, to ensure
#   the testing suite successfully exits.
all: prep   ##  Build and run the testing suite, then the debug, debugger, and release targets
	@$(MAKE) test
	@$(MAKE) debug debugger release

help: ## Show this help menu.
	@echo "$$(grep -hE '^\S+:.*##' $(MAKEFILE_LIST) | sed -e 's/:.*##\s*/:/' | column -c2 -t -s :)"

#   Before installing anything, make sure the "release" target is fully up-to-date
install: ## Build and install the release target to your system
	@$(MKDIR) $(INSTALLDIRS)
	@printf "%-8s %-16s -> %s\n" "(CP)" "$(RELBIN)" "$(TARGET_INSTALL_BINDIR)/$(notdir $(RELBIN))" $(QUIETMODE)
	@$(CP) $(RELBIN) $(TARGET_INSTALL_BINDIR)/$(notdir $(RELBIN))
	@printf "%-8s %-16s -> %s\n" "(CP)" "$(DBGBIN)" "$(TARGET_INSTALL_BINDIR)/$(notdir $(DBGBIN))" $(QUIETMODE)
	@$(CP) $(DBGBIN) $(TARGET_INSTALL_BINDIR)/$(notdir $(DBGBIN))
	@printf "%-8s %-16s -> %s\n" "(CP)" "$(INSTALLHEADERS)" "$(addprefix $(TARGET_INSTALL_HEADERDIR)/,$(notdir $(INSTALLHEADERS)))" $(QUIETMODE)
	@$(CP) $(INSTALLHEADERS) $(addprefix $(TARGET_INSTALL_HEADERDIR)/,$(notdir $(INSTALLHEADERS)))
	@printf "%-8s %-16s -> %s\n" "(CP)" "$(INSTALLMANPAGES)" "$(addprefix $(INSTALLMANDIR)/,$(notdir $(INSTALLMANPAGES)))" $(QUIETMODE)
	@$(CP) $(INSTALLMANPAGES) $(addprefix $(INSTALLMANDIR)/,$(notdir $(INSTALLMANPAGES)))
	@printf "%-8s %-16s -> %s\n" "(LN)" "$(notdir $(RELBIN))" "$(TARGETNAME)$(TARGETSUFFIX)" $(QUIETMODE)
	@$(LN) $(TARGET_INSTALL_BINDIR)/$(notdir $(RELBIN)) $(INSTALL_BINDIR)/$(TARGETNAME)$(TARGETSUFFIX)
	@printf "%-8s %-16s -> %s\n" "(LN)" "$(notdir $(DBGBIN))" "$(TARGETNAME)-debug$(TARGETSUFFIX)" $(QUIETMODE)
	@$(LN) $(TARGET_INSTALL_BINDIR)/$(notdir $(DBGBIN)) $(INSTALL_BINDIR)/$(TARGETNAME)-debug$(TARGETSUFFIX)
	@printf "%-8s %-16s -> %s\n" "(LN)" "$(TARGET_INSTALL_HEADERDIR)/$(TARGETNAME).h" "$(INSTALL_HEADERDIR)/$(TARGETNAME).h" $(QUIETMODE)
	@$(LN) $(TARGET_INSTALL_HEADERDIR)/$(notdir $(TARGETNAME).h) $(INSTALL_HEADERDIR)/$(TARGETNAME).h
	@printf "Successfully installed library to [ %s ] and header to [ %s ].\n" "$(INSTALL_BINDIR)" "$(INSTALL_HEADERDIR)"

uninstall:  ## Remove everything installed by the "install" target.
	@printf "%-8s %s\n" "(RM)" "$(INSTALL_BINDIR)/$(TARGETNAME)$(TARGETSUFFIX)" $(QUIETMODE)
	@$(RM) $(INSTALL_BINDIR)/$(TARGETNAME)$(TARGETSUFFIX)
	@printf "%-8s %s\n" "(RM)" "$(INSTALL_BINDIR)/$(TARGETNAME)-debug$(TARGETSUFFIX)" $(QUIETMODE)
	@$(RM) $(INSTALL_BINDIR)/$(TARGETNAME)-debug$(TARGETSUFFIX)
	@printf "%-8s %s\n" "(RM)" "$(TARGET_INSTALL_BINDIR)/$(notdir $(RELBIN))" $(QUIETMODE)
	@$(RM) $(TARGET_INSTALL_BINDIR)/$(notdir $(RELBIN))
	@printf "%-8s %s\n" "(RM)" "$(TARGET_INSTALL_BINDIR)/$(notdir $(DBGBIN))" $(QUIETMODE)
	@$(RM) $(TARGET_INSTALL_BINDIR)/$(notdir $(DBGBIN))
	@printf "%-8s %s\n" "(RM)" "$(addprefix $(TARGET_INSTALL_HEADERDIR)/,$(notdir $(INSTALLHEADERS)))" $(QUIETMODE)
	@$(RM) $(addprefix $(TARGET_INSTALL_HEADERDIR)/,$(notdir $(INSTALLHEADERS)))
	@printf "%-8s %s\n" "(RM)" "$(INSTALL_HEADERDIR)/$(TARGETNAME).h" $(QUIETMODE)
	@$(RM) $(INSTALL_HEADERDIR)/$(TARGETNAME).h
	@printf "%-8s %s\n" "(RM)" "$(addprefix $(INSTALLMANDIR)/,$(notdir $(INSTALLMANPAGES)))" $(QUIETMODE)
	@$(RM) $(addprefix $(INSTALLMANDIR)/,$(notdir $(INSTALLMANPAGES)))

#   Ensure all of the build and output directories exist.
prep:   ## Ensure the build and output directory trees exist as expected.
	@$(MKDIR) $(RELBUILDDIR)
	@$(MKDIR) $(DBGBUILDDIR)
	@$(MKDIR) $(DBGRBUILDDIR)
	@$(MKDIR) $(TESTBUILDDIR)
	@$(MKDIR) $(RELOUTDIR)
	@$(MKDIR) $(DBGOUTDIR)
	@$(MKDIR) $(DBGROUTDIR)
	@$(MKDIR) $(TESTOUTDIR)
	@$(MKDIR) $(LIBDIR)

#   Clean-hard will remove all of the object files, but also the outputs
#   and the dependency files generated by the -MMD -MP flags.
clean-hard: clean   ##  Perform a clean, as well as removing all products and *.d dependency files.
	@printf "%-8s %s\n" "(RM)" "$(notdir $(RELBIN))" $(QUIETMODE)
	@$(RM) $(RELBIN)
	@printf "%-8s %s\n" "(RM)" "$(notdir $(DBGBIN))" $(QUIETMODE)
	@$(RM) $(DBGBIN)
	@printf "%-8s %s\n" "(RM)" "$(notdir $(DBGRBIN))" $(QUIETMODE)
	@$(RM) $(DBGRBIN)
	@printf "%-8s %s\n" "(RM)" "$(notdir $(TESTBIN))" $(QUIETMODE)
	@$(RM) $(TESTBIN)

#   Clean will only remove the object files, nothing more.
clean: $(COMPONENTS) $(TESTCOMPONENTS)  ##  Remove all object files from the build tree.

remake: prep    ##  Perform a "clean", followed by "all"
	@$(MAKE) clean
	@$(MAKE) all

#	Sub-component Make arguments
#   These are effectively the same rule, just split up to allow differentiation
#   between debug/release builds, and testing/debugger builds.
#
#   Recursively build each sub-component of the full project.
#   If components depend on each other, they should only do so via
#   cross-referenced headers, and the .d dependancy files will clear this up
#   before final linking to the end target.
$(COMPONENTS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

$(TESTCOMPONENTS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)


#   Testing Build Target
#   Execute the test executable at the end, to allow its return code to influence this Makefile.
#   The aim of this is to allow the test executable to report nonzero if tests fail, aborting
#   the Makefile and halting an "all" target.
#
#   Ideally, this could be used in a build system to report the number of failing tests
#   to some larger system, in order to ensure only successfully tested builds proceed.
test: prep $(COMPONENTS) $(TESTCOMPONENTS)  ##  Build and then execute the testing suite.
	@printf "%-8s %-16s -> %s\n" "(CC)" "$(notdir $(TESTOBJS) $(DBGLIBS))" "$(TESTBIN:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(CC) $(TESTFLAGS) -o $(TESTBIN) $(TESTOBJS) $(DBGLIBS)
	@$(TESTBIN) $(QUIETMODE)


#   Debugger Build Target
debugger: prep $(COMPONENTS) $(TESTCOMPONENTS)  ##  Build the project with debugging enabled, and insert an entry point for use with a debugger.
	@printf "%-8s %-16s -> %s\n" "(CC)" "$(notdir $(DBGROBJS) $(DBGLIBS))" "$(DBGRBIN:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(CC) $(DBGRFLAGS) -o $(DBGRBIN) $(DBGROBJS) $(DBGLIBS)


#   Debug Build Target
debug: prep $(COMPONENTS)   ##  Build the project with debugging enabled.
	@printf "%-8s %-16s -> %s\n" "(LIBTOOL)" "$(notdir $(DBGOBJS) $(DBGLIBS)) " "$(DBGBIN:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(LIBTOOL) $(DBGBIN) $(DBGOBJS) $(DBGLIBS)


#   Release Build Target
release: prep $(COMPONENTS)    ##  Build the project with debugging disabled and strip the resulting output.
	@printf "%-8s %-16s -> %s\n" "(LIBTOOL)" "$(notdir $(RELOBJS) $(RELLIBS)) " "$(RELBIN:$(ROOTDIR)/%=%)" $(QUIETMODE)
	@$(LIBTOOL) $(RELBIN) $(RELOBJS) $(RELLIBS)
	@printf "%-8s %s\n" "(STRIP)" "$(notdir $(RELBIN))" $(QUIETMODE)
	@$(STRIP) $(RELBIN)

