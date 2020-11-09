INCLUDES	+= ./
CFLAGS	+= -I$(INCLUDES)
OBJCFLAGS	+= $(CFLAGS)

#ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security
#ASAN_LIBS  = -static-libasan

OBJDIR	= obj

_DEPS	= Utils.h SuffixArray.h List.h TestUtils.h MRP.h TestSuffixArray.h hashset.h TestList.h TestMRP.h TestHashset.h MyStructs.h EditDistance.h DirectCircleRepeatFinder.h TestDirectCircleRepeatFinder.h InvertedCircleRepeatFinder.h TestInvertedCircleRepeatFinder.h PartitionTask.h TestPartitionTask.h

_SOURCE	= Utils.c SuffixArray.c List.c TestUtils.c MRP.c TestSuffixArray.c hashset.c TestList.c TestMRP.c TestHashset.c EditDistance.c DirectCircleRepeatFinder.c TestDirectCircleRepeatFinder.c InvertedCircleRepeatFinder.c TestInvertedCircleRepeatFinder.c PartitionTask.c TestPartitionTask.c



DEPS	= $(_DEPS)
OBJ	= $(patsubst %, $(OBJDIR)/%, $(_SOURCE:.c=.o))

.PHONY:	all
.PHONY:	clean

all:	$(OBJDIR) CRPFinder Test Partition

$(OBJDIR):	
	mkdir -p $(OBJDIR)


CRPFinder:	$(OBJ) $(OBJDIR)/CRPFinder.o
	$(CC) -o $@ $^ $(CFLAGS)

Test:	$(OBJ) $(OBJDIR)/Test.o
	$(CC) -o $@ $^ $(CFLAGS)

Partition:	$(OBJ) $(OBJDIR)/Partition.o
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR)/%.o:	%.c $(DEPS)
	$(CC) -std=gnu99 -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o:	%.m $(DEPS)
	$(CC) -c -o $@ $< $(OBJCFLAGS)

clean:
	rm -f $(OBJDIR)/*.o mdf test partition
