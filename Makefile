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
MINOR_VERSION := 5
PATCH_VERSION := 2
VERSION       := v$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION)

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
COMPONENTS     := src/array src/list src/hashmap src/tree/binary-tree src/stack
TESTCOMPONENTS := src

#   Additional Libraries to include when building the final applications
SLIBS          := $(wildcard $(LIBDIR)/*.a)     #   Static Libraries
DLIBS          := $(wildcard $(LIBDIR)/*.so)    #   Dynamic Libraries

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
INSTALL_PATH     ?= /usr/local/$(TARGETNAME)#   If this is defined as an environment variable, allow this to be overwritten
INSTALLBINDIR    := $(INSTALL_PATH)/lib
INSTALLHEADERDIR := $(INSTALL_PATH)/include
INSTALLMANDIR    ?= /usr/local/share/man/man1
INSTALLDIRS      := $(INSTALLBINDIR) $(INSTALLHEADERDIR) $(INSTALLMANDIR)
#   Other installation directories go here...

INSTALLBIN       := $(RELBIN)
INSTALLHEADERS   := $(wildcard $(ROOTDIR)/include/*.h)
INSTALLLIBS      := $(SLIBS) $(DLIBS)
INSTALLMANPAGES  := $(wildcard $(ROOTDIR)/man/*.1)
# Other installation targets and files go here

#   Uninstallation settings - Remove any and all artefacts moved by the "install" target
#       such that it can no longer be managed by either "clean" or "clean-hard"
UNINSTALLFILES := 

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
	@printf "%-8s %-16s -> %s\n" "(CP)" "$(INSTALLBIN)" "$(INSTALLBINDIR)/$(notdir $(INSTALLBIN))"
	@$(CP) $(INSTALLBIN) $(INSTALLBINDIR)/$(notdir $(INSTALLBIN))
	@printf "%-8s %-16s -> %s\n" "(CP)" "$(INSTALLHEADERS)" "$(addprefix $(INSTALLHEADERDIR)/,$(notdir $(INSTALLHEADERS)))"
	@$(CP) $(INSTALLHEADERS) $(addprefix $(INSTALLHEADERDIR)/,$(notdir $(INSTALLHEADERS)))
	@printf "%-8s %-16s -> %s\n" "(CP)" "$(INSTALLMANPAGES)" "$(addprefix $(INSTALLMANDIR)/,$(notdir $(INSTALLMANPAGES)))"
	@$(CP) $(INSTALLMANPAGES) $(addprefix $(INSTALLMANDIR)/,$(notdir $(INSTALLMANPAGES)))
	@printf "%-8s %-16s -> %s\n" "(LN)" "$(notdir $(INSTALLBIN))" "$(TARGETNAME)$(TARGETSUFFIX)"
	@$(LN) $(INSTALLBINDIR)/$(notdir $(INSTALLBIN)) $(INSTALLBINDIR)/$(TARGETNAME)$(TARGETSUFFIX)

uninstall:  ## Remove everything installed by the "install" target.
	@printf "%-8s %s\n" "(RM)" "$(INSTALLBINDIR)/$(TARGETNAME)$(TARGETSUFFIX)"
	@$(RM) $(INSTALLBINDIR)/$(TARGETNAME)$(TARGETSUFFIX)
	@printf "%-8s %s\n" "(RM)" "$(INSTALLBINDIR)/$(notdir $(INSTALLBIN))"
	@$(RM) $(INSTALLBINDIR)/$(notdir $(INSTALLBIN))
	@printf "%-8s %s\n" "(RM)" "$(addprefix $(INSTALLHEADERDIR)/,$(notdir $(INSTALLHEADERS)))"
	@$(RM) $(addprefix $(INSTALLHEADERDIR)/,$(notdir $(INSTALLHEADERS)))
	@printf "%-8s %s\n" "(RM)" "$(addprefix $(INSTALLMANDIR)/,$(notdir $(INSTALLMANPAGES)))"
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
	@printf "%-8s %s\n" "(RM)" "$(notdir $(RELBIN))"
	@$(RM) $(RELBIN)
	@printf "%-8s %s\n" "(RM)" "$(notdir $(DBGBIN))"
	@$(RM) $(DBGBIN)
	@printf "%-8s %s\n" "(RM)" "$(notdir $(DBGRBIN))"
	@$(RM) $(DBGRBIN)
	@printf "%-8s %s\n" "(RM)" "$(notdir $(TESTBIN))"
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
	@printf "%-8s %-16s -> %s\n" "(CC)" "$(notdir $(TESTOBJS))" "$(TESTBIN:$(ROOTDIR)/%=%)"
	@$(CC) $(TESTFLAGS) -o $(TESTBIN) $(TESTOBJS) $(SLIBS) $(DLIBS)
	@$(TESTBIN)


#   Debugger Build Target
debugger: prep $(COMPONENTS) $(TESTCOMPONENTS)  ##  Build the project with debugging enabled, and insert an entry point for use with a debugger.
	@printf "%-8s %-16s -> %s\n" "(CC)" "$(notdir $(DBGROBJS))" "$(DBGRBIN:$(ROOTDIR)/%=%)"
	@$(CC) $(DBGRFLAGS) -o $(DBGRBIN) $(DBGROBJS) $(SLIBS) $(DLIBS)


#   Debug Build Target
debug: prep $(COMPONENTS)   ##  Build the project with debugging enabled.
	@printf "%-8s %-16s -> %s\n" "(LIBTOOL)" "$(notdir $(DBGOBJS)) " "$(DBGBIN:$(ROOTDIR)/%=%)"
	@$(LIBTOOL) $(DBGBIN) $(DBGOBJS) $(SLIBS) $(DLIBS)


#   Release Build Target
release: prep $(COMPONENTS)    ##  Build the project with debugging disabled and strip the resulting output.
	@printf "%-8s %-16s -> %s\n" "(LIBTOOL)" "$(notdir $(RELOBJS)) " "$(RELBIN:$(ROOTDIR)/%=%)"
	@$(LIBTOOL) $(RELBIN) $(RELOBJS) $(SLIBS) $(DLIBS)
	@printf "%-8s %s\n" "(STRIP)" "$(notdir $(RELBIN))"
	@$(STRIP) $(RELBIN)

