# GameTrafficOptimization 
The project is not finished yet.

Testing TCP, SCTP protocols, ROHC header compression, LZ4, Huffman payload compression with cocos2d-x. 

Made for masters thesis.

An application that supposed to work on different platforms.

Used for testing different algorithms:

Transport protofol header compression: ROHC
Payload that is mostly used in online games compression: LZ4, Huffman, Simd
And also to test couple of transport protocols:
TCP
SCTP

Library list:

1. TCP - ported from 39dll (gamemaker lib) : http://www.lukeescude.com/online-tutorial/
2. ROHC - https://rohc-lib.org/ //Note: ios doesn't allow to create socket without root access. So sending raw packet would be possible only on jailbroken devices.
3. Huffman - https://github.com/crookedbard/Working-Huffman
4. Lz4 - ... in code
5. Simd - ?
6. Sctp - ?

Progress:

1. Control panel sciene (in which tests will be performed) - Done.
2. TCP - Done, with some minor issues. some functions doesn't work properly on unix systems: setasync(), setformat().
3. Huffman - Done, but there should be a posibility for some improvements.
4. Lz4 - Done, but there should be a posibility for some improvements.
5. ROHC - ios doesn't allow to create socket without root access. So sending raw packet would be possible only on jailbroken devices.
6. Added possibility to print debug info not in console but on cocos2dx scene. 

To do:

1. All the different libraries needs to be ported to different platforms. 
2. Prepare payload compression tests: cycles per time, compress ratio based on different game payload type.
3. SCTP tests ??
4. ROHC tests ??
5. Sctp
6. Simd 
