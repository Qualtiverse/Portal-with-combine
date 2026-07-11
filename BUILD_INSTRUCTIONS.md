# Combine Aperture Incursion - Build Guide

## What you get
- **Portal gun** from Portal 1 + **shotgun** from HL2
- **Combine soldiers** that auto-spawn in waves
- Aperture test chamber environment with Combine invasion

## Prerequisites
- Steam: Portal 1, Half-Life 2, Source SDK Base 2013 Singleplayer
- Visual Studio 2019 (Community, with "Desktop development with C++" workload)

## Setup

### 1. Get Portal-Base
```
git clone https://github.com/SonicEraZoR/Portal-Base.git
```
Or download/unzip to a folder.

### 2. Add our mod code
Run the setup script from the Portal-Base root:
```
.\combine_aperture_mod\setup_mod.ps1
```

### 3. Edit the VPC build file
Open `sp/src/game/server/server_hl2.vpc`.
Find the last `$Folder` section (it's `"Unused"`) and add BEFORE its closing `}`:

```
		$Folder	"Combine Aperture"
		{
			$File	"ca_mod\point_combine_spawner.cpp"
			$File	"ca_mod\point_combine_spawner.h"
		}
```

### 4. Build
```
cd sp/src
creategameprojects
```
Open `game_wc.sln` in VS2019. Build both:
- `Server (Portal_HL2)` → produces `server.dll`
- `Client (Portal_HL2)` → produces `client.dll`

### 5. Deploy
Copy both DLLs to `Source SDK Base 2013 Singleplayer/combine_aperture/bin/`

### 6. Launch
Steam > Source SDK Base 2013 SP > Properties > Launch Options: `-game combine_aperture`

## Making Maps

1. Open Hammer from the Source SDK launcher
2. Configure: Tools > Options > Game Config > Add config
   - Game: Source SDK Base 2013 SP
   - Mod: combine_aperture  
   - FGD: add `combine_aperture.fgd`
3. Place `point_combine_spawner` for Combine waves
4. Place `weapon_portalgun` and `weapon_shotgun` for player weapons
5. Compile and test

## No-Compile Alternative (skip steps 3-5)

If you don't want to compile, use existing entities:

| Desired behavior | Entity to use |
|---|---|
| Auto-spawn Combine | `logic_timer` + `env_entity_maker` (entity: `npc_combine`) |
| Give portal gun | Place `weapon_portalgun` in the map or `impulse 102` |
| Give shotgun | Place `weapon_shotgun` in the map |
| Wave system | `point_template` + `logic_relay` + `env_entity_maker` |

The `point_combine_spawner` just makes it cleaner — it's one entity instead of 3+.

## Console Commands
- `impulse 102` - Get portal gun
- `give weapon_shotgun` - Get shotgun
- `npc_create npc_combine` - Spawn combine (cheats)
- `nav_generate` - Rebuild nav mesh (fixes frozen NPCs)
- `sv_use_portal_gamerules 1` - Enable portal-style gameplay
