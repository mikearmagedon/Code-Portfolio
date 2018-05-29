This project deals with file encryption using the ChaCha20(https://tools.ietf.org/html/rfc7539) algorithm. 

The goal was to use the algorithm in a multi-threaded application to study parallelism.

Basically, for each file to be encrypted there is a consumer thread (for reading the plaintext file) and a corresponding producer thread (for writing the ciphertext file).
Regardless of the number of files there's the processing thread that applies the algorithm to convert the plaintext into ciphertext.
The threads communicate using queues and signals and were implemented using the POSIX threads API.