# README - Sleeping Teaching Assistant Program

### Team Information

| Name                  | Section # | Email Address               |
|-----------------------|-----------|-----------------------------|
| Christopher Contreras | 351-02    | cdc17507@csu.fullerton.edu  |
| Alexie Gonzalez       | 351-02    | alexieg3@csu.fullerton.edu  |
| Britney Lopez         | 351-02    | bnlopez2@csu.fullerton.edu  |

---

### Program Overview

This program simulates a scenario where students seek help from a teaching assistant (TA) during office hours. The TA assists one student at a time, with limited seating for waiting students. If no seats are available, students return later.

The program is  in C/C++ and uses POSIX threads, semaphores, and mutex locks to make sure for proper synchronization between the TA and students.

---

### How to Run the Program

1. **Platform**:  
   - This program is designed to run on **Linux** using the **G++ compiler**.

2. **Compilation**:
   - Open a terminal in the directory containing the program files.
   - Compile the program using the following command:
     ```bash
     g++ main.cpp -o main
     ./main
     ```
     
3. **Testing**:
   - The program outputs the sequence of events as students arrive, sit, get help, or leave.
   - **Example Output**:
     ```
     Number of Students specified. Creating 8 threads.
     Student 0 needs help.
     Student 0 is sitting on chair 0. Waiting for the TA.
     TA is helping a student.
     Student 1 needs help.
     Student 1 is sitting on chair 1. Waiting for the TA.
     ...
     ```

4. **Screenshot**:
      
   <img length= "500" width="400" alt="output351" src="https://github.com/user-attachments/assets/1264d712-d348-4709-b936-3c1c48aabebd">

---

### Team Collaboration and Contributions

Each team member was responsible for implementing specific sections of the code, and we used regular meetings to discuss our progress and integrate the sections into a cohesive program.

- **Christopher Contreras**:  
  - **Function**: `TA_Activity`
  - **Responsibilities**: Implemented the TA's behavior, including sleeping, helping students, and managing the waiting queue.
  - **Details**: Christopher wrote the logic for the TA’s sleeping and waking up process, as well as handling the waiting chairs and notifying students.

- **Alexie Gonzalez**:  
  - **Function**: `Student_Activity`
  - **Responsibilities**: Defined the behavior of each student, including checking the TA’s status, waiting if necessary, and handling re-attempts if no chairs are available.
  - **Details**: Alexie implemented the student actions for requesting help, waking up the TA, and managing the waiting process.

- **Britney Lopez**:  
  - **Function**: `main`
  - **Responsibilities**: Set up the program, including initializing semaphores and mutexes, creating threads for the TA and students, and managing program execution.
  - **Details**: Britney handled thread creation, joining, and cleanup processes, ensuring synchronization and resource deallocation upon program exit.

### Collaboration Process

Our team coordinated efforts by:
- Holding regular online meetings to discuss each function’s design, confirm requirements, and check progress.
- Using shared online documentation to track each member’s updates and outline integration steps.
- Testing code segments individually and then integrating them into a final program, where we conducted group testing and resolved any issues.

Each team member contributed equally by focusing on their assigned section, and we worked together to troubleshoot synchronization and compilation issues.

The program is  in C/C++ and uses POSIX threads, semaphores, and mutex locks to make sure for proper synchronization between the TA and students.

## Disclaimer

I am not maintaining this code; it is just for learning and educational purposes.
