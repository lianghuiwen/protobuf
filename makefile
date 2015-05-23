CC = g++
FLAG = -g -Wall
SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.o,$(SRCS))
INCS = -I$(PROTOBUF_PATH)/include
LIBS = -L$(PROTOBUF_PATH)/lib -l protobuf
TARG = a.out

$(TARG): $(OBJS)
	$(CC) $(FLAG) -o $@ $(OBJS) $(LIBS)

%.o: %.cc
	$(CC) $(FLAG) -c $^ $(INCS) -o $@

clean:
	rm -f $(OBJS) $(TARG)