# C-Project---Wordgame

Description
This project implements a C program that performs various tasks such as:

Loading place names from a file into an array.
Converting strings to lowercase.
Generating random letters, excluding the letter 'x'.

The code is designed to handle a maximum of 1000 places with a defined character length for place names and player names. This project could be useful in scenarios where you need to work with lists of places and perform string manipulations or random selections.

**Features**

Random Letter Generator: Generates random lowercase letters, excluding 'x'.
String Manipulation: Converts strings to lowercase for uniformity.
File Handling: Loads a list of places from a text file into an array for further processing.

**Requirements**
C Compiler 
A text file containing place names for the program to load.

**Installation**
Clone the repository:
git clone https://github.com/arunmp1/C-Project---Wordgame.git

Navigate to the project directory:
cd projectname

Compile the code:
gcc -o place_loader c_updated_features.c

Run the program (make sure to have a file with place names in the same directory):
./place_loader places.txt

**Usage**
File Input: The program expects a text file (places.txt) with each place name on a new line.
Random Letter Generation: The program will generate random letters, skipping the letter 'x'.
Lowercase Conversion: Any string processed will be converted to lowercase.

Example
Input file (places.txt):

New York
Los Angeles
Chicago
Houston
Phoenix

The program will load these places and process them according to the functions defined in the code.

Contributing
If you would like to contribute to this project:

Fork the repository.
Create a new branch (git checkout -b feature-branch).
Make your changes and commit them (git commit -am 'Add new feature').
Push the branch (git push origin feature-branch).
Create a Pull Request.
