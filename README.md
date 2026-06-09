# 🎨 2D Graphics Editor

A terminal-based 2D graphics editor built in C that lets you draw and manage shapes on an ASCII canvas using classic computer graphics algorithms.

---

## 📋 Features

- ✅ Draw Lines using **Bresenham's Line Algorithm**
- ✅ Draw Rectangles (border outline)
- ✅ Draw Circles using **Midpoint Circle Algorithm**
- ✅ Draw Triangles (three connected lines)
- ✅ Add, Delete, and Modify shapes by ID
- ✅ Live canvas re-rendering after every change
- ✅ Supports up to **100 shapes** at once
- ✅ Robust input validation (no crashes on bad input)
- ✅ Cross-platform (Windows & Linux/Mac)

---

## 🖥️ Canvas

- Size: **60 × 20** characters
- Shapes are drawn using `*` characters
- X-axis: 0 → 59 (horizontal)
- Y-axis: 0 → 19 (vertical)
- Coordinate grid is displayed for easy reference

---

## 🚀 How to Compile & Run

### Linux / Mac
```bash
gcc graphics_editor.c -o graphics_editor -lm
./graphics_editor
```

### Windows (GCC / MinGW)
```bash
gcc graphics_editor.c -o graphics_editor.exe -lm
graphics_editor.exe
```

---

## 🗂️ Menu Options

| Option | Description |
|--------|-------------|
| 1 | Add a Shape |
| 2 | Delete a Shape by ID |
| 3 | Modify a Shape by ID |
| 4 | Clear Canvas (delete all shapes) |
| 5 | Exit |

---

## 📐 Shapes & Parameters

| Shape | Parameters Required |
|-------|-------------------|
| Line | Start point (x1, y1) → End point (x2, y2) |
| Rectangle | Top-left (x1, y1) → Bottom-right (x2, y2) |
| Circle | Center (cx, cy) + Radius (1–20) |
| Triangle | Three vertices (x1,y1), (x2,y2), (x3,y3) |

---

## 🧠 Algorithms Used

- **Bresenham's Line Algorithm** — Efficient integer-only line drawing
- **Midpoint Circle Algorithm** — Smooth circle rendering using 8-point symmetry
- **Triangle** — Three Bresenham lines connecting three vertices

---

## 📁 Project Structure

```
2d-graphics-editor/
│
├── graphics_editor.c    # Main source file (all code)
└── README.md            # Project documentation
```

---

## 💡 Example

```
=== 2D GRAPHICS EDITOR ===

   0         1         2         3         4         5
   0123456789012345678901234567890123456789012345678901234567890
  +------------------------------------------------------------+
 0|____________________________________________________________|
 1|____*_______________________________________________________| 
 2|___***______________________________________________________| 
 3|__*****_____________________________________________________| 
  +------------------------------------------------------------+

 ID 1: Triangle (5,0)-(1,4)-(9,4)
```

---

## 👨‍💻 Author

Made as a C mini project for ACP (Advanced C Programming).

---

## 📄 License

This project is open source and free to use.
