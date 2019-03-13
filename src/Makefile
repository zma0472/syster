
#  $Id: Makefile,v 1.1 2013/08/12 22:12:28 zma0472 Exp $

CC=gcc
CFLAGS=-g

HEADERS = log.h syster.h
OBJS = daemon.o env.o io.o lock.o main.o signal.o

CPC_OBJS = $(OBJS) cpc_args.o cpc_env.o cpc_test.o cpc_usage.o cpc_version.o
MEM_OBJS = $(OBJS) mem_args.o mem_env.o mem_test.o mem_usage.o mem_version.o
NET_OBJS = $(OBJS) net_args.o net_env.o net_test.o net_usage.o net_version.o
UFS_OBJS = $(OBJS) ufs_args.o ufs_env.o ufs_test.o ufs_usage.o ufs_version.o

BINS = syster_cpc syster_mem syster_net syster_ufs syster_report

all: $(BINS)

syster_cpc: $(CPC_OBJS) $(HEADERS)
	$(CC) -o $@ $(CPC_OBJS)

syster_mem: $(MEM_OBJS) $(HEADERS)
	$(CC) -o $@ $(MEM_OBJS)

syster_net: $(NET_OBJS) $(HEADERS)
	$(CC) -o $@ $(NET_OBJS)

syster_ufs: $(UFS_OBJS) $(HEADERS)
	$(CC) -o $@ $(UFS_OBJS)

syster_report: syster_report.o io.o $(HEADERS)
	$(CC) -o $@ syster_report.o io.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f core *.o $(BINS)

