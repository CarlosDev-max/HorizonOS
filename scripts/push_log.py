
#!/usr/bin/env python3
import os, sys, json, base64, urllib.request, urllib.error

TOKEN = os.environ.get("GITHUB_TOKEN", "")
REPO  = "CarlosDev-max/HorizonOS"

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

if len(sys.argv) < 3:
    print("Usage: push_log.py <local_file> <repo_path>")
    sys.exit(1)

local_file = sys.argv[1]
repo_path  = sys.argv[2]

content = open(local_file, "r", errors="replace").read() if os.path.exists(local_file) else "file not found"
b64 = base64.b64encode(content.encode()).decode()

sc, ex = api(f"/repos/{REPO}/contents/{repo_path}")
pl = {"message": "ci: update log", "content": b64}
if sc == 200 and "sha" in ex:
    pl["sha"] = ex["sha"]

sc2, result = api(f"/repos/{REPO}/contents/{repo_path}", "PUT", pl)
print(f"Push {repo_path}: HTTP {sc2}")
