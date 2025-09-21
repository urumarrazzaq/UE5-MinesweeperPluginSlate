# 🎮 UE5 Minesweeper Plugin (Slate)

A fun and interactive **Minesweeper mini-game** implemented as a **plugin for Unreal Engine 5**, leveraging the **Slate UI framework**.  
This plugin demonstrates custom editor tools, interactive Slate widgets, and game logic integration inside the Unreal Editor.

---

## ✨ Features

- 🪟 **Editor Integration**  
  - Launchable via **Window → Minesweeper Tool** in the UE5 editor:contentReference[oaicite:0]{index=0}  
  - Toolbar button with a custom Minesweeper icon:contentReference[oaicite:1]{index=1}

- 🎨 **Slate UI**  
  - Custom widget (`SMinesweeperWidget`) for rendering the grid:contentReference[oaicite:2]{index=2}  
  - Scrollable, DPI-aware grid with adjustable layout  
  - Real-time UI warnings for excessive bomb counts

- 🧩 **Gameplay Logic**  
  - Custom game logic (`FMinesweeperGameLogic`) handling grid setup, bomb placement, adjacency, and flood-fill reveal:contentReference[oaicite:3]{index=3}  
  - Adjustable width, height, and bomb count via UI spin boxes:contentReference[oaicite:4]{index=4}  
  - Classic reveal, adjacency numbers, and bomb explosion handling  

- 💥 **Game Over & Warnings**  
  - Game over dialog when hitting a bomb:contentReference[oaicite:5]{index=5}  
  - Soft warning if bombs exceed 20% of the grid:contentReference[oaicite:6]{index=6}

---

## 📸 Screenshots
<img width="1754" height="974" alt="image" src="https://github.com/user-attachments/assets/08991066-64b7-4836-9445-3d96f5cc5af5" />
<img width="902" height="736" alt="image" src="https://github.com/user-attachments/assets/89ad2b16-0a28-4fc1-85e6-9964a192f1b5" />

---

## 🚀 Installation

1. Clone or download this repository.  
2. Place the plugin folder inside your project’s `Plugins/` directory.  
