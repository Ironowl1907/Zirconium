from utils import *
import os

repo_url = "https://github.com/KhronosGroup/SPIRV-Cross"
dest_path = "../zirconium/vendor/spirv-cross"

clone_repo(repo_url, dest_path)

# Assuming your Makefile is in the root of your cloned repo
makefile_dir = os.path.join(dest_path)  # Adjust if necessary

run_makefile(makefile_dir)
