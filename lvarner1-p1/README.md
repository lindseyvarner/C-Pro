Colsort Implementation:

The file begins with structs for line and list. A list represents the input file as a whole, whereas the line represents each piece of data separated by a new line character. 

Next, there are several functions for managing file input. The init function takes a list struct and initializes (mallocs space for) a new list with the given size and capacity parameters.

The resize function takes a list struct and mallocs space to create a new list with double the capacity of the input list.

The add function takes a list struct and a char*. If the size of the input list is at capacity, it calls resize. It then adds the data from the char* to a new line in the list.

The keyword function takes a line struct and an integer value for the desired keyword. It keeps track of the word count and the desired keyword to account for cases in which the desired keyword exceed the line's word count. This function traverses its input line, skipping additional spaces and incrementing count when a new character occurs after a space. It provides us with the key we will use for the compare function.

The compare function takes two line structs and calls strcmp() on each key for each input line.

The cleanup function takes a list struct and frees the memory occupied by the input list.

The open_file function takes an integer value and a char*. It creates a FILE* that is then assigned to the opened file of the input char* value. It initializes a new list and mallocs space for a temporary list. This temporary list is used to hold a copy of the data as we traverse a line and check for the new line character. If this character is not present at the maximum line length, we get a 'line too long' error. Next, the keyword function is called on the specified line and column, the file is closed, and the list is returned. 

The drive_sort function accounts for command line input errors. It checks for inputs with too many arguments but, for the sake of code length, does not check for improper argument types. It allows for an optional column argument. We create a list struct and assign it the value of the open_file function call. We then pass this list's parameters into the sort function along with the compare function. 

Finally, we print the sorted list and call cleanup() to free the memory occupied by the list.