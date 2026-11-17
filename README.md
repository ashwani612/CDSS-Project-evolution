# Clinical Decision Support System (CDSS)
## Evolution 1 – Basic 

### 1. Project Overview

The Clinical Decision Support System (CDSS) – Evolution 1 is a beginner-friendly, menu-driven console application developed using C++.

The main purpose of this project is to demonstrate the basic concept of a Clinical Decision Support System. It provides a simple interface for patient registration and displays basic educational information related to common symptoms such as fever, cough, and headache.

This version represents the initial development stage of the CDSS project. It focuses on understanding the basic system structure, menu-driven programming, user input handling, and conditional logic using C++.

### 2. Project Objectives

The main objectives of Evolution 1 are:

- To develop a basic Clinical Decision Support System using C++.
- To create a simple menu-driven console interface.
- To allow users to enter basic patient information.
- To provide educational information about selected common symptoms.
- To implement basic input validation.
- To establish a foundation for future improvements in the CDSS project.

### 3. Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Main programming language |
| C++ Standard Library | Input/output and basic program operations |
| GCC / MinGW | Compiling and running the program |
| VS Code | Code editing and development environment |

### 4. Implemented Features

#### A. Menu-Driven Interface
The application provides a simple menu through which users can select different available operations.

#### B. Patient Registration
Users can enter basic patient information through the console.

Patient information is stored temporarily during program execution. It is not permanently saved after the application is closed.

#### C. Educational Symptom Information
The system provides basic educational information about the following common symptoms:

- Fever
- Cough
- Headache

This feature demonstrates how a basic rule-based system can present symptom-related information.

#### D. Input Validation
The application includes basic validation for menu selection to handle invalid menu inputs and improve user interaction.

### 5. How the System Works

The basic workflow of Evolution 1 is:

1. The user starts the application.
2. The system displays the main menu.
3. The user selects an available operation.
4. The user can enter patient information or access educational symptom information.
5. The system processes the selected operation.
6. The application continues running until the user selects the exit option.

### 6. Project Structure

```text
CDSS-Project-Evolution-1/
│
├── src/
│   └── main.cpp
│
├── README.md
│
└── .gitignore
```

**File Description:**

- `src/main.cpp` – Contains the complete C++ source code and program logic.
- `README.md` – Contains project documentation and instructions.
- `.gitignore` – Specifies files that should not be tracked by Git.

### 7. System Requirements

To run this project, the following software is required:

- Windows, Linux, or macOS.
- C++17 compatible compiler.
- GCC, MinGW-w64, or MSVC.
- Visual Studio Code or another C++ code editor (optional).

### 8. Installation and Execution

#### Step 1: Clone the Repository

```bash
git clone https://github.com/ashwani612/CDSS-Project-evolution-1.git
```

#### Step 2: Open the Project Folder

Open the downloaded project folder in Visual Studio Code or another code editor.

#### Step 3: Compile the Program

Using GCC or MinGW:

```bash
g++ -std=c++17 src/main.cpp -o CDSS_Evolution1.exe
```

#### Step 4: Run the Program

On Windows PowerShell:

```powershell
.\CDSS_Evolution1.exe
```

On Linux or macOS:

```bash
./CDSS_Evolution1
```

### 9. Current Limitations

Evolution 1 is an initial prototype and has the following limitations:

- Patient information is stored only during the current program session.
- Patient records are not permanently stored.
- Only selected common symptoms are covered.
- The system does not use a database.
- The application has a console-based interface.
- It does not provide clinically validated diagnosis or treatment recommendations.

### 10. Future Scope

The project can be improved in future versions by introducing:

- Permanent patient record storage.
- Patient search, update, and delete functionality.
- Improved input validation.
- Database integration.
- A graphical web-based user interface.
- More comprehensive educational decision-support features.

These improvements can be implemented progressively in the next evolutions of the project.

### 11. Disclaimer

This project is developed strictly for educational and demonstration purposes.

It is not a clinically validated medical device and must not be used for actual medical diagnosis, treatment decisions, or emergency medical situations.

All demonstrations should use fictional patient information.

### 12. Author

**Project:** Clinical Decision Support System (CDSS)  
**Version:** Evolution 1  
**Programming Language:** C++  
**Development Stage:** Basic Prototype
