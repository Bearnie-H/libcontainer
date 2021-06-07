
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

$(TESTBUILDDIR)/%.o : %.c
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<"
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)"
	@$(CC) -c $(TESTFLAGS) -o $@ $< -MMD -MP

$(DBGBUILDDIR)/%.o : %.c
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<"
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)"
	@$(CC) -c $(DBGFLAGS) -o $@ $< -MMD -MP

$(DBGRBUILDDIR)/%.o : %.c
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<"
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)"
	@$(CC) -c $(DBGRFLAGS) -o $@ $< -MMD -MP

$(RELBUILDDIR)/%.o : %.c
ifdef FMTTOOL
	@printf "%-8s %-16s\n" "(FMT)" "$<"
	@$(FMTTOOL) -i $<
endif
	@printf "%-8s %-16s -> %s\n" "(CC)" "$<" "$(@:$(ROOTDIR)/%=%)"
	@$(CC) -c $(RELFLAGS) -o $@ $< -MMD -MP

