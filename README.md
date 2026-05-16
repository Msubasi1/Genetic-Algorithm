# Genetic Algorithm

[![C](https://img.shields.io/badge/C-99-A8B9CC.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Make](https://img.shields.io/badge/Build-Make-427819.svg)](https://www.gnu.org/software/make/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A binary-encoded **Genetic Algorithm** simulator in C. Tracks a population of fixed-length binary chromosomes across multiple generations, applying user-defined selection, crossover, and mutation schedules. Built as an assignment for **Hacettepe University — BBM203 Software Lab**.

## How It Works

The simulation operates on:

- a **Gene** — a single bit (0 or 1), linked into…
- a **Chromosome** — a fixed-length linked list of `PROB_SIZE` genes, decoded to an integer (big-endian), and…
- a **Population** — a linked list of `POP_SIZE` chromosomes.

Each generation runs the following sequence:

1. **Selection** — pair chromosomes for crossover according to `selection` file
2. **Crossover** — swap bit ranges between matched pairs (`xover` file)
3. **Mutation** — flip a chosen bit position (`mutate` file)
4. **Decode** — recompute decimal value of each chromosome
5. **Sort** — order population by decimal value
6. **Report** — print population and update best-so-far (lowest decimal value)

The objective is **minimization** — the best chromosome has the smallest decoded integer.

## Tech Stack

- **Language**: C99
- **Build**: GNU Make / `gcc -lm`
- **Stdlib**: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `math.h`, `stdbool.h`

## Project Structure

```
.
├── src/
│   ├── genetic_algo.c   # GA implementation
│   ├── Makefile         # Build target
│   └── GA               # Pre-built binary (Linux ELF)
├── GA.pdf               # Original assignment specification
├── LICENSE
└── README.md
```

## Input Files (required at runtime)

Place these files in the **current working directory** when running the binary:

| File | Format |
|------|--------|
| `population` | Initial population — one chromosome per line, genes separated by `:` (e.g. `1:0:1:1:0:0:1:0`) |
| `selection`  | Pairings per generation — one line per generation, pairs as `i:j i:j …` |
| `xover`      | Crossover range per generation — `start:end` |
| `mutate`     | Mutation bit position per generation — single integer |

If any required file is missing, the program prints `file(s) not found` and exits with status 88.

## Build

```bash
cd src
make
```

Or compile directly:

```bash
cd src
gcc -o GA genetic_algo.c -lm
```

## Run

```bash
cd src
# Make sure population, selection, xover, mutate files exist here
./GA <PROB_SIZE> <POP_SIZE> <MAX_GENE>
```

| Argument | Meaning |
|----------|---------|
| `PROB_SIZE` | Number of genes per chromosome |
| `POP_SIZE`  | Number of chromosomes in the population |
| `MAX_GENE`  | Number of generations to simulate |

### Example

```bash
./GA 8 6 10
```

Runs 10 generations on a population of 6 chromosomes, each 8 bits wide.

## Example Output

```
GENERATION: 0
1 0 1 1 0 0 1 0  -> 178
0 1 0 0 1 1 0 1  -> 77
1 1 0 0 0 1 0 0  -> 196
0 0 1 1 0 1 1 0  -> 54
1 0 0 1 1 1 0 0  -> 156
0 1 1 0 0 0 1 1  -> 99
Best chromosome found so far: 0 0 1 1 0 1 1 0  -> 54
GENERATION: 1
...
```

## Clean

```bash
cd src
make clean
```

## Author

**Muhammet Subaşı** ([@Msubasi1](https://github.com/Msubasi1))

## License

Released under the [MIT License](LICENSE).
