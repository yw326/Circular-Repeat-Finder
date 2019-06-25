INCLUDES	+= ./
CFLAGS	+= -I$(INCLUDES)
OBJCFLAGS	+= $(CFLAGS)

#ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security
#ASAN_LIBS  = -static-libasan

OBJDIR	= obj

_DEPS	= sfxtree.h maximal_repeat.h file_helper.h debug_helper.h edit_distance.h  str_helper.h circle_repeat_finder.h rev_circle_repeat_finder.h task.h
_SOURCE	= sfxtree.c maximal_repeat.c file_helper.c debug_helper.c edit_distance.c  str_helper.c circle_repeat_finder.c rev_circle_repeat_finder.c task.c



DEPS	= $(_DEPS)
OBJ	= $(patsubst %, $(OBJDIR)/%, $(_SOURCE:.c=.o))

.PHONY:	all
.PHONY:	clean

all:	$(OBJDIR) mdf partition

$(OBJDIR):	
	mkdir -p $(OBJDIR)


mdf:	$(OBJ) $(OBJDIR)/mdf.o
	$(CC) -o $@ $^ $(CFLAGS)

partition:	$(OBJ) $(OBJDIR)/partition.o
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR)/%.o:	%.c $(DEPS)
	$(CC) -std=gnu99 -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o:	%.m $(DEPS)
	$(CC) -c -o $@ $< $(OBJCFLAGS)

clean:
	rm -f $(OBJDIR)/*.o mdf partition
