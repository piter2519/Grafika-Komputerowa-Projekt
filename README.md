# 🎮 Projekt: Grafika Komputerowa - Scena 3D w OpenGL

Projekt zrealizowany w ramach czwartego semestru (Grafika Komputerowa). Jest to aplikacja renderująca scenę 3D w czasie rzeczywistym, napisana w **C++** z użyciem **OpenGL**.

## ✨ Główne funkcjonalności
- Renderowanie modeli 3D z użyciem biblioteki `tiny_obj_loader`.
- Obsługa zaawansowanych materiałów PBR.
- Swobodna kamera umożliwiająca wędrowanie po scenie (`Camera.h`).
- Implementacja własnych shaderów napisanych w GLSL (`default.vert`, `default.frag`).
- Wczytywanie i zarządzanie teksturami dla rożnych powierzchni (np. sufit, podłoga, ściana) z wykorzystaniem biblioteki `stb_image`.

## 🛠️ Technologie i Biblioteki
- **C++17** (lub nowszy)
- **OpenGL** (z loaderem **GLAD**)
- **GLFW** (zakładany menedżer okien renderowania)
- [stb_image](https://github.com/nothings/stb) (wczytywanie tekstur)
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) (wczytywanie plików `.obj`)

## 🚀 Jak uruchomić?
1. Sklonuj to repozytorium do wybranego folderu.
   ```bash
   git clone https://github.com/piter2519/Grafika-Komputerowa-Projekt
   ```
2. Otwórz projekt w środowisku **Visual Studio** dwukrotnie klikając na plik `lab6.sln`.
3. Upewnij się, że platforma kompilacji ustawiona jest pod system docelowy (często **x64**).
4. Skompiluj i uruchom aplikację (*Local Windows Debugger* / F5).

## 🗂️ Struktura projektu
- `/res/` – zasoby niezbędne do wyświetlenia grafiki (modele w formacie obj/mtl, shadery gsls oraz tekstury jako pliki obrazów).
- `Main.cpp` – punkt startowy aplikacji, ustawienie silnika.
- Kod pomocniczy do zarządzania potokiem renderowania, obiektami buforów (VAO, VBO, EBO) oraz shaderami.
