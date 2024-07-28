# xNote
![xNote](https://raw.githubusercontent.com/xanmoy/xnote/main/media/Screenshot%20from%202024-07-29%2004-12-04.png)

![xNote](https://raw.githubusercontent.com/xanmoy/xnote/main/media/Screenshot%20from%202024-07-29%2004-12-18.png)

xNote is a simple text editor built with C and GTK3. It features basic text editing capabilities, such as opening and saving files.

## Features

- Open text files
- Save text files
- Simple and intuitive graphical user interface (GUI)

## Requirements

- GTK+ 3.0
- pkg-config
- gcc (GNU Compiler Collection)

## Installation

### Arch Linux

1. **Install Dependencies:**

   Make sure you have the necessary dependencies installed on your system:

```sh
   sudo pacman -Syu
   sudo pacman -S base-devel gtk3 atk glib2
```  
2. **Clone the Repository:**

```sh
git clone https://github.com/xanmoy/xnote.git
cd xnote
```
3. **Compile the Project:**

```sh
gcc xnote.c -o xnote `pkg-config --cflags --libs gtk+-3.0`
```
4. **Build the Project:**

Use the provided Makefile to build the project:

```sh
make
```
5. **Install the Application:**

Install the application to /usr/local/bin:

```sh
sudo make install
```

## Usage
Run the application from the terminal:

```sh
xnote
```

## Development
If you want to contribute or make modifications to the project, you can build it using the following commands:

Build the Project:

```sh
make
```
Clean the Build:

```sh
make clean
```

### File Structure
xnote.c: Main source code file for the text editor.
Makefile: Build instructions for the project.
## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing
Fork the repository.
Create your feature branch (git checkout -b feature/my-new-feature).
Commit your changes (git commit -am 'Add some feature').
Push to the branch (git push origin feature/my-new-feature).
Create a new Pull Request.
Acknowledgements
GTK+ - The GUI toolkit used for this project.
## Contact
If you have any questions, feel free to reach out to the project maintainer at [contact@xanmoy.me].
