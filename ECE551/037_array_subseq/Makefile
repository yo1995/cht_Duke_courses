CC= gcc
CFLAGS = -std=gnu99 -Wall
test-subseq: maxSeq.o test-subseq.o
	$(CC) -o test-subseq maxSeq.o test-subseq.o
maxSeq.o: maxSeq.c
	$(CC) $(CFLAGS) -c maxSeq.c
test-subseq.o :test-subseq.c
	$(CC) $(CFLAGS) -c test-subseq.c