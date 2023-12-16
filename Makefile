# Makefile generated by Chat-GPT 3.5
# 16.12.2023

CC := gcc
CFLAGS := -Wall -Wextra -std=c99
LDFLAGS := -lncurses -lm

SRCDIR := src
OBJDIR := obj
BINDIR := bin
INCDIR := include

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE := $(BINDIR)/rogue

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

