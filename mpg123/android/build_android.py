#!/usr/bin/env python
import argparse
import datetime
import json
import os
from multiprocessing import cpu_count
import shutil
import subprocess
import sys
from sys import platform

WORKING_DIR = '.'
BUILD_TYPES = ('Release', 'Debug')
ANDROID_ABIS = {
                    'arm64-v8a': { 'triple': 'aarch64-linux-android', 'binutilsPrefix': 'aarch64-linux-android', 'mpg123cpu': 'aarch64'},
                    'armeabi-v7a': { 'triple': 'armv7a-linux-androideabi', 'binutilsPrefix': 'arm-linux-androideabi', 'mpg123cpu': 'arm_nofpu'},
                    'x86': { 'triple': 'i686-linux-android', 'binutilsPrefix': 'i686-linux-android', 'mpg123cpu': 'x86'},
                    'x86_64': { 'triple': 'x86_64-linux-android', 'binutilsPrefix': 'x86_64-linux-android', 'mpg123cpu': 'x86-64'}
                }
ANDROID_PLATFORM = 'android-19'
    
ERROR_MISSING_ANDROID_NDK = 1

def call(cwd, cmd, capture_output=False, exception_on_nonzero=True):
    """Execute command in provided working directory"""

    # print('dir: %s' % cwd)
    # print('cmd: %s' % cmd)
    if capture_output:
        proc = subprocess.Popen(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    else:
        proc = subprocess.Popen(cmd, cwd=cwd)
    (out, err) = proc.communicate()
    if exception_on_nonzero and proc.returncode != 0:
        print("Bailing for non zero returncode")
        raise Exception(proc.returncode)
    return (out, err)

def build(android_ndk_root):
    # Let's check we have an NDK path
    ndk_root = None
    if not android_ndk_root is None:
        ndk_root = android_ndk_root
    elif 'ANDROID_NDK_ROOT' in os.environ:
        ndk_root = os.environ['ANDROID_NDK_ROOT']
    else:
        print("Error: Couldn't find Android NDK. Set with --android-ndk-root or in environment variable ANDROID_NDK_ROOT")
        sys.exit(ERROR_MISSING_ANDROID_NDK)

    # Verify NDK looks crudely sane
    toolchain_file = os.path.join(ndk_root, 'build', 'cmake', 'android.toolchain.cmake')
    if not os.path.exists(toolchain_file):
        print("Error: Android NDK path '%s' does not appear to be valid (couldn't find build/cmake/android.toolchain.cmake')" % ndk_root)
        sys.exit(ERROR_MISSING_ANDROID_NDK)

    if os.path.exists('android'):
        shutil.rmtree('android')

    # Iterate ABIs
    for abi, abi_config in ANDROID_ABIS.iteritems():

        for build_type in BUILD_TYPES:
            if os.path.exists('android_stage'):
                shutil.rmtree('android_stage')

            print(repr(ndk_root))
            if os.system == 'darwin':
                os.environ['HOST_TAG'] = 'darwin-x86_64'
            else:
                os.environ['HOST_TAG'] = 'linux-x86_64'

            os.environ['TOOLCHAIN'] = ndk_root + '/toolchains/llvm/prebuilt/%s' % os.environ['HOST_TAG']
            toolchain = ndk_root + '/toolchains/llvm/prebuilt/%s' % os.environ['HOST_TAG']
            os.environ['AR'] = '%s/bin/%s-ar' % (toolchain, abi_config['binutilsPrefix'])
            os.environ['AS'] = '%s/bin/%s-as' % (toolchain, abi_config['binutilsPrefix'])
            os.environ['CC'] = '%s/bin/%s21-clang' % (toolchain, abi_config['triple'])
            os.environ['CXX'] = '%s/bin/%s21-clang++' % (toolchain, abi_config['binutilsPrefix'])
            os.environ['LD'] = '%s/bin/%s-ld' % (toolchain, abi_config['binutilsPrefix'])
            os.environ['RANLIB'] = '%s/bin/%s-ranlib' % (toolchain, abi_config['binutilsPrefix'])
            os.environ['STRIP'] = '%s/bin/%s-strip' % (toolchain, abi_config['binutilsPrefix'])

            configure_command = ['./configure', 
                                 '--host', abi_config['binutilsPrefix'],
                                 '--prefix=%s' % os.path.abspath('./android_stage'),
                                 '--with-cpu=%s' % abi_config['mpg123cpu'],
                                 '--with-audio=dummy'
                                 ]

            if build_type.lower() == 'debug':
                configure_command.append('--enable-debug')

            # Configure
            call(WORKING_DIR, configure_command)

            subprocess.call(['echo "\n#define HAVE_MMAP 1" >> src/config.h'], shell=True)

            # Clean
            call(WORKING_DIR, ['make', 'clean'])

            # Build
            call(WORKING_DIR, ['make', 'install', '-j', str(cpu_count())])
            # call(WORKING_DIR, ['make', '-j', str(cpu_count())])

            # Install to
            install_dir = os.path.join('.', 'android', 'lib', build_type, abi)
            if not os.path.exists(install_dir):
                os.makedirs(install_dir)
            lib_filename = 'libmpg123.so'
            library_path = os.path.join('.', 'android_stage', 'lib', lib_filename)
            shutil.copyfile(library_path, os.path.join(install_dir, lib_filename))

    # Install includes
    # TODO fairly hacky copying of includes from the Linux install path to accommodate
    #      there (seeming to?) be no install phase in the Android toolchain
    include_src_dir = os.path.join('.', 'android_stage', 'include')
    include_install_dir = os.path.join('android', 'include')
    if os.path.exists(include_install_dir):
        shutil.rmtree(include_install_dir)
    shutil.copytree(include_src_dir, include_install_dir)

    # # Install license/docs
    shutil.copyfile('README', os.path.join('android', 'README'))
    shutil.copyfile('AUTHORS', os.path.join('android', 'AUTHORS'))
    shutil.copyfile('doc/LICENSE', os.path.join('android', 'LICENSE'))
    shutil.copyfile('doc/PATENTS', os.path.join('android', 'PATENTS'))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--android-ndk-root", 
                        type=str,
                        help="The path to NDK to use for the Android build")

    args = parser.parse_args()

    if sys.platform.startswith('win'):
        print("Unfortunately this is Unix-only territory, at least until somebody tests this with https://docs.microsoft.com/en-us/windows/wsl/about")
        sys.exit(1)

    build(args.android_ndk_root)
