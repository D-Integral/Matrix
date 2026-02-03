This program demonstrates matrix creation, manipulation, and analysis using multi-threading in C++.

- Dynamically allocates a matrix of integers.
- Allows the user to fill the matrix manually or generate random values.
- Computes per-row and per-column statistics:
  1) Average value (Middle)
  2) Count of elements below average (Less)
  3) Count of elements above average (More)
- Uses POSIX threads (pthread) to perform row and column calculations concurrently.
- Includes mutex locks to manage safe concurrent access to the matrix.

By doing this task I passed the course "Parallel and Distributed Computing" at Kyiv Polytechnic Institute in 2013 with the grade 100/100.
