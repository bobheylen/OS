# OS
## clab1gcc
In this first practical session, you will learn the basics of programming with C.
You will have to write basic C programs, build them with gcc, run and test the programs.
Additionally, you will have to commit and push your code in a git repo.
Finally, we also practice some APIs in C to use and manipulate strings.
## clab2pointers
The second lab on programming with C contains exercises related to pointers in C, parameter passing to functions, and understanding the function stack in C.
You should be familiar with the contents of the slides of Chapter 1 and 2, as well as with homework 1 and 2 before starting this practical lab.  We created a branch clab2pointers in your osc gitrepo on gitlab.groept.be.
You can use this branch to store evolving versions of your code in a safe and stable place while developing.
## clab3lists
This is the assignment document for lab 3 of the practical labs on C.
It contains 2 exercises on linked list and the description of milestone 1 for your final project.



###################### ASSIGNMENT milestone1.zip #####################

To prepare for this assignment you should study Chapter 1 and Chapter 2 of the slides, especially the slides on the introduction to C and data structures (Chapter 1) as well as the slides on development tools (chapter 2).
We have updated the slides and demo code from the lectures on Toledo with the final versions.
We also assume you have completed the two homework assignments as well as clab1 and clab2.

Start by reading the assignment carefully, and make sure you understand all the hints and information provided.
You can get additional support during the lab sessions. Do not hold back on unit testing your implementation.
We assess both your list implementation as well as your testing strategy.

Do not worry about submitting an incomplete implementation or a buggy implementation with some tests still failing.
We do not expect perfect solutions and will reward any descent attempt with the promised bonus point.

Note that we will apply plagiarism tools to detect copycats.

The deadline for submission is Monday November 7th 2022, 23:59. 
## plab1datamgr
This next lab kicks off the project lab series with an introduction to the project overview and an exercise on file I/O.
You can find the skeleton code to get you started in your git repo in branch 'plab1datamgr' and in the zip file associated with this document item on Toledo. 
## plab2sensordb
This is a copy of project lab assignment 2, including milestone 2.
The document and source code here in the course documents are  duplicates for archival purposes of Milestone 2 in Assignments.



###################### ASSIGNMENT milestone2.zip #####################

This week you will be working on the second milestone of your project assignments.
This is a stand-alone milestone that does not depend on any previous assignments, projects, or code you wrote.
So don’t worry if your linked list is not fully functional yet.
You can just focus on this second milestone to implement two key building blocks of your final project: the storage manager, which stores sensor measurements in a csv file, and the logger, a child process responsible for logging all events.

You can find the assignment ‘Milestone 2’ on Toledo under “Assignments”.
The assignment contains two exercises as a warm-up, and the final exercise is the second milestone of the project you need to submit as milestone2.zip.
Make sure that you create this zip using the zip build target of the Makefile that comes with the skeleton code in the folder “milestone2”.
Otherwise, our testing scripts can’t process your solution.

To prepare for this assignment you should have studied the lectures on processes (3a), inter-process communication (3b), and file systems.).

Start by reading the assignment carefully, and make sure you understand all the hints and information provided.
You can get additional support during the lab sessions.
Do not hold back on testing your implementation against erroneous function calls or out of order function calls.
We assess your implementation also based on defensive programming against faulty input.

Do not worry about submitting an incomplete implementation or a buggy implementation with some parts still failing.
We do not expect perfect solutions and will reward any descent attempt with the promised bonus point.

Note that we will apply plagiarism tools to detect copycats.

The deadline for submission is Monday November 21st 2022, 23:59:59.
## plab3connmgr
This is the third step of the project labs.
Here we will start building the basis of our server to process temperature readings of multiple client-side sensor nodes.
The source code provided in the zip file also contains a handy make file to build and run the server and client.
## plab4sbuffer


###################### ASSIGNMENT milestone3.zip #####################

The next assignment is an exercise on a variant of the consumer-producer problem with a shared buffer.
To complete this assignment you need to understand the concepts from the lectures on multi-threading, synchronization basics and synchronization examples.
This includes threads, semaphores, mutexes, condition variables, programming support in pthreads, as well as the example problems such as the readers-writers problem and the consumer-producer problem.

The deadline for submission is Monday December 5th 2022, 23:59:59.
## plab5finalproject


###################### ASSIGNMENT finalproject.zip #####################

This is the final assignment for the course on operating systems.
It is part of your exam and counts for 50% of your final grade.
In this final assignment you will develop a server that collects temperature measurements from different client nodes in parallel, and then processes them and stores them.
You will need to use code snippets from your previous milestones and project labs that you implemented during the practical labs to integrate into a final solution.
You will also need to understand the concepts we discussed during the lectures: processes, file I/O, threads, fork, pipe, as well as synchronization tools such as mutexes, condition variables and semaphores.
You will also need to apply the development tools we practiced (gcc, make, etc) to build the required executables and libraries.
Make sure you backup your solution frequently using git (commit and push). Laptops have the tendency to act up and crash when deadlines approach.

Read the assignment carefully and make sure you understand the required deliverables and provided hints and suggestions.
The deadline for submission on Toledo for this final assignment is December 23th 23:59:59.
