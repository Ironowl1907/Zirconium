import os
import subprocess
import urllib.request

def download_and_build_lua(url, custom_path):
    """
    Downloads Lua source to a custom path, extracts it, and builds it using make.

    Args:
        url (str): The URL of the Lua source tarball.
        custom_path (str): The directory where the file will be downloaded and extracted.
    """
    try:
        # Ensure the custom path exists
        if not os.path.exists(custom_path):
            os.makedirs(custom_path)

        # Change to the specified custom path
        os.chdir(custom_path)

        # Extract filename from URL
        filename = url.split("/")[-1]

        # Download the file to the custom path
        print(f"Downloading {filename} to {custom_path}...")
        urllib.request.urlretrieve(url, filename)
        print(f"Downloaded {filename}")

        # Extract the tarball
        print(f"Extracting {filename}...")
        subprocess.run(["tar", "zxf", filename], check=True)

        # Get the directory name from the tarball filename
        directory_name = filename.replace(".tar.gz", "")

        # Change directory to the extracted folder
        print(f"Changing directory to {directory_name}...")
        os.chdir(directory_name)


        # Run make all test
        print("Running 'make'...")
        subprocess.run(["make"], check=True)

        # Run make all test
        print("Running 'make local'...")
        subprocess.run(["make", "local"], check=True)

        print("Lua build and test completed successfully!")

    except subprocess.CalledProcessError as e:
        print(f"Error during build process: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    lua_url = "https://www.lua.org/ftp/lua-5.4.7.tar.gz"

    # Specify your custom download path here
    custom_download_path = "../zirconium/vendor/lua"

    download_and_build_lua(lua_url, custom_download_path)
