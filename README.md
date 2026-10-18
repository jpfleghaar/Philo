# 🍝 philo — Dining Philosophers

A multithreaded simulation of the dining philosophers problem in C, using POSIX threads and mutexes.

*This project was created as part of the 42 curriculum by jpflegha.*

<!-- add screenshot: philo.png (colored terminal output of ./bin/philo 5 800 200 200) -->

## Description

A number of philosophers sit around a table with one fork between each pair of neighbours. To eat, a philosopher needs **both** forks next to them. Each philosopher repeatedly **eats → sleeps → thinks**. If a philosopher doesn't start eating within `time_to_die` milliseconds after their last meal, they die and the simulation stops.

Every philosopher is a **thread** and every fork is a **mutex**. The program must avoid deadlocks, data races and starvation, and must report a death within 10 ms.

How this implementation works:

- **Deadlock prevention:** even-positioned philosophers take their left fork first, odd ones their right fork first (`assign_forks` in `init.c`), so the forks are never all held in a circle.
- **Synchronized start:** all threads wait on the table mutex until everything is created, and the start time is set once for everyone.
- **De-synchronization:** odd-numbered philosophers wait for half of `time_to_eat` before their first meal, so neighbours don't fight for the same forks at the start.
- **Fairness with an odd number of philosophers:** after sleeping, a philosopher thinks for a short calculated time (`thinking` in `sync_utils.c`) so neighbours get a chance to take the forks.
- **Monitor thread:** a separate thread checks every philosopher's last meal time and prints the death message.
- **Thread-safe state:** shared values are read and written through small mutex-protected getters and setters (`getters_setters.c`). All output goes through one function that locks the table mutex, so lines never mix and nothing is printed after the simulation ends.
- **Error handling:** wrappers around `malloc`, `pthread_create/join` and `pthread_mutex_init/destroy` check every return value (`safe_functions.c`).

## Key concepts

- POSIX threads (`pthread_create`, `pthread_join`)
- Mutexes and avoiding deadlocks and data races
- Starvation and fairness in concurrent programs
- Time measurement with `gettimeofday` and custom sleep
- Input validation and clean resource release

## Build and run

```bash
git clone https://github.com/jpfleghaar/Philo.git
cd Philo/philo
make
```

The binary is created at `bin/philo`.

```bash
./bin/philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

All times are in milliseconds and must be at least 60. Example:

```bash
$ ./bin/philo 4 410 200 200
0     1 is thinking
0     2 is thinking
0     2 has taken a fork
0     2 has taken a fork
0     2 is eating
1     4 is thinking
1     4 has taken a fork
1     4 has taken a fork
1     4 is eating
1     3 is thinking
202   4 is sleeping
202   1 has taken a fork
...
```

The output is colored in the terminal. Each line shows the timestamp in ms since the start, the philosopher's number and the action.

Test cases I checked:

| Command | Expected result |
|:---|:---|
| `./bin/philo 1 800 200 200` | takes one fork, dies at ~800 ms |
| `./bin/philo 4 410 200 200` | nobody dies |
| `./bin/philo 5 800 200 200 7` | nobody dies, stops after every philosopher ate 7 times |
| `./bin/philo 4 310 200 100` | one philosopher dies |
| `./bin/philo 200 410 200 200 5` | nobody dies |

Other Makefile targets: `make run ARGS="5 800 200 200"`, `make clean`, `make fclean`, `make re`, `make tester` (clones [LazyPhilosophersTester](https://github.com/MichelleJiam/LazyPhilosophersTester)), `make help`.

## Project structure

```
philo/
├── Makefile
├── philo.h             # structs (table, philosopher, fork) and prototypes
├── main.c              # argument count check and program flow
├── parsing.c           # input validation and conversion
├── init.c              # allocation, mutex setup, fork assignment
├── dinner.c            # philosopher routine, single-philosopher case, thread start/join
├── monitor.c           # death detection thread
├── getters_setters.c   # mutex-protected access to shared values
├── sync_utils.c        # start synchronization, de-sync, thinking delay
├── safe_functions.c    # error-checked malloc, mutex and thread wrappers
└── utils.c             # time, sleep, status output, cleanup
```

## What I learned

- How deadlocks happen and how a simple fork ordering prevents them.
- That avoiding deadlock is not enough: with an odd number of philosophers, one can still starve unless the others leave room.
- Every shared variable needs a clear owner or a mutex; tools like `-fsanitize=thread` help to find mistakes.
- `usleep` is not precise, so a custom sleep that checks the time in small steps is needed for accurate timing.
