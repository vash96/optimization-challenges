DEBUG_OPTS = -std=c++17 -O3 -Wall -Wfatal-errors -Wextra -pedantic
RUN_OPTS = -std=c++17 -O3 -DNDEBUG # -fopenmp

debug: greedy.cpp
	g++ $(DEBUG_OPTS) greedy.cpp -o debug

greedy: greedy.cpp
	g++ $(RUN_OPTS) greedy.cpp -o greedy

opt-debug: optimizer.cpp
	g++ $(DEBUG_OPTS) optimizer.cpp -o opt-debug

opt: optimize.cpp
	g++ $(RUN_OPTS) optimizer.cpp -o opt


clean:
	rm -f debug greedy opt-debug opt


.PHONY: clean debug greedy opt-debug opt
