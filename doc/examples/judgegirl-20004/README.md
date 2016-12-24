## Expereiment ##

### Cache-Oblivious Cartesian Tree / RMQ ###

Make quadruple compression memory usage, and improve 10% ~ 25% performance.

### Build ###

```bash
make
```

### Test ###

```bash
perf stat --repeat 5 -e cache-misses,cache-references ./cormq >/dev/null
perf stat --repeat 5 -e cache-misses,cache-references ./rmq >/dev/null
```

### Result ###

```bash
 Performance counter stats for './rmq' (5 runs):

       134,873,833      cache-misses              #   62.031 % of all cache refs      ( +-  6.88% )
       217,429,736      cache-references                                              ( +-  3.72% )

       3.036004062 seconds time elapsed                                          ( +-  1.34% )
```

```bash
 Performance counter stats for './cormq' (5 runs):

        82,093,350      cache-misses              #   56.142 % of all cache refs      ( +-  4.45% )
       146,225,629      cache-references                                              ( +-  0.67% )

       2.171397899 seconds time elapsed                                          ( +-  0.44% )
```

|Method                | Time   | Space |
|----------------------|--------|-------|
|parallel-rmq          | 4116 ms| 5.6 MB|
|parallel-cartesian-rmq| 2594 ms| 3.5 MB|

### Note ###

如果使用索引方式降低空間用量 (如前綴和後綴最大值)，反而會增加時間。
