# create

### Requirements
- pip3 install selenium
- make
- create a secrets.py file with `GitHub_Username = 'username'` and `GitHub_Password = 'password'`
- VSCode

### Description 
```Usage: ./create project_path_and_name [-r ReadMe Description]```

This program is used to set up a C coding project with 1 command.  
It sets up all the local files, and a github repo

when run, it:
- Creates a project folder at the path provided
- Initializes a git repo in it
- Creates a readme file with the name of the project as the title
- Create C source file with the project name plus ".c"
- Creates an empty .gitignore
- Creates a makefile with a rule to compile the source file.
- Opens the project directory with VSCode
- Opens up a Google Chrome browser window, logs into github with the credentials provided in the secrets.py,  
creates the new repo in github, copies the git remote, closes the window, and add the git remote to the local project repo

With the `-r` flag, text can be added to the readme file

## Notes
To be ran from path, and Linux-specific bit of code is used. So not sure whether it will run on MacOS, and won't work on Windows (unless used with WSL, which I did).  
Also, selenium (which is the python module that opens up the browser), could not get chromedriver to work on it's own, so I had to download a chromedriver.exe executable from [here](https://chromedriver.chromium.org/downloads), place it in my downloads folder, and run it from there.  
IT WON'T WORK UNLESS YOU DO THE SAME, or change the code at the python script (this line `browser = webdriver.Chrome("/mnt/c/Users/Stathis Kapnidis/Downloads/chromedriver.exe", options=chrome_options)`) so that it runs on your machine.  