# install `scene' module
from __future__ import print_function # Python3 compatible
import shutil
import sys
import os

scene_fn = "scene.so"

def contains(folder, file):
    return file in os.listdir(os.path.join("build", folder))

def run(cmd):
    if (os.system(cmd) != 0):
        sys.exit(1) # Stop if something fails

def move(src, dst):
    try:
        shutil.move(src, dst)
        return True
    except:
        return False

print("Building")
run("python setup.py build")   # build `scene'
folders = os.listdir("build") # get folders
for folder in folders:
    if contains(folder, scene_fn):
        break
else:
    sys.stderr.write("failed to locate `%s'", scene_fn)
    exit(1)

if not os.path.exists("scene"):
    os.mkdir("scene")

for file in ["scene.py", "_scene_types.py", "sound.py"]:
    path = os.path.join("python", file)
    print("Moving `%s'" % path)
    if (not move(path, "scene")):
        print("Failed to move %s" % path)

path = os.path.join("build", folder, scene_fn)
print("Moving `%s'" % path)
if (not move(path, os.path.join("scene", "_scene.so"))):
    print("Failed to move %s" % path)

print("Done.")
