# Zadanie rekrutacyjne AGH Space Systems – Biblioteka TLV

Projekt zawiera pełną implementację biblioteki obsługującej protokół **TLV (Type-Length-Value)** przez interfejs **UART** dla mikrokontrolera z rodziny STM32. Kod został napisany w języku C z wykorzystaniem biblioteki HAL i zawiera zabezpieczenia przed błędnymi danymi oraz przepełnieniem buforów.

Projekt został wygenerowany w programie **STM32CubeMX** jako generator **CMake** i jest przystosowany do pracy w środowisku **Visual Studio Code**.

---

## Wymagania wstępne

Do poprawnej kompilacji projektu wymagane są:

1. **Visual Studio Code** z zainstalowanymi rozszerzeniami:
   - *C/C++* (Microsoft)
   - *CMake Tools* (Microsoft)
2. **ARM GNU Toolchain** (`arm-none-eabi-gcc`) – zainstalowany i dodany do zmiennej środowiskowej `PATH`.
3. **CMake** – dodany do zmiennej środowiskowej `PATH`.
4. **Ninja** (system budowania) – plik `ninja.exe` dodany do zmiennej środowiskowej `PATH` (np. umieszczony w katalogu `bin` narzędzia ARM Toolchain).

---

## Instrukcja kompilacji

### Metoda 1: Visual Studio Code (Zalecana)

1. Otwórz główny folder projektu w **Visual Studio Code**.
2. Rozszerzenie *CMake Tools* automatycznie wykryje plik `CMakeLists.txt` i poprosi o wybór zestawu narzędzi (Kit). Wybierz:
   - **`GCC for arm-none-eabi`**
3. Skompiluj projekt na jeden ze sposobów:
   - Wciśnij klawisz **`F7`**, lub
   - Kliknij przycisk **Build** na dolnym pasku stanu VS Code.
4. Pliki wynikowe (`.elf` oraz `.bin`) zostaną wygenerowane w folderze `build/`.

---

### Metoda 2: Terminal / Wiersz poleceń (CLI)

Aby skompilować projekt ręcznie z poziomu terminala, otwórz wiersz poleceń w głównym folderze projektu i wykonaj:

```bash
mkdir build
cd build
cmake -G Ninja "-DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake" ..
cmake --build .
