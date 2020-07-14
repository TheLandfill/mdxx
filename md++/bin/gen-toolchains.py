#!/usr/bin/env python3
import sys
import os
import subprocess

bin_dir = "bin/"

class cd:
    """Context manager for changing the current working directory"""
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)

def gen_toolchain(toolchains):
    with cd(bin_dir):
        for toolchain in toolchains:
            with cd(toolchain):
                os.makedirs("Release", exist_ok=True)
                with cd("Release"):
                    subprocess.run(['cmake', '-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake', '../../../'])
                os.makedirs("Debug", exist_ok=True)
                with cd("Debug"):
                    subprocess.run(['cmake', '-DCMAKE_BUILD_TYPE=Debug', '-DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake', '../../../'])

def create_cur_toolchain_symlink(toolchain):
    debug_link = "Debug"
    release_link = "Release"
    if os.path.islink(debug_link):
        os.remove(debug_link)
    if os.path.islink(release_link):
        os.remove(release_link)
    os.symlink(bin_dir + toolchain + '/Debug', debug_link, target_is_directory=True)
    os.symlink(bin_dir + toolchain + '/Release', release_link, target_is_directory=True)


if __name__ == "__main__":
    if len(sys.argv) >= 2:
        gen_toolchain(sys.argv[1:])
        create_cur_toolchain_symlink(sys.argv[1])
