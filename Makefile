# Zachary Leggett (zjl0002)
# Target binary file: scheduler
# Run using: make
CC = g++
CFLAGS = -g

scheduler: scheduler.cpp
	@echo "Creating the scheduler ..."
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp %.h
	@echo "Creating object files ..."
	$(CC) -c $< -o $@

clean:
	@echo "Deleting object and binary files ..."
	rm -rvf *.o scheduler
