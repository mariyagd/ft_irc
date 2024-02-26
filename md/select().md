With the `select()` function, a program can specify a list of descriptors to check for pending I/O; select() suspends the program until one of the descriptors in the list becomes ready to perform I/O and returns an indication of which descriptors are ready. Then the program can proceed with I/O on that descriptor with the assurance that the operation will not block.

```
int select(int maxDescPlus1, fd_set *readDescs, fd_set *writeDescs, fd_set *exceptionDescs, stnlct timeval *timeout)
```

--------
`select()` monitors three separate lists of descriptors that, typically, are implemented as vectors of bits: 
- `readDescs`: Descriptors in this vector are checked for immediate input data availability. 
- `writeDescs`: Descriptors in this vector are checked for the ability to immediately write data. 
- ` exceptionDescs`: Descriptors in this vector are checked for pending exceptions.

--------

Passing `NULL` for any of the descriptor vectors makes `select ()` ignore that type of I/O. For example, passing `NULL` for `exceptionDescs` causes `select()` to completely ignore exceptions on any sockets. 

-----

Though the maximum number of descriptors can be quite large, most applications use very few descriptors. To avoid making `select()` search all possible vector positions for all three vectors, we give it a hint by specifying in `maxDescPlus1` the maximum number of descriptor values to consider in each descriptor vector. Since descriptors begin at 0, the number of descriptors is always the maximum descriptor value plus one. For example, if descriptors 0, 3, and 5 are set in the descriptor list, the number of descriptors for `select ()` to consider is 6 (0 through 5), which is also the maximum descriptor value (5) plus one. Notice that we set `maxDescPlus1` for all three descriptor lists. If the exception descriptor list has the largest descriptor value, say, 7, then we set `maxDescPlus1` to 8, irrespective of the descriptor values set for read and write.

-------------

The last parameter (`timeout`) allows control over how long `select()` will wait for something to happen. The `timeout` is specified with a timeval data structure:

```
struct timeval 
{ 
	time_t tv_sec; 
	time_t tv_usec; 
};
```

If the time specified in the `timeval` structure elapses before any of the specified descriptors becomes ready for I/O, `select()` returns the value 0. If `timeout` is `NULL`, `select()` has no timeout bound and waits until some descriptor becomes ready. 

Setting both `tv_sec` and `tv_usec` to 0 causes `select()` to return immediately, enabling polling of I/O descriptors. 

-----------------
##### Macors:
The `fd_set` descriptor lists are manipulated by four system-provided macros:
```
FD_ZERO(fd_set *descriptorVector) 
FD_CLR(int descriptor, fd_set * descriptorVector) 
FD_SET(int descriptor, fd_set * descriptorVector) 
FD_ISSET(int descriptor, fcl_set * descriptorVector)
```

`FD_ZERO()` removes **all** descriptors from the vector. `FD_CLR()` and `FD_SET()` remove and add descriptors to the vector. Vector membership of a descriptor is tested by `FD_ISSET()`.

##### Returned value:
If no errors occur, `select()` returns the total number of descriptors prepared for I/O. To indicate the descriptors ready for I/O,` select()` changes the descriptor lists so that only the positions corresponding to ready descriptors are set. For example, if descriptors 0, 3, and 5 are set in the read descriptor list, the write and exception descriptor lists are NULL, and descriptors 0 and 5 have data available for reading, select() returns 2, and only positions 0 and 5 are set in the returned read descriptor list. An error in `select()` is indicated by a return value of - 1