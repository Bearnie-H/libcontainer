#   This file contains the tunable build parameters for the library, allowing
#    for simple specification at build-time to override the default values
#    specified in the library itself.

#   This variable holds the full set of additional definitions to pass to the compiler.
TUNING_PARAMETERS :=

#   Set the default memory allocation (unless otherwise specified) when initializing an Array_t.
LIBCONTAINER_ARRAY_DEFAULT_CAPACITY :=

# ...

ifdef LIBCONTAINER_ARRAY_DEFAULT_CAPACITY
TUNING_PARAMETERS += -DLIBCONTAINER_ARRAY_DEFAULT_CAPACITY=$(LIBCONTAINER_ARRAY_DEFAULT_CAPACITY)
endif

#   Add up all of the additional definitions
CFLAGS += $(TUNING_PARAMETERS)
