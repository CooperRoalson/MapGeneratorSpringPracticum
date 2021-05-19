# MapGeneratorSpringPracticum

![Title Banner](Banner.png)

This is a map generator project, completed as part of LASA's Advanced Computer Science course.

The [SFML Graphics Library](https://www.sfml-dev.org/download.php) is required. Originally compiled using version 2.5.1.

The contributors are:

* Ben Woytek
* [Cooper Roalson](https://github.com/CooperRoalson)
* [Hollister Ream](https://hollikill.net)
* Nathan Williams
* Walter Auby

## Building in Visual Studio 2019

Make all modifications in the `Project -> Properties` window.
You can either statically link the libraries or use DLLs.

### For All Configurations add the following changes.

Add the entire `SFML-2.5.1\include` directory path to `C/C++ -> General -> Additional Include Directories` and the entire `SFML-2.5.1\lib` directory path to `Linker -> General -> Additional Library Directories`.

Add `_CRT_SECURE_NO_WARNINGS` to `C/C++ -> Preprocessor -> Preprocessor Definitions`. This is not part of SFML itself, but rather to ignore unneccessary warnings in our code.
If you're using the static libraries, add `SFML_STATIC` as well.

### For the Release configuration add the following changes.

Append the following to the beginning of `Linker -> Input -> Additional Dependencies`.

`opengl32.lib;freetype.lib;winmm.lib;gdi32.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;`
If you're using the static libraries, the sfml libraries should have `-s` appended to look like `sfml-graphics-s.lib` instead.

### For the Debug configuration add the following changes.

Append the following to the beginning of `Linker -> Input -> Additional Dependencies`.

`opengl32.lib;freetype.lib;winmm.lib;gdi32.lib;sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;`
If you're using the static libraries, the sfml libraries should have `-s` appended before the `-d` to look like `sfml-graphics-s-d.lib` instead.

### Compiling and Running

If you're not using the static version, the SFML DLL files must be placed in the same directory as the resulting EXE file.
