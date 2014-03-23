all: default profile

default: p2.c
	mpicc -o p2 p2.c -lm

profile: p2.c
	mpicc -o p2-profile p2.c -g -pg -lm

all-factors: all-factors-default all-factors-profile

all-factors-default: p2-all-factors.c
	mpicc -o p2-all-factors p2-all-factors.c -lm

all-factors-profile: p2-all-factors.c
	mpicc -o p2-all-factors-profile p2-all-factors.c -g -pg -lm
