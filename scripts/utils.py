import subprocess
import sys
import os

def clone_repo(repo_url, dest_path):
    """Clone the repository."""
    try:
    # Check if destination path exists (i.e., repo is already cloned)
        if os.path.exists(dest_path):
            print(f"Repository {repo_url} is already cloned at {dest_path}.")
            return True
        subprocess.run(['git', 'clone', repo_url, dest_path], check=True)
        print(f"Repository cloned successfully to {dest_path}")
    except subprocess.CalledProcessError as e:
        print(f"Failed to clone repository: {e}")
        sys.exit(1)

def run_makefile(makefile_dir):
    """Run the Makefile in the specified directory."""

    try:
        subprocess.run(['make', '-j' ,'-C', makefile_dir], check=True)
        print("Make command executed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to run Make command: {e}")

# Example usage
# if __name__ == "__main__":
#     repo_url = "https://github.com/your-repo-url.git"
#     dest_path = "path/to/local/directory"

#     clone_repo(repo_url, dest_path)

#     # Assuming your Makefile is in the root of your cloned repo
#     makefile_dir = os.path.join(dest_path)  # Adjust if necessary

#     run_makefile(makefile_dir)
