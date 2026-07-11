# CI Build Setup for Portal-Base

## What this gives you
Push code to GitHub → GitHub Actions compiles it with VS 2022 → download server.dll

## Step-by-step

1. Create a repo on GitHub (e.g. `portal-base-combine-aperture`)

2. Upload the source:
   - Copy the entire `Portal-Base-0.8.5-beta\sp\src` folder into the repo
   - Copy the `.github\workflows\build.yml` into the repo root
   - Copy `game\server\server_hl2.vcxproj` into `sp\src\game\server\server_hl2.vcxproj` in the repo
   - Copy `crt_stubs.cpp` into `sp\src\game\server\crt_stubs.cpp` in the repo

3. Custom code goes in:
   - `sp\src\game\server\portal\` (new entity .cpp/.h files)
   - `sp\src\game\shared\portal\` (shared code changes)
   - Don't forget to add new .cpp files to `server_hl2.vcxproj`

4. Push to GitHub

5. Go to your repo → Actions tab → "Build Portal-Base Server DLL" workflow

6. When it finishes, download `server-dll` artifact

7. Replace `hl2_with_ashpd\bin\server.dll` in your Portal-Base release

## Custom entities
After the workflow runs, you can add new files and the CI will rebuild.
