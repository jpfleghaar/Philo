# Compiler and compiler flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Isrc -pthread -g

# Directories
OBJ_DIR 	= obj
BIN_DIR 	= bin
TESTER_DIR 	= tester

# Target executable name
TARGET = $(BIN_DIR)/philo

# Source files (manually listed)
SRC_FILES = main.c dinner.c getters_setters.c init.c monitor.c parsing.c safe_functions.c \
            sync_utils.c utils.c

# Define the full source and object file paths (without using SRC_DIR)
SRC = $(SRC_FILES)
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Default target
all: directories $(TARGET)
	@printf "\n\033[95m" ; \
	printf "  ██████╗\n" ; \
	printf "  ██╔═══ \n" ; \
	printf "  ██║    \n" ; \
	printf "  ██║    \n" ; \
	printf "  ██████║\n" ; \
	printf "  ╚═════╝\n" ; \
	printf "Creating $(TARGET)...\n " ; \
	printf "\033[0m\n"


# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Compile the final binary
$(TARGET): $(OBJ)
	@echo -e "\033[1;33mLinking the program... \033[0m🌀"
	$(CC) $(CFLAGS) -o $@ $^

# Compile object files
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@echo  "\033[1;32mObject directory deleted\033[0m"

# Clean everything
fclean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(TESTER_DIR)
	@echo  "\033[1;32mEverything is deleted\033[0m"

# Rebuild everything
re: fclean all

# Run the program
run: all
	$(TARGET) $(ARGS)

# Clone Tester repository
tester: all
	@git clone https://github.com/MichelleJiam/LazyPhilosophersTester.git tester
	@cp $(TARGET) tester/philo
	@echo -e "\033[1;32mTester cloned and moved to ./bin\033[0m 🧪"


# Display help
help:
	@printf "\033[1;32mMakefile for C Philo\033[0m\n"
	@printf "\033[1;34mUsage:\033[0m\n"
	@printf " \033[1;33mmake\033[0m               	- Build the program\n"
	@printf " \033[1;33mmake clean\033[0m         	- Remove object files\n"
	@printf " \033[1;33mmake fclean\033[0m        	- Remove all files\n"
	@printf " \033[1;33mmake run ARGS=\\\"...\\\"\033[0m 	- Build and run with arguments\n"
	@printf " \033[1;33mmake tester\033[0m           	- Download philo tester\n"
	@printf " \033[1;33mmake help\033[0m           	- Display this help information\n"


.PHONY: all clean fclean re run help directories
