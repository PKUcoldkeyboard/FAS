#!/bin/bash

perf record -o perf-greedy.data -g ../FASSolver graph greedy
perf script -i perf-greedy.data | ./FlameGraph/stackcollapse-perf.pl | ./FlameGraph/flamegraph.pl > perf-greedy.svg

perf record -o perf-sort.data -g ../FASSolver graph sort
perf script -i perf-sort.data | ./FlameGraph/stackcollapse-perf.pl | ./FlameGraph/flamegraph.pl > perf-sort.svg

perf record -o perf-pagerank.data -g ../FASSolver graph pagerank
perf script -i perf-pagerank.data | ./FlameGraph/stackcollapse-perf.pl | ./FlameGraph/flamegraph.pl > perf-pagerank.svg