# 🛰️ ZAPPY

ZAPPY is a multiplayer real-time strategy game where several teams compete on a toroidal tile-based map to elevate their players through resource collection, coordination, and survival. The first team to have six players reach the maximum level wins.

## 🗂️ Project Structure

- `zappy_server/` — Main server written in C++
- `zappy_gui/` — Graphical interface written in C++ (using Raylib)
- `zappy_ai/` — AI client written in your language of choice (Python)

Gui and Server are compiled via makefile
```bash
./build.sh
```
The Client can be compiled thanks to the following command
```bash
python3 Client/client.py --port [PORT] --team [TEAM]
```

## 🧠 Concept

Each team controls several drones (players) that spawn on a shared tile-based world called **Trantor**. Drones must:
- Gather food to survive
- Collect resources (7 types)
- Collaborate to perform **elevation rituals** to gain levels

A player dies if they run out of food. Elevations are cooperative and require exact conditions to succeed.

## 🌍 World Mechanics

- The world is a **wrap-around 2D map** (toroidal)
- The map is populated with food and six resource types:  
  `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`
- Resources respawn every 20 time units

The number of resources at spawn time is calculated using:
```
quantity = width * height * density
```

## 🧾 How to Run

### 🧠 Server

```bash
./zappy_server -p PORT -x WIDTH -y HEIGHT -n TEAM1 TEAM2 ... -c CLIENTS_PER_TEAM -f FREQ
```

| Flag   | Description                                     |
|--------|-------------------------------------------------|
| `-p`   | Port to listen on                              |
| `-x`   | Map width                                       |
| `-y`   | Map height                                      |
| `-n`   | List of team names                              |
| `-c`   | Number of clients per team                      |
| `-f`   | Game frequency (time unit reciprocal)           |

### 🖥️ GUI

```bash
./zappy_gui -p PORT -h HOST
```

The GUI connects to the server and visualizes the world in 2D (SFML-based). It receives real-time updates for tiles and player actions.

### 🤖 AI Client

```bash
./zappy_ai -p PORT -n TEAM_NAME -h HOST
```

The AI controls one drone. It sends commands to the server autonomously to:
- Gather food/resources
- Track inventory
- Coordinate elevations
- Reproduce (fork command)
- Communicate via broadcast

## 🕹️ Game Flow

1. Each team starts with `N` player slots (eggs).
2. Drones connect and spawn randomly.
3. Players must collect food to stay alive (`1 unit = 126 time units`).
4. Players must gather specific resources and team members to level up via the **incantation** command.
5. Level 8 requires a full group of 6 to win.

## 🧬 Elevation Requirements

| Level | Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|---------|----------|-----------|-------|-----------|--------|----------|
| 1→2   | 1       | 1        | 0         | 0     | 0         | 0      | 0        |
| 2→3   | 2       | 1        | 1         | 1     | 0         | 0      | 0        |
| 3→4   | 2       | 2        | 0         | 1     | 0         | 2      | 0        |
| 4→5   | 4       | 1        | 1         | 2     | 0         | 1      | 0        |
| 5→6   | 4       | 1        | 2         | 1     | 3         | 0      | 0        |
| 6→7   | 6       | 1        | 2         | 3     | 0         | 1      | 0        |
| 7→8   | 6       | 2        | 2         | 2     | 2         | 2      | 1        |

> All players involved in an incantation must be of the same level. Stones are consumed if elevation succeeds.

## 🧭 Commands Summary

Clients communicate with the server using simple newline-terminated strings. Here's a few core commands:

| Command       | Description                                | Time Cost (in `f` units) |
|---------------|--------------------------------------------|---------------------------|
| `Forward`     | Move forward                               | `7/f`                     |
| `Right`/`Left`| Turn right/left                            | `7/f`                     |
| `Look`        | Get visible surroundings                   | `7/f`                     |
| `Inventory`   | Get current inventory                      | `1/f`                     |
| `Broadcast`   | Send a message                             | `7/f`                     |
| `Connect_nbr` | Remaining egg slots for the team           | instant                   |
| `Fork`        | Lay an egg                                 | `42/f`                    |
| `Take`/`Set`  | Pick up or drop a resource                 | `7/f`                     |
| `Incantation` | Start level-up ritual                      | `300/f`                   |

If a command is invalid or fails: server replies with `ko`.

## 📢 Broadcast & Direction

When a player uses `Broadcast`, all other players receive:
```
message K, text
```
Where `K` is the direction (tile ID) relative to the receiver's orientation.

## 🧪 Development & Debugging

- All sockets are handled via `poll()` (non-blocking I/O)
- Protocol is fully ASCII, line-based
- GUI identifies itself by sending `GRAPHIC` as team name
- AI clients are autonomous after launch
- Multiple clients can run on localhost for testing

## 📸 GUI Features

- Real-time tile updates
- Resource and player display
- Support for player events: moves, pickups, drops, deaths, elevations
- Optimized updates via tile-based event broadcasting

## ✅ Win Condition

The game ends when **one team has at least six players at level 8**.

---

## 🛠️ Dependencies

- `zappy_gui`: SFML (>= 2.5)
- `zappy_server`: C standard library only
- `zappy_ai`: no constraints, can use any libs

---

## 🧑‍💻 Authors

Developed as part of the Epitech YEP project.
