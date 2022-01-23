Class Activities: 
1. Why is it important to always use the sizeof() operator when allocating space for a structure? 

The reason you should always use the sizeof() operator when allocating space for a structure is because structures contain
padding so if you were to just calculate the size of your struct and not use sizeof when inserting your structure into the allocated space you 
may be taking more memory than allocated and could therefore could be accessing memory you shouldnt be and that would cause a segfault.
 
 
 
2. The stdio function getchar() reads a character from stdin and, on success, returns it. On failure 
getchar() returns EOF which is defined to be -1. 
In spite of the fact that it’s reading chars, getchar() returns an int. Why? 
 
 getchar() returns an int because characters are stored as an integer value representing characters through the ascii table
 for example the character lower case a is equal to the integer 97. 
 
 
 
 
3. Write any code that causes a segfault and explain (!!) why a segfault was caused. 
 
 
 int main()
 {
     int *p = 0;
     *p = 150;
     return 1;
 }
 This code segment would set a pointer to address location 0 then set that address location to a value of 150 however this would result in a segmentation fault.
 This is due to the fact that low address locations like 0 are reserved for certain bits of information and cannot be accessed, whether that be the operating system, or your
 programs own code, you should not be able to change this information so it responds by segfaulting for accessing memory you do not have access to. 
 
 
 
 
 
4. This week you have learned about the system calls brk and sbrk.  
malloc is a library function. How do you think malloc works?  
What is the difference between a system call and a library function? 
 
 
 Malloc probably works by using sbrk to find and increment the program break so as to allocate the amount of space requested by the malloc call. 
 As we know from the lab it may also hold its own information and make chunks dependent on pages but at a basic level it increments the program break and returns
 the starting location of our allocated space. 

 The difference between system calls and library functions is in how they are defined. A library function, as its name states, is defined in a library
 and can be included into your program by using a header file. A system call acts as a macro and is defined by your system therefore lies somewhere your operating system has
 access to. System calls allow for the user to run the function without any includes and works quickly because it is predefined on your computer and unlike a library calls 
 avoid a check to where the header file is to see certain function prototypes. 
 
 
 
 
 