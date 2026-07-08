# Grand Terminal Potato 0.6 🥔

An independent, custom-built 3D rendering engine written entirely from scratch in C++. This project renders a fully real-time 3D viewport using ASCII/text characters running purely inside the Windows Command Prompt. 

> **Disclaimer:** This is a non-commercial, open-source technical showcase and parody project built entirely for educational and portfolio purposes.

---
 
![Grand Terminal Potato 0.6 Trailer](media/gtp_0.6_trailer.gif)
 
---

## System Requirements & Compatibility

* **Platform:** Windows Only.
* **IDE:** Compile with **Visual Studio**.

---

## Critical First-Time Setup Instructions

Because this engine renders high-resolution 3D environments using standard console characters, you **must** manually adjust your terminal's buffer and display scaling upon launch to see the image correctly:

1. **Launch the Project:** Compile and run the solution inside Visual Studio.
2. **Go Fullscreen:** As soon as the command prompt window opens, press `Alt + Enter` to make the terminal completely fullscreen.
3. **Adjust the Scaling (Crucial):** Hold down `Ctrl` and **scroll down with your mouse wheel (middle mouse roll)** to zoom out. Keep zooming out until the text font shrinks completely, revealing a crisp, clear 3D rendering of the world.

---

## Controls

Navigate through the 3D space using your keyboard. The engine supports full 3D translation and horizontal rotation tracking:

| Key | Action |
| :---: | :--- |
| **W** | Move Forward |
| **S** | Move Backward |
| **A** | Strafe Left |
| **D** | Strafe Right |
| **Q** | Move Up (Ascend) |
| **E** | Move Down (Descend) |
| **J** | Look/Turn Left |
| **L** | Look/Turn Right |

---

This project bypasses modern graphics APIs (like DirectX or OpenGL) to demonstrate the fundamentals of computer graphics pipeline mathematics entirely on the CPU

*Author: T. Selçuk Yüksel*
