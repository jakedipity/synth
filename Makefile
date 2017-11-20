IDIR = include/
BUILDDIR = build
DEBUGDIR = $(BUILDDIR)/debug
RELEASEDIR = $(BUILDDIR)/release
SRCDIR = src
BINDIR = bin
DEBUGTARGET = $(BINDIR)/debug/openparse
RELEASETARGET = $(BINDIR)/release/openparse

CC = g++
CFLAGS = -I$(IDIR) -std=c++11
CFLAGSDEBUG = $(CFLAGS) -g -O -Wall -Weffc++ -pedantic \
			-pedantic-errors -Wextra -Wcast-align \
			-Wcast-qual -Wchar-subscripts -Wcomment -Wconversion \
			-Wdisabled-optimization \
			-Werror -Wfloat-equal  -Wformat  -Wformat=2 \
			-Wformat-nonliteral -Wformat-security \
			-Wformat-y2k \
			-Wimport -Winit-self -Winline \
			-Winvalid-pch \
			-Wlong-long -Wmissing-braces \
			-Wmissing-field-initializers -Wmissing-format-attribute \
			-Wmissing-include-dirs -Wmissing-noreturn \
			-Wpacked -Wparentheses -Wpointer-arith \
			-Wredundant-decls -Wreturn-type \
			-Wsequence-point -Wshadow -Wsign-compare -Wstack-protector \
			-Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default \
			-Wswitch-enum -Wtrigraphs -Wuninitialized \
			-Wunknown-pragmas -Wunreachable-code -Wunused \
			-Wunused-function -Wunused-label -Wunused-parameter \
			-Wunused-value -Wunused-variable -Wvariadic-macros \
			-Wvolatile-register-var -Wwrite-strings
CFLAGSRELEASE = $(CFLAGS)	-s -O2 -fno-rtti -fno-stack-protector -ffunction-sections \
				-fdata-sections -Wl,--gc-sections -fno-math-errno \
				-fmerge-all-constants -fno-ident -ffast-math -Wl,-z,norelro \
				-Wl,--hash-style=gnu -Wl,--build-id=none
DEPFLAGS = -MMD -MP

SRCS = $(patsubst $(SRCDIR)/%,%,$(shell find $(SRCDIR) -name '*.cpp'))
DEBUGOBJS = $(addprefix $(DEBUGDIR)/, $(SRCS:.cpp=.o))
RELEASEOBJS = $(addprefix $(RELEASEDIR)/, $(SRCS:.cpp=.o))
DEBUGDEPS = $(addprefix $(DEBUGDIR)/, $(SRCS:.cpp=.d))
RELEASEDEPS = $(addprefix $(RELEASEDIR)/, $(SRCS:.cpp=.d))

.PHONY: all
all: release

.PHONY: debug
debug: $(DEBUGOBJS) | $(BINDIR)/debug/.
	$(CC) $? -o $(DEBUGTARGET) $(CFLAGSDEBUG)

.PHONY: release
release: $(RELEASEOBJS) | $(BINDIR)/release/.
	$(CC) $? -o $(RELEASETARGET) $(CFLAGSRELEASE)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.SECONDEXPANSION:
$(DEBUGDIR)/%.o: $(SRCDIR)/%.cpp | $$(@D)/.
	$(CC) -c $< -o $@ $(CFLAGSDEBUG) $(DEPFLAGS)

.SECONDEXPANSION:
$(RELEASEDIR)/%.o: $(SRCDIR)/%.cpp | $$(@D)/.
	$(CC) -c $< -o $@ $(CFLAGSRELEASE) $(DEPFLAGS)

%/.:
	@mkdir -p $@

-include $(DEBUGDEPS) $(RELEASEDEPS)
