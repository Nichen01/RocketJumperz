# Dead Weight
**A zero-gravity 2D action-platformer where weapon recoil is your only method of propulsion.**

Trapped aboard an alien-infested spaceship, players must master momentum-based traversal, manage limited ammunition, and fight through hazard-filled sections to reach the escape pod. Built by a 4-person student team at DigiPen Institute of Technology Singapore.

## Technical & Design Highlights
* **In-Engine Grid Level Editor:** Built a custom tile-based level editor enabling real-time placement, deletion, undo states, and rapid switching/saving across multiple levels via keyboard shortcuts (`Ctrl+S`, `Ctrl+Z`, `Ctrl+1-3`).
* **Recoil-Driven Traversal:** Designed levels around zero-gravity physics where every weapon applies distinct impulse forces, turning combat choices into directional movement puzzles.
* **Section-Based World Flow:** Structured levels into discrete *Jump King*-style camera sections connected by box-to-box collision triggers, pacing combat encounters, hazards, and loot pickups.

## Role: Level Designer (Joraye)
* **Internal Tooling & Level Pipeline:** Designed and iterated on the custom grid-based level editor, enabling the team to rapidly prototype, save, and modify level layouts without recompiling code.
* **Zero-Gravity Encounter & Puzzle Design:** Built multi-section stages that test momentum mastery, pairing environmental hazards (such as gravity wells and saw traps) with enemy placements and weapon/health pickups.

## Controls
| Action | Input |
| :--- | :--- |
| **Aim & Fire (Propel)** | Mouse Cursor + Left Click |
| **Interact / Loot** | `Spacebar` |
| **Reset Level** | In-Game UI Button |
| **Level Editor Shortcuts** | `Ctrl + Click` (Place) · `Right Click` (Clear) · `Ctrl + S` (Save) · `Ctrl + Z` (Undo) · `Ctrl + [1-3]` (Switch Level) |

## Team RocketJumperz
* **Joraye** – Level Designer
* **Nicholas** – Project Lead
* **Ivan** – Lead Programmer
* **Jeremiah** – Gameplay Programmer
