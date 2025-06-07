import os
import shutil
import argparse
import subprocess
import re
from pathlib import Path

def run_command(command, cwd=None):
    try:
        subprocess.run(command, shell=True, check=True, cwd=cwd)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {command}")
        print(e)
        exit(1)
    
def validate_project_name(name):
    if not (3 <= len(name) <= 13):
        print("Error: Project name must be between 3 and 13 characters long")
        exit(1)
    if not re.match(r'^[A-Za-z0-9_]+$', name):
        print("Error: Project name contains forbidden characters, allowed characters: A-z, 0-9, _")
        exit(1)
    return name

def create_new_project(project_name):
    validate_project_name(project_name)

    print(f"Cloning low-end-engine repository...")
    run_command("git clone --depth 1 https://github.com/Flone-dnb/low-end-engine.git")
    
    print(f"Renaming directory to {project_name}...")
    os.rename("low-end-engine", project_name)
    
    os.chdir(project_name)

    with open("engine_settings.cmake", "w") as f:
        f.write(f"set(GAME_PROJECT_NAME \"{project_name}\")\n")
    
    print("Renaming engine's branch/origin...")
    run_command("git branch -m master engine", cwd=".")
    run_command("git remote rename origin origin_engine", cwd=".")
    run_command("git checkout -b master", cwd=".")
    
    project_dir = Path(".").resolve()
    src_dir = project_dir / "src"
    
    script_dir = Path(__file__).parent.resolve()
    game_src = script_dir / "game"
    game_lib_src = script_dir / "game_lib"
    game_tests_src = script_dir / "game_tests"

    if not game_src.exists():
        print(f"Error: expected the 'game' directory to be found at {game_src}")
        exit(1)
    if not game_lib_src.exists():
        print(f"Error: expected the 'game_lib' directory to be found at {game_lib_src}")
        exit(1)
    if not game_tests_src.exists():
        print(f"Error: expected the 'game_tests' directory to be found at {game_tests_src}")
        exit(1)

    # Copy game targets.
    shutil.copytree(game_src, src_dir / project_name, dirs_exist_ok=True)
    shutil.copytree(game_lib_src, src_dir / f"{project_name}_lib", dirs_exist_ok=True)
    shutil.copytree(game_tests_src, src_dir / f"{project_name}_tests", dirs_exist_ok=True)
    
    print("Pulling submodules...")
    run_command("git submodule update --init --recursive", cwd=".")

    print("\n- Everything is ready, you now need to commit the changes - this will be the first commit of your game.")
    print("- After that you would need to setup your remote (git remote add origin <url>) to push your changes.")
    print("- This way you should have 2 branches and 2 origins: master (your game's master) and engine (engine's master), origin (your game's origin) and origin_engine (engine's origin).")

def update_engine_for_project(project_path):
    project_path = Path(project_path).resolve()
    if not project_path.exists():
        print(f"Error: Project directory not found at {project_path}")
        exit(1)
    os.chdir(project_path)
    
    print("Updating engine branch...")
    run_command("git checkout engine")
    run_command("git pull --rebase origin_engine master")
    
    print("Merging changes into the game's master branch...")
    run_command("git checkout master")
    run_command("git merge --squash engine")
    run_command("git submodule update --init --recursive")
    run_command("git add .")
    
    print("\nEngine branch was updated and merged into your game's master branch, now you need to make sure everything is working and commit the changes.")

def main():
    parser = argparse.ArgumentParser(description='Create or update a game project.')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--new', metavar='PROJECT_NAME', type=str, 
                      help='Create a new project with the given name.')
    group.add_argument('--update', metavar='PROJECT_PATH', type=str,
                      help='Path to existing project directory to update the engine version.')
    
    args = parser.parse_args()
    
    if args.new:
        create_new_project(args.new)
    elif args.update:
        update_engine_for_project(args.update)

if __name__ == "__main__":
    main()
