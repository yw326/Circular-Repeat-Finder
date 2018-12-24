INCLUDES	+= ./
CFLAGS	+= -I$(INCLUDES)
OBJCFLAGS	+= $(CFLAGS)

OBJDIR	= obj

_DEPS	= sfxtree.h benchmark.h maximal_repeat.h
_SOURCE	= sfxtree.c benchmark.c maximal_repeat.c

DEPS	= $(_DEPS)
OBJ	= $(patsubst %, $(OBJDIR)/%, $(_SOURCE:.c=.o))

.PHONY:	all
.PHONY:	clean

all:	$(OBJDIR) mccreight 

$(OBJDIR):	
	mkdir -p $(OBJDIR)


	
mccreight:	$(OBJ) $(OBJDIR)/mccreight.o
	$(CC) -o $@ $^ $(CFLAGS)


$(OBJDIR)/%.o:	%.c $(DEPS)
	$(CC) -std=c99 -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o:	%.m $(DEPS)
	$(CC) -c -o $@ $< $(OBJCFLAGS)

clean:
	rm -f $(OBJDIR)/*.o naive mccreight ukkonen
