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

### For All Configurations add the following changes.

Add the entire `SFML-2.5.1\include` directory path to `C/C++ -> General -> Additional Include Directories` and the entire `SFML-2.5.1\lib` directory path to `Linker -> General -> Additional Library Directories`.

Add `_CRT_SECURE_NO_WARNINGS` to `C/C++ -> Preprocessor -> Preprocessor Definitions`.

### For the Release configuration add the following changes.

Append the following to the beginning of `Linker -> Input -> Additional Dependencies`.

`opengl32.lib;freetype.lib;winmm.lib;gdi32.lib;sfml-graphics-s.lib;sfml-window-s.lib;sfml-system-s.lib;`

### For the Debug configuration add the following changes.

Append the following to the beginning of `Linker -> Input -> Additional Dependencies`.

`opengl32.lib;freetype.lib;winmm.lib;gdi32.lib;sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;`
