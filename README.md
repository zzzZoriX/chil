# CHIL

**CHIL** is a lightweight console-based music player designed for convenient music playback and playlist management.

**Version:** 1.0.0  
**Author:** [zorix](https://github.com/zzzZoriX)

---

## Features

- Create and manage music playlists.
- Play, pause, resume and stop music.
- Adjust playback volume.
- Add and remove tracks from playlists.
- Browse tracks in the currently loaded playlist.
- Automatic directory initialization.
- Simple command-based interface.

## Getting Started

### 1. Launching CHIL

To start using CHIL, simply launch the `chil` executable.

On startup, the application automatically checks the integrity of its required directories.

If any required directories are missing, CHIL creates them automatically.

### 2. Directory Structure

CHIL creates two directories in the computer's root directory:

```text
/
├── chil_playlists/
└── chil_to_add/
```

| Directory | Description |
|---|---|
| `chil_playlists` | Stores your playlists and their tracks. |
| `chil_to_add` | Stores tracks waiting to be added to a playlist. |

### 3. Managing Playlists

CHIL uses directories to organize playlists and music files.

- **Loading:** Select a playlist from `chil_playlists`.
- **Adding tracks:** Place tracks in `chil_to_add` and use the `add` command.
- **Removing tracks:** Use the `remove` command to move tracks back to `chil_to_add`.
- **Refreshing:** Reload the playlist after adding new tracks to make them available for playback.

## Commands

All commands are entered directly into the CHIL terminal.

| Command | Alias | Description |
|---|---|---|
| `exit` | `0` | Exit the application. |
| `load` | `1` | Load a playlist from `chil_playlists`. |
| `make` | `2` | Make a playlist in `chil_playlists`. |
| `play` | `3` | Start playing the loaded playlist. |
| `pause` | `4` | Pause or resume the current track. |
| `stop` | `5` | Stop playlist playback. |
| `volume` | `6` | Adjust playback volume. |
| `add` | `7` | Move a track from `chil_to_add` to the loaded playlist. |
| `remove` | `8` | Move a track from the loaded playlist back to `chil_to_add`. |
| `look` | `9` | Display tracks from the loaded playlist. |
| `info` | `10` | Display application information and available commands. |

## Important Notes

- Newly added tracks will not appear in the currently loaded playlist until it is reloaded.
- The `look` command displays only tracks that were present when the playlist was loaded.
- The `add` and `remove` commands move files between directories rather than simply changing playlist entries.
- Required directories are automatically created if they do not exist.

## Help

To display information about available commands, enter either:

```text
info
```

or:

```text
10
```

## About

CHIL is a personal project focused on building a simple, convenient and lightweight music player with playlist management capabilities.

**Developed by zorix.**
