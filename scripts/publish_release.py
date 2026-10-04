#!/usr/bin/env python3
"""Publishes (or re-publishes) the dev pre-release on GitHub.
Reads GITHUB_TOKEN and COMMIT_SHA from environment.
Assets to upload must be passed as CLI args.
"""
import os, sys, json, time, urllib.request, urllib.error

TOKEN = os.environ["GITHUB_TOKEN"]
SHA   = os.environ.get("COMMIT_SHA", "unknown")[:7]
REPO  = "CarlosDev-max/HorizonOS"
ASSETS = sys.argv[1:]  # e.g. build/horizonos.uf2 build/horizonos.elf

def api(path, method="GET", payload=None):
    url  = f"https://api.github.com{path}"
    body = json.dumps(payload).encode() if payload else None
    req  = urllib.request.Request(url, data=body, method=method, headers={
        "Authorization": f"Bearer {TOKEN}",
        "Content-Type":  "application/json",
        "Accept":        "application/vnd.github+json"
    })
    try:
        with urllib.request.urlopen(req) as r:
            return r.status, json.loads(r.read() or b"{}")
    except urllib.error.HTTPError as e:
        return e.code, json.loads(e.read() or b"{}")

def upload_asset(upload_url, filepath):
    name = os.path.basename(filepath)
    with open(filepath, "rb") as f:
        data = f.read()
    url = f"{upload_url}?name={name}"
    req = urllib.request.Request(url, data=data, method="POST", headers={
        "Authorization": f"Bearer {TOKEN}",
        "Content-Type":  "application/octet-stream",
        "Accept":        "application/vnd.github+json"
    })
    try:
        with urllib.request.urlopen(req) as r:
            a = json.loads(r.read())
            print(f"  Uploaded {name}: {a.get('browser_download_url', '?')}")
    except urllib.error.HTTPError as e:
        print(f"  Upload {name} FAILED HTTP {e.code}: {e.read()[:200]}")

# --- Delete existing 'dev' release ---
sc, releases = api(f"/repos/{REPO}/releases")
if sc == 200:
    for r in releases:
        if r.get("tag_name") == "dev":
            sc2, _ = api(f"/repos/{REPO}/releases/{r['id']}", "DELETE")
            print(f"Deleted release {r['id']}: HTTP {sc2}")
            break
else:
    print(f"List releases: HTTP {sc}")

# --- Delete the 'dev' tag ref ---
sc, _ = api(f"/repos/{REPO}/git/refs/tags/dev", "DELETE")
print(f"Deleted tag 'dev': HTTP {sc}")
time.sleep(3)

# --- Create the new release ---
sc, rel = api(f"/repos/{REPO}/releases", "POST", {
    "tag_name":         "dev",
    "target_commitish": "main",
    "name":             f"HorizonOS dev — {SHA}",
    "body":             (
        f"**Automatic build** from commit `{SHA}`\n\n"
        "| File | Use for |\n|------|---------|\n"
        "| `horizonos.uf2` | Flash to Pico W / Pico 2W hardware |\n"
        "| `horizonos.elf` | Wokwi simulation |\n"
        "| `horizonos_sim.exe` + `SDL2.dll` | Windows desktop simulator |\n"
    ),
    "draft":      False,
    "prerelease": True
})
print(f"Created release: HTTP {sc} — {rel.get('html_url', rel.get('message', rel))}")
if sc not in (200, 201):
    sys.exit(1)

upload_url = rel["upload_url"].split("{")[0]
print(f"Uploading {len(ASSETS)} asset(s)...")
for asset in ASSETS:
    if os.path.exists(asset):
        upload_asset(upload_url, asset)
    else:
        print(f"  SKIP {asset} (not found)")

print("Done.")
