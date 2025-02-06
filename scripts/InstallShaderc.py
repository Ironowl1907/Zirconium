import os
import subprocess

# Define paths
script_path = os.path.dirname(os.path.abspath(__file__))
vendor_path = os.path.join(script_path, '../zirconium/vendor')
shaderc_repo_url = 'https://github.com/google/shaderc.git'
shaderc_clone_path = os.path.join(vendor_path, 'shaderc')

# Clone Shaderc repository if it doesn't exist
if not os.path.exists(shaderc_clone_path):
    print("Cloning Shaderc repository...")
    subprocess.run(['git', 'clone', shaderc_repo_url], cwd=vendor_path)

# Sync dependencies using git-sync-deps (assuming it exists in utils/)
sync_deps_script = os.path.join(shaderc_clone_path, 'utils/git-sync-deps')
if not os.access(sync_deps_script, os.X_OK):
    print("The git-sync-deps script is not executable or does not exist.")
else:
    print("Syncing dependencies using git-sync-deps...")
    subprocess.run([sync_deps_script], cwd=shaderc_clone_path)

# Create build directory if it doesn't exist
build_dir = os.path.join(shaderc_clone_path, 'build')
if not os.path.exists(build_dir):
    print("Creating build directory...")
    os.makedirs(build_dir)

# Build Shaderc using CMake and Ninja (or MSVC)
cmake_generator = '-GNinja'  # Use Ninja on Linux or Windows; remove this line to use MSVC on Windows

print("Building Shaderc...")
subprocess.run([
    'cmake',
    cmake_generator,
    '-DCMAKE_BUILD_TYPE=Release',
    '..'  # Specify source directory relative to build dir.
], cwd=build_dir)
subprocess.run(['ninja'], cwd=build_dir)  # Use ninja ctest for testing

print("Shader compilation tools are ready.")
