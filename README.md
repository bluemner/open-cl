# Open-Cl
Open Cl repository with sample data structures, and code that will run on an approved 
open cl device (or one that has open-cl implemented)

# Install & Running
## Step 1 - Get binary file (lib) and header files (include)
Download one of the following implementation of open-cl (AMD,Intel, Nvidia)

* AMD - [http://support.amd.com/en-us/kb-articles/Pages/OpenCL2-Driver.aspx]()
* Intel - [https://software.intel.com/en-us/articles/opencl-drivers]()
* Nvidia - [https://developer.nvidia.com/opencl]()



## Step - 2 Tell your compiler path to lib file
### Windows 7+
 environment variables called:
cl_include => ...path to include folder from download

cl_lib => ... path to lib folder from download

update build.bat with the correct path (note by default set to x64)
```batch
set cl_incldues=%cl_incldue%
set cl_libs=%cl_lib%/x64
```

### GNU Linux ( Mac? )
Update the makefile with the correct path to where your lib folder is located

## Step - 3 Compile
### Windows
Run build.bat
### GNU Linux ( Mac? )
Run make

## Step - 4 Run Binary file
### Windows
Run [file-name].exe
In powershell
```
./file-name.exe
```
### GNU Linux ( Mac? )
Run [file-name].o
In Terminal
```
./file-name.o
```
## VS Code
If your are using vscode, the configuration should be set in the ```.vscode``` folder. Listed below are some Recommended plugins for vscode , not required but which have been very helpful in implementation :) 
### Recommended plugins
* C/C++ (Microsoft)
* change-case (wmaurer)
* Code Spellchecker (Street side software)
* vscode-icons (Roberto Huertas)