4.  char* cp;
    char** ap;
    int** pp;

    char c;
    char x[10];
    int* d; 
    cp = &c;
    ap = &x;
    pp = &d;

    Once initializing the values we find that the pointers now contain values equal 
    to the address locations of the local variables that they were assigned to. 

5. You could theoretically point a pointer to itself like this.
    
    void* p = &p; 

6. The fundamental problem with the code lies in the for loop
    
    for(p = s; p != '\0'; p++)
    
    should instead use *p took look at the character p is pointing at to 
    see it is the null terminator. 

    so for(p = s; *p != '\0': p++)

7. A better statement would be that array act as pointers. The fundamental
difference between arrays and pointers is that the value that the array points
at cannot be changed. where as a pointer may change the address points to. 
An array points to the first element and may be incremented further but the
starting position cannot change.

