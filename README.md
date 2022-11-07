# primey

Multi-threaded prime calcatator and calculator.

## Installation

To install `primey`, clone the repository or download a build. It requires a C compiler and `zip`.

> Note: `primey` only works on POSIX-compliant systems due to usage of `pthreads`. In the future I may build `primey` with `win32-pthreads`. Until then, no Windows/DOS support will be implemented.

If opting to build `primey` from scratch, run `make build`.

### Make options

* `CC`: C/C++ compiler, must be C99 compliant
* `OPT`: Optimization level, a value `0`-`3`.
* `CFLAGS`: Flags passed to the C compiler *before* linking.
* `LDFLAGS`: Linker flags.
* `USEMOLD`: Use modern linker as provided by GCC. Requires modern linker.
* `FASTMATH`: Faster arithmetic at the cost of IEE 674 standard violation. Generally results in a 4-5% performance decrease.

## Technical Details for Nerds

Primey uses a naive algorithm. All primes $n > 3$ are in the form $6n \pm 1, n \in \mathbb{Z}^+$. Of these, when $n$ is in the form $5k + 4, k \in \mathbb{Z}^+$, $6n + 1$ is divisble by $5$. We will denote the set of primes as $\mathbb{P}$. All primes $(p > 2) \in \mathbb{P}$ are odd, and thus will have no even factors Secondarily, we can show that at least one factor of $n$ which we will denote $F \le \sqrt{n}$. This is trivial to show.

Suppose $n$ is not prime, that is, $n$ has at least 2 factors. $n = pq$ where $p,q > 1$ and $p \ge q$. Thus, $q^2 \le pq$ and $pq = n$. Thus, if $n$ is composite, it has at least one factor $q = F$ such that $q \le \sqrt{n}$.

We can employ another technique, that is caching the previous primes and using them for the factorization. Checking for distinctly prime factors will be much faster, but it comes at a large memory cost. It also makes distributing the computative load much harder, as all threads will need to access the computations of other threads. I may implement this feature in the future.

Primey distrubutes the computative load over threads. For the given number of accessable threads $t$ and the maximum number $m$, Primey will have each thread compute primes over an interval $\frac{m}{t}$. For example, to compute the first 100 million primes on two threads, the first thread would compute the first 50 million and the second the last 50 million.

## Benchmarks

All of these benchmarks were done on a 16-thread, 8-core AMD Ryzen 7 3800X @ 3.6 GHz with the IEE compliant Primey. I am planning to do more in the future, up to $10^{15}$. It will most likely require a short patch to remove the writing of the primes. Upcoming updates will store the primes in cache files for significantly large upper bounds.

### Primes under $10^3$

* Run 1: 0.00 s
* Run 2: 0.00 s
* Run 3: 0.00 s

### Primes under $10^4$

* Run 1: 0.00 s
* Run 2: 0.00 s
* Run 3: 0.00 s

### Primes under $10^5$

* Run 1: 0.00 s
* Run 2: 0.00 s
* Run 3: 0.00 s

### Primes under $10^6$

* Run 1: 0.03 s
* Run 2: 0.03 s
* Run 3: 0.03 s

### Primes under $10^7$

* Run 1: 0.75 s
* Run 2: 0.80 s
* Run 3: *0.69* s

### Primes under $10^8$

* Run 1: 19.46 s
* Run 2: 19.67 s
* Run 3: 19.27 s

### Primes under $10^9$

* Run 1: 9m 25s (565.28 s)
* Run 2: 9m 22s (562.81 s)
* Run 3: 9m 12s (552.11 s)

### Primes under $10^{10}$

* Run 1: 27m 12s (1632.22 s)
* Run 2: 27m 24s (1644.82 s)
* Run 3: 27m 03s (1623.51 s)


## Post-benchmark analysis

I have found that the run-time is radic, and can be approximated by the function $F$ with an $R^2$ value of $0.99$.

$$F(t) = ([1077 \cdot 10^{-7}]t)^{0.53313}$$
