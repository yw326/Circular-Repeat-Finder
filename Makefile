INCLUDES	+= ./
CFLAGS	+= -I$(INCLUDES)
OBJCFLAGS	+= $(CFLAGS)

#ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security
#ASAN_LIBS  = -static-libasan

OBJDIR	= obj

_DEPS	= sfxtree.h maximal_repeat.h file_helper.h debug_helper.h edit_distance.h KMP.h str_helper.h circle_repeat_finder.h rev_circle_repeat_finder.h
_SOURCE	= sfxtree.c maximal_repeat.c file_helper.c debug_helper.c edit_distance.c KMP.c str_helper.c circle_repeat_finder.c rev_circle_repeat_finder.c



DEPS	= $(_DEPS)
OBJ	= $(patsubst %, $(OBJDIR)/%, $(_SOURCE:.c=.o))

.PHONY:	all
.PHONY:	clean

all:	$(OBJDIR) mccreight 

$(OBJDIR):	
	mkdir -p $(OBJDIR)


mccreight:	$(OBJ) $(OBJDIR)/mccreight.o
	$(CC) -o $@ $^ $(CFLAGS)
	#$(CC) $(ASAN_FLAGS) -o $@ $^ $(CFLAGS)


$(OBJDIR)/%.o:	%.c $(DEPS)
	$(CC) -std=c99 -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o:	%.m $(DEPS)
	$(CC) -c -o $@ $< $(OBJCFLAGS)

clean:
	rm -f $(OBJDIR)/*.o naive mccreight ukkonen
