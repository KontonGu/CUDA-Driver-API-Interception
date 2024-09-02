## CUDA-Driver-API-Interception

### 1. Display the CUDA Driver API calls by a CUDA Program or other pragram using cuda
- build
    ```
    $ mkdir build
    $ cd src
    $ make libcuhook_api_show.so.1
    ```
    The generated cuda hook dynamic library to show the API calls will built to the folder `../build`;

- run
    ```
    $ LD_PRELOAD=<path_to_the_libcuhook_api_show.so.1> <your_program>
    ```
    ex.
    ```
    $ LD_PRELOAD=./CUDA-Driver-API-Interception/build/libcuhook_api_show.so.1 ./vector_add
    ```
