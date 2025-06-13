# ecc.py - export compile commands

import json
import os
import subprocess
import sys

def run_command(cmd, cwd="."):
    print(f"Running: {cmd}")
    result = subprocess.run(cmd, shell=True, cwd=cwd)
    if result.returncode != 0:
        raise RuntimeError(f"Command failed: {cmd}")

def fix_include_paths(compile_commands_path, output_path):
    with open(compile_commands_path) as f:
            data = json.load(f)

    for entry in data:
        args = entry.get("arguments")
        directory = os.path.abspath(entry["directory"])

        if not args:
            continue

        new_args = []
        i = 0
        while i < len(args):
            arg = args[i]
            if arg == "-I" and i + 1 < len(args):
                rel_path = args[i + 1]
                abs_path = os.path.abspath(os.path.join(directory, rel_path))
                new_args.extend(["-I", abs_path])
                i += 2
            elif arg.startswith("-I"):
                rel_path = arg[2:]
                abs_path = os.path.abspath(os.path.join(directory, rel_path))
                new_args.append(f"-I{abs_path}")
                i += 1
            else:
                new_args.append(arg)
                i += 1

        entry["arguments"] = new_args

    with open(output_path, "w") as f:
        json.dump(data, f, indent=2)

def fix_wsl_path(path):
    if path.startswith("/mnt/"):
        drive_letter = path[5]
        rest = path[6:]
        return f"{drive_letter.upper()}:/{rest.replace('/', '\\')}"
    return path.replace('\\', '/').replace('//', '/')

def remap_mnt_dir(compile_commands_path, output_path):
    with open(compile_commands_path) as f:
        data = json.load(f)

    for entry in data:
        for key in ("file", "output", "directory"):
            if key in entry:
                entry[key] = fix_wsl_path(entry[key])

        args = entry.get("arguments", [])
        new_args = []
        for arg in args:
            if "/mnt/" in arg:
                new_args.append(fix_wsl_path(arg))
            else:
                new_args.append(arg)
        entry["arguments"] = new_args

    with open(output_path, "w") as f:
        json.dump(data, f, indent=2)

# --- Run steps ---
if __name__ == "__main__":
    if "--fix" in sys.argv:
        fix_include_paths("compile_commands.json", "compile_commands.json")
    elif "--wsl2win" in sys.argv:
        remap_mnt_dir("compile_commands.json", "compile_commands.json")
    else:
        try:
            run_command("make clean")
            run_command("bear -- make")
            fix_include_paths("compile_commands.json", "compile_commands.json")
            print("✅ Done: compile_commands.json written.")
        except Exception as e:
            print(f"❌ Error: {e}")
