#!/usr/bin/env python3
import os, sys, json, time, io, urllib.request, urllib.error, base64

TOKEN  = os.environ["GITHUB_TOKEN"]
SHA    = os.environ.get("COMMIT_SHA", "unknown")[:7]
REPO   = "CarlosDev-max/HorizonOS"
ASSETS = sys.argv[1:]
LOG    = []

def log(msg):
    print(msg, flush=True)
    LOG.append(msg)

def api(path, method="GET", payload=None):
    url  = f"https://api.github.com{path}"
    body = json.dumps(payload).encode() if payload else None
    req  = urllib.request.Request(url, data=body, method=method, headers={
        "Authorization": f"Bearer {TOKEN}",
        "Content-Type":  "application/json",
        "Accept":        "application/vnd.github+json"
    })
    try:
        with urllib.request.urlopen(req) as r: return r.status, json.loads(r.read() or b"{}")
    except urllib.error.HTTPError as e: return e.code, json.loads(e.read() or b"{}")

def push_log():
    content = "\n".join(LOG)
    b64 = base64.b64encode(content.encode()).decode()
    ex  = api("/repos/CarlosDev-max/HorizonOS/contents/ci-logs/latest_publish.txt")
    pl  = {"message": "ci: publish log", "content": b64}
    if "sha" in ex[1] if isinstance(ex, tuple) else "sha" in ex:
        try: pl["sha"] = ex[1]["sha"] if isinstance(ex, tuple) else ex["sha"]
        except: pass
    api("/repos/CarlosDev-max/HorizonOS/contents/ci-logs/latest_publish.txt", "PUT", pl)

try:
    # --- Delete existing release ---
    sc, releases = api(f"/repos/{REPO}/releases")
    log(f"List releases: HTTP {sc}, count={len(releases) if isinstance(releases, list) else '?'}")
    if sc == 200 and isinstance(releases, list):
        for r in releases:
            if r.get("tag_name") == "dev":
                sc2, _ = api(f"/repos/{REPO}/releases/{r['id']}", "DELETE")
                log(f"Deleted release {r['id']}: HTTP {sc2}")
                break

    # --- Delete tag ref ---
    sc, resp = api(f"/repos/{REPO}/git/refs/tags/dev", "DELETE")
    log(f"Delete tag: HTTP {sc} {resp}")
    time.sleep(3)

    # --- Create release ---
    sc, rel = api(f"/repos/{REPO}/releases", "POST", {
        "tag_name": "dev", "target_commitish": "main",
        "name": f"HorizonOS dev — {SHA}",
        "body": f"Build {SHA}. horizonos.uf2=hardware | horizonos_sim.exe=Windows",
        "draft": False, "prerelease": True
    })
    log(f"Create release: HTTP {sc} — {rel.get('html_url', str(rel)[:200])}")
    if sc not in (200, 201):
        log(f"ERROR: {rel}")
        push_log()
        sys.exit(1)

    upload_url = rel["upload_url"].split("{")[0]
    log(f"Upload URL: {upload_url}")

    # --- Upload assets ---
    for fpath in ASSETS:
        if not os.path.exists(fpath):
            log(f"SKIP {fpath} (not found)")
            continue
        name = os.path.basename(fpath)
        size = os.path.getsize(fpath)
        log(f"Uploading {name} ({size} bytes)...")
        with open(fpath, "rb") as f: data = f.read()
        url = f"{upload_url}?name={name}"
        req = urllib.request.Request(url, data=data, method="POST", headers={
            "Authorization": f"Bearer {TOKEN}",
            "Content-Type": "application/octet-stream",
            "Accept": "application/vnd.github+json"
        })
        try:
            with urllib.request.urlopen(req) as r:
                a = json.loads(r.read())
                log(f"  OK: {a.get('browser_download_url', '?')}")
        except urllib.error.HTTPError as e:
            log(f"  FAILED HTTP {e.code}: {e.read()[:300]}")

    log("Done.")
    push_log()

except Exception as ex:
    log(f"EXCEPTION: {type(ex).__name__}: {ex}")
    push_log()
    sys.exit(1)
