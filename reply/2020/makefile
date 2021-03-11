DEBUG_OPTS = -std=c++17 -O3 -Wall -Wfatal-errors
RUN_OPTS = -std=c++17 -O3 -fopenmp -DNDEBUG

debug: sol.cpp
	g++ $(DEBUG_OPTS) sol.cpp -o debug

run: sol.cpp
	g++ $(RUN_OPTS) sol.cpp -o run

opt: optimize.cpp
	g++ $(RUN_OPTS) optimize.cpp -o opt


clean:
	rm -f debug run opt


.PHONY: clean debug run
