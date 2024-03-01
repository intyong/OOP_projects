 First, when the program is executed, it prints some options with "- Main Menu -", and users can choice the option.

 1) If users insert the "1. Insertion", users can insert student's informations (name, student ID, department, birth year, tel).
 In this case, the program search all contents, and count duplicate informations with student ID.
 If the count >= 1, the duplicate information is existing and program doesn't store these informations and prints error message.
 Else, the program stores inserted informations to file1.txt.
 But when it stores one member, it has to sort all the informations by name (alphabetically).
 So, it uses vector struct, load all students' informations, and stores it to another struct (temporalily) and stores it to vector.
 And sorts by name using function (bool SortByName(Student x, Student y))and stores them in file1.txt again.

 2) If users insert the "2. Search", users can select searching options (1. name, 2. student ID, 3. admission year, 4. department name, 5. show all of the lists).
 
 2-1) The users can insert the Name that they want to search. And program explores all the contents of the file and output matching values.
 2-2) Same as above, but searching by Student ID.
 2-3) Same as above, but searching by Admission year.
 2-4) Same as above, but searching by Department.
 2-5) just print all contents in the file1.txt.

 3) If users insert the "3. Sorting Option", users can select searching options (1. name, 2. student ID, 3. birth year, 4. department name).
 
 3-1) It uses vector struct, load all students' informations, and stores it to another struct (temporalily) and stores it to vector.
 And sorts by name using function (bool SortByName(Student x, Student y)) and stores them in file1.txt again.
 3-2) Same as above, but sorting by student ID using function (bool SortBySID(Student x, Student y)).
 3-3) Same as above, but sorting by admission year using function (bool SortByByear(Student x, Student y)).
 3-4) Same as above, but sorting by department name using function (bool SortByDept(Student x, Student y)).

 4) If users insert the "4. Exit", we have to set sorting option again because default sorting option is Sorted by name.
 And finally, program is ended with return 0.