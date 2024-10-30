Package: libsystemd:x64-linux@256.4

**Host Environment**

- Host: x64-linux
- Compiler: GNU 14.2.1
-    vcpkg-tool version: 2024-10-18-e392d7347fe72dff56e7857f7571c22301237ae6
    vcpkg-scripts version: bb1ca2757 2024-10-29 (13 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Found Python version '3.12.7 at /usr/bin/python3'
-- Using meson: /home/justine/.local/share/vcpkg/downloads/tools/meson-1.5.1-81f7e3/meson.py
-- Using cached systemd-systemd-v256.4.tar.gz.
-- Cleaning sources at /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source /home/justine/.local/share/vcpkg/downloads/systemd-systemd-v256.4.tar.gz
-- Applying patch disable-warning-nonnull.patch
-- Applying patch only-libsystemd.patch
-- Applying patch pkgconfig.patch
-- Using source at /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean
-- Found external ninja('1.12.1').
-- Configuring x64-linux-dbg
-- Getting CMake variables for x64-linux
CMake Error at scripts/cmake/vcpkg_execute_required_process.cmake:127 (message):
    Command failed: /usr/bin/python3 -I /home/justine/.local/share/vcpkg/downloads/tools/meson-1.5.1-81f7e3/meson.py setup -Dmode=release -Dstatic-libsystemd=pic -Dtests=false -Ddns-over-tls=false -Dtranslations=false -Dacl=disabled -Dapparmor=disabled -Daudit=disabled -Dblkid=disabled -Dbpf-framework=disabled -Dbzip2=disabled -Ddbus=disabled -Delfutils=disabled -Dfdisk=disabled -Dgcrypt=disabled -Dglib=disabled -Dgnutls=disabled -Dkmod=disabled -Dlibcurl=disabled -Dlibcryptsetup=disabled -Dlibfido2=disabled -Dlibidn=disabled -Dlibidn2=disabled -Dlibiptc=disabled -Dmicrohttpd=disabled -Dopenssl=disabled -Dp11kit=disabled -Dpam=disabled -Dpcre2=disabled -Dpolkit=disabled -Dpwquality=disabled -Dpasswdqc=disabled -Dseccomp=disabled -Dselinux=disabled -Dtpm2=disabled -Dxenctrl=disabled -Dxkbcommon=disabled -Dzlib=disabled -Dlz4=enabled -Dxz=enabled -Dzstd=enabled --backend ninja --wrap-mode nodownload -Doptimization=plain --native /home/justine/.local/share/vcpkg/buildtrees/libsystemd/meson-x64-linux-dbg.log --libdir lib --pkgconfig.relocatable -Ddebug=true --prefix /home/justine/.local/share/vcpkg/packages/libsystemd_x64-linux/debug --includedir ../include --pkg-config-path ['/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig','/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig'] -Dcmake_prefix_path=['/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug','/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux'] /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean
    Working Directory: /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg
    Error code: 1
    See logs for more information:
      /home/justine/.local/share/vcpkg/buildtrees/libsystemd/config-x64-linux-dbg-meson-log.txt.log
      /home/justine/.local/share/vcpkg/buildtrees/libsystemd/config-x64-linux-dbg-out.log

Call Stack (most recent call first):
  /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/vcpkg-tool-meson/vcpkg_configure_meson.cmake:467 (vcpkg_execute_required_process)
  /home/justine/.cache/vcpkg/registries/git-trees/03b20e5c1a908db5d38dd3fad3fa074c10e6b04b/portfile.cmake:17 (vcpkg_configure_meson)
  scripts/ports.cmake:192 (include)



```

<details><summary>/home/justine/.local/share/vcpkg/buildtrees/libsystemd/config-x64-linux-dbg-out.log</summary>

```
The Meson build system
Version: 1.5.1
Source dir: /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean
Build dir: /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg
Build type: native build
Project name: systemd
Project version: 256.4
C compiler for the host machine: /usr/bin/cc -fPIC -g (gcc 14.2.1 "cc (GCC) 14.2.1 20240910")
C linker for the host machine: /usr/bin/cc -fPIC -g ld.bfd 2.43.0
Host machine cpu family: x86_64
Host machine cpu: x86_64
Program tools/git-setup.sh found: YES (/home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean/tools/git-setup.sh)
Program tools/meson-build.sh found: YES (/home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean/tools/meson-build.sh)
C++ compiler for the host machine: /usr/bin/c++ -fPIC -g (gcc 14.2.1 "c++ (GCC) 14.2.1 20240910")
C++ linker for the host machine: /usr/bin/c++ -fPIC -g ld.bfd 2.43.0
Compiler for C supports arguments -Wno-missing-field-initializers: YES 
Compiler for C supports arguments -Wno-unused-parameter: YES 
Compiler for C supports arguments -Wno-nonnull-compare: YES 
Compiler for C supports arguments -Warray-bounds: YES 
Compiler for C supports arguments -Warray-bounds=2: YES 
Compiler for C supports arguments -Wdate-time: YES 
Compiler for C supports arguments -Wendif-labels: YES 
Compiler for C supports arguments -Werror=format=2: YES 
Compiler for C supports arguments -Werror=format-signedness: YES 
Compiler for C supports arguments -Werror=implicit-function-declaration: YES 
Compiler for C supports arguments -Werror=implicit-int: YES 
Compiler for C supports arguments -Werror=incompatible-pointer-types: YES 
Compiler for C supports arguments -Werror=int-conversion: YES 
Compiler for C supports arguments -Werror=missing-declarations: YES 
Compiler for C supports arguments -Werror=missing-prototypes: YES 
Compiler for C supports arguments -Werror=overflow: YES 
Compiler for C supports arguments -Werror=override-init: YES 
Compiler for C supports arguments -Werror=return-type: YES 
Compiler for C supports arguments -Werror=shift-count-overflow: YES 
Compiler for C supports arguments -Werror=shift-overflow=2: YES 
Compiler for C supports arguments -Werror=strict-flex-arrays: YES 
Compiler for C supports arguments -Werror=undef: YES 
Compiler for C supports arguments -Wfloat-equal: YES 
Compiler for C supports arguments -Wimplicit-fallthrough=5: YES 
Compiler for C supports arguments -Winit-self: YES 
Compiler for C supports arguments -Wlogical-op: YES 
Compiler for C supports arguments -Wmissing-include-dirs: YES 
Compiler for C supports arguments -Wmissing-noreturn: YES 
Compiler for C supports arguments -Wnested-externs: YES 
Compiler for C supports arguments -Wold-style-definition: YES 
Compiler for C supports arguments -Wpointer-arith: YES 
Compiler for C supports arguments -Wredundant-decls: YES 
Compiler for C supports arguments -Wshadow: YES 
Compiler for C supports arguments -Wstrict-aliasing=2: YES 
Compiler for C supports arguments -Wstrict-prototypes: YES 
Compiler for C supports arguments -Wsuggest-attribute=noreturn: YES 
Compiler for C supports arguments -Wunused-function: YES 
Compiler for C supports arguments -Wwrite-strings: YES 
Compiler for C supports arguments -Wzero-length-bounds: YES 
Compiler for C supports arguments -Wno-error=#warnings: NO 
Compiler for C supports arguments -Wno-string-plus-int: NO 
Compiler for C supports arguments -fdiagnostics-show-option: YES 
Compiler for C supports arguments -fno-common: YES 
Compiler for C supports arguments -fstack-protector: YES 
Compiler for C supports arguments -fstack-protector-strong: YES 
Compiler for C supports arguments -fstrict-flex-arrays: YES 
Compiler for C supports arguments --param=ssp-buffer-size=4: YES 
Compiler for C supports arguments -Wno-maybe-uninitialized: YES 
Compiler for C supports arguments -Wno-unused-result: YES 
Compiler for C supports arguments -ftrivial-auto-var-init=zero: YES 
Compiler for C supports link arguments -fstack-protector: YES 
Compiler for C supports arguments -fno-strict-aliasing: YES 
Compiler for C supports arguments -fstrict-flex-arrays=1: YES 
Compiler for C supports arguments -fvisibility=hidden: YES 
Compiler for C supports link arguments -Wl,--fatal-warnings: YES 
Compiler for C supports link arguments -Wl,-z,now: YES 
Compiler for C supports link arguments -Wl,-z,relro: YES 
Compiler for C supports link arguments -Wl,--warn-common: YES 
Compiler for C supports arguments -Wzero-length-bounds: YES (cached)
Checking if "-Werror=shadow with local shadowing" compiles: YES 
Compiler for C++ supports arguments -Wno-missing-field-initializers: YES 
Compiler for C++ supports arguments -Wno-unused-parameter: YES 
Compiler for C++ supports arguments -Wno-nonnull-compare: YES 
Compiler for C supports arguments -Wstringop-truncation: YES 
Checking for size of "dev_t" : 8 
Checking for size of "ino_t" : 8 
Checking for size of "rlim_t" : 8 
Checking for size of "time_t" : 8 
Checking for size of "typeof(((struct timex *)0)->freq)" : 8 
Computing int of "LONG_MAX" : 9223372036854775807
Checking for size of "char16_t" : 2 
Checking for size of "char32_t" : 4 
Checking for size of "struct mount_attr" : 32 
Checking for size of "struct statx" : 256 
Checking for size of "struct dirent64" : 280 
Checking for size of "struct sched_attr" : -1 
Checking for function "secure_getenv" : YES 
Checking for function "__secure_getenv" : NO 
Checking for function "memfd_create" : YES 
Checking for function "gettid" : YES 
Checking for function "fchmodat2" : NO 
Checking for function "pivot_root" : NO 
Checking for function "ioprio_get" : NO 
Checking for function "ioprio_set" : NO 
Checking for function "sched_setattr" : NO 
Checking for function "name_to_handle_at" : YES 
Checking for function "setns" : YES 
Checking for function "renameat2" : YES 
Checking for function "kcmp" : NO 
Checking for function "keyctl" : NO 
Checking for function "copy_file_range" : YES 
Checking for function "bpf" : NO 
Checking for function "statx" : YES 
Checking for function "explicit_bzero" : YES 
Checking for function "reallocarray" : YES 
Checking for function "set_mempolicy" : NO 
Checking for function "get_mempolicy" : NO 
Checking for function "pidfd_send_signal" : YES 
Checking for function "pidfd_open" : YES 
Checking for function "rt_sigqueueinfo" : NO 
Checking for function "rt_tgsigqueueinfo" : NO 
Checking for function "mallinfo" : YES 
Checking for function "mallinfo2" : YES 
Checking for function "execveat" : YES 
Checking for function "close_range" : YES 
Checking for function "epoll_pwait2" : YES 
Checking for function "mount_setattr" : YES 
Checking for function "move_mount" : YES 
Checking for function "open_tree" : YES 
Checking for function "fsopen" : YES 
Checking for function "fsconfig" : YES 
Checking for function "fsmount" : YES 
Checking for function "getdents64" : YES 
Checking for function "pidfd_spawn" : YES 
Checking for function "getrandom" : YES 
Program sh found: YES (/usr/bin/sh)
Program echo found: YES (/usr/bin/echo)
Program sed found: YES (/usr/bin/sed)
Program awk found: YES (/usr/bin/awk)
Program stat found: YES (/usr/bin/stat)
Program ln found: YES (/usr/bin/ln)
Program git found: YES (/usr/bin/git)
Program env found: YES (/usr/bin/env)
Program rsync found: NO
Program diff found: YES (/usr/bin/diff)
Program find found: YES (/usr/bin/find)
Program quotaon /usr/sbin/quotaon /sbin/quotaon found: NO
Program quotacheck /usr/sbin/quotacheck /sbin/quotacheck found: NO
Program kmod found: YES (/usr/bin/kmod)
Program kexec /usr/sbin/kexec /sbin/kexec found: NO
Program sulogin found: YES (/usr/bin/sulogin)
Program mount found: YES (/usr/bin/mount)
Program umount found: YES (/usr/bin/umount)
Program loadkeys found: YES (/usr/bin/loadkeys)
Program setfont found: YES (/usr/bin/setfont)
Program nologin found: YES (/usr/bin/nologin)
Program gperf found: YES
Message: gperf len type is size_t
Has header "sys/capability.h" : YES 
Has header "crypt.h" : YES 
Has header "linux/ioprio.h" : YES 
Has header "linux/memfd.h" : YES 
Has header "linux/time_types.h" : YES 
Has header "linux/vm_sockets.h" : YES 
Has header "sys/auxv.h" : YES 
Has header "sys/sdt.h" : YES 
Has header "threads.h" : YES 
Has header "valgrind/memcheck.h" : YES 
Has header "valgrind/valgrind.h" : YES 
Run-time dependency threads found: YES
Library rt found: YES
Library m found: YES
Library dl found: YES
Found pkg-config: YES (/bin/pkg-config) 2.1.1
Run-time dependency libcrypt found: YES 4.4.36
Run-time dependency libcap found: YES 2.70
Checking if "libatomic" : links: YES 
Checking for function "crypt_ra" with dependency libcrypt: YES 
Checking for function "crypt_preferred_method" with dependency libcrypt: YES 
Checking for function "crypt_gensalt_ra" with dependency libcrypt: YES 
Dependency libbpf skipped: feature bpf-framework disabled
Run-time dependency mount found: YES 2.40.0
Dependency fdisk skipped: feature fdisk disabled
Dependency pwquality skipped: feature pwquality disabled
Dependency passwdqc skipped: feature passwdqc disabled
Dependency libseccomp skipped: feature seccomp disabled
Dependency libselinux skipped: feature selinux disabled
Dependency libapparmor skipped: feature apparmor disabled
Dependency polkit-gobject-1 skipped: feature polkit disabled
Dependency libacl skipped: feature acl disabled
Dependency audit skipped: feature audit disabled
Dependency blkid skipped: feature blkid disabled
Dependency libkmod skipped: feature kmod disabled
Dependency xencontrol skipped: feature xenctrl disabled
Dependency pam skipped: feature pam disabled
Library pam skipped: feature pam disabled
Dependency pam_misc skipped: feature pam disabled
Library pam_misc skipped: feature pam disabled
Dependency libmicrohttpd skipped: feature microhttpd disabled
Dependency libcryptsetup skipped: feature libcryptsetup disabled
Checking for function "crypt_activate_by_token_pin" with dependency libcryptsetup: NO 
Dependency libcurl skipped: feature libcurl disabled
Dependency libidn2 skipped: feature libidn2 disabled
Dependency libidn skipped: feature libidn disabled
Dependency libiptc skipped: feature libiptc disabled
Run-time dependency libqrencode found: YES 4.1.1
Dependency libgcrypt skipped: feature gcrypt disabled
Dependency gpg-error skipped: feature gcrypt disabled
Library gpg-error skipped: feature gcrypt disabled
Dependency gnutls skipped: feature gnutls disabled
Dependency openssl skipped: feature openssl disabled
Dependency p11-kit-1 skipped: feature p11kit disabled
Dependency libfido2 skipped: feature libfido2 disabled
Dependency tss2-esys tss2-rc tss2-mu tss2-tcti-device skipped: feature tpm2 disabled
Dependency libdw skipped: feature elfutils disabled
Dependency zlib skipped: feature zlib disabled
Dependency bzip2 skipped: feature bzip2 disabled
Library bz2 skipped: feature bzip2 disabled
Run-time dependency liblzma found: YES 5.6.2
Run-time dependency liblz4 found: YES 1.10.0
Run-time dependency libzstd found: YES 1.5.6
Run-time dependency libarchive found: YES 3.7.7
Dependency xkbcommon skipped: feature xkbcommon disabled
Dependency libpcre2-8 skipped: feature pcre2 disabled
Dependency glib-2.0 skipped: feature glib disabled
Dependency gobject-2.0 skipped: feature glib disabled
Dependency gio-2.0 skipped: feature glib disabled
Dependency dbus-1 skipped: feature dbus disabled
Message: default-dnssec cannot be set to yes or allow-downgrade openssl and gcrypt are disabled. Setting default-dnssec to no.
Program python3 (jinja2) found: NO

../src/v256.4-825386a66f.clean/meson.build:1725:15: ERROR: python3 is missing modules: jinja2

A full log can be found at /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-logs/meson-log.txt
```
</details>

<details><summary>/home/justine/.local/share/vcpkg/buildtrees/libsystemd/config-x64-linux-dbg-meson-log.txt.log</summary>

```
Build started at 2024-10-30T10:50:14.798843
Main binary: /usr/bin/python3
Build Options: -Dmode=release -Dstatic-libsystemd=pic -Dtests=false -Ddns-over-tls=false -Dtranslations=false -Dacl=disabled -Dapparmor=disabled -Daudit=disabled -Dblkid=disabled -Dbpf-framework=disabled -Dbzip2=disabled -Ddbus=disabled -Delfutils=disabled -Dfdisk=disabled -Dgcrypt=disabled -Dglib=disabled -Dgnutls=disabled -Dkmod=disabled -Dlibcurl=disabled -Dlibcryptsetup=disabled -Dlibfido2=disabled -Dlibidn=disabled -Dlibidn2=disabled -Dlibiptc=disabled -Dmicrohttpd=disabled -Dopenssl=disabled -Dp11kit=disabled -Dpam=disabled -Dpcre2=disabled -Dpolkit=disabled -Dpwquality=disabled -Dpasswdqc=disabled -Dseccomp=disabled -Dselinux=disabled -Dtpm2=disabled -Dxenctrl=disabled -Dxkbcommon=disabled -Dzlib=disabled -Dlz4=enabled -Dxz=enabled -Dzstd=enabled -Doptimization=plain -Ddebug=true '-Dcmake_prefix_path=['"'"'/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug'"'"','"'"'/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux'"'"']' -Dprefix=/home/justine/.local/share/vcpkg/packages/libsystemd_x64-linux/debug -Dincludedir=../include -Dlibdir=lib -Dbackend=ninja -Dwrap_mode=nodownload -Dpkgconfig.relocatable=True '-Dpkg_config_path=['"'"'/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig'"'"','"'"'/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig'"'"']' --native-file=/home/justine/.local/share/vcpkg/buildtrees/libsystemd/meson-x64-linux-dbg.log
Python system: Linux
The Meson build system
Version: 1.5.1
Source dir: /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean
Build dir: /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg
Build type: native build
Project name: systemd
Project version: 256.4
-----------
Detecting compiler via: `/usr/bin/cc -fPIC -g --version` -> 0
stdout:
cc (GCC) 14.2.1 20240910
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
-----------
Running command: -cpp -x c -E -dM -
-----
-----------
Detecting linker via: `/usr/bin/cc -fPIC -g -Wl,--version -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib` -> 0
stdout:
GNU ld (GNU Binutils) 2.43.0
Copyright (C) 2024 Free Software Foundation, Inc.
This program is free software; you may redistribute it under the terms of
the GNU General Public License version 3 or (at your option) a later version.
This program has absolutely no warranty.
-----------
stderr:
collect2 version 14.2.1 20240910
/usr/bin/ld -plugin /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/lto-wrapper -plugin-opt=-fresolution=/tmp/ccwDd306.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s --build-id --eh-frame-hdr --hash-style=gnu -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/Scrt1.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/crti.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/crtbeginS.o -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1 -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib -L/lib/../lib -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../.. --version -lgcc --push-state --as-needed -lgcc_s --pop-state -lc -lgcc --push-state --as-needed -lgcc_s --pop-state /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/crtendS.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/crtn.o
-----------
Sanity testing C compiler: /usr/bin/cc -fPIC -g
Is cross compiler: False.
Sanity check compiler command line: /usr/bin/cc -fPIC -g sanitycheckc.c -o sanitycheckc.exe -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include -D_FILE_OFFSET_BITS=64 -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib
Sanity check compile stdout:

-----
Sanity check compile stderr:

-----
Running test binary command:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/sanitycheckc.exe
C compiler for the host machine: /usr/bin/cc -fPIC -g (gcc 14.2.1 "cc (GCC) 14.2.1 20240910")
C linker for the host machine: /usr/bin/cc -fPIC -g ld.bfd 2.43.0
-----------
Detecting archiver via: `/usr/bin/ar --version` -> 0
stdout:
GNU ar (GNU Binutils) 2.43.0
Copyright (C) 2024 Free Software Foundation, Inc.
This program is free software; you may redistribute it under the terms of
the GNU General Public License version 3 or (at your option) any later version.
This program has absolutely no warranty.
-----------
-----------
Detecting compiler via: `/usr/bin/cc -fPIC -g --version` -> 0
stdout:
cc (GCC) 14.2.1 20240910
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
-----------
Running command: -cpp -x c -E -dM -
-----
-----------
Detecting linker via: `/usr/bin/cc -fPIC -g -Wl,--version` -> 0
stdout:
GNU ld (GNU Binutils) 2.43.0
Copyright (C) 2024 Free Software Foundation, Inc.
This program is free software; you may redistribute it under the terms of
the GNU General Public License version 3 or (at your option) a later version.
This program has absolutely no warranty.
-----------
stderr:
collect2 version 14.2.1 20240910
/usr/bin/ld -plugin /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/lto-wrapper -plugin-opt=-fresolution=/tmp/ccbPtsPc.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s --build-id --eh-frame-hdr --hash-style=gnu -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/Scrt1.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/crti.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/crtbeginS.o -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1 -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib -L/lib/../lib -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../.. --version -lgcc --push-state --as-needed -lgcc_s --pop-state -lc -lgcc --push-state --as-needed -lgcc_s --pop-state /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/crtendS.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/crtn.o
-----------
Sanity testing C compiler: /usr/bin/cc -fPIC -g
Is cross compiler: False.
Sanity check compiler command line: /usr/bin/cc -fPIC -g sanitycheckc.c -o sanitycheckc.exe -D_FILE_OFFSET_BITS=64
Sanity check compile stdout:

-----
Sanity check compile stderr:

-----
Running test binary command:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/sanitycheckc.exe
C compiler for the build machine: /usr/bin/cc -fPIC -g (gcc 14.2.1 "cc (GCC) 14.2.1 20240910")
C linker for the build machine: /usr/bin/cc -fPIC -g ld.bfd 2.43.0
-----------
Detecting archiver via: `/usr/bin/ar --version` -> 0
stdout:
GNU ar (GNU Binutils) 2.43.0
Copyright (C) 2024 Free Software Foundation, Inc.
This program is free software; you may redistribute it under the terms of
the GNU General Public License version 3 or (at your option) any later version.
This program has absolutely no warranty.
-----------
Build machine cpu family: x86_64
Build machine cpu: x86_64
Host machine cpu family: x86_64
Host machine cpu: x86_64
Target machine cpu family: x86_64
Target machine cpu: x86_64
Running command: /usr/bin/realpath --relative-to=/home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean
--- stdout ---
../src/v256.4-825386a66f.clean

--- stderr ---


Program tools/git-setup.sh found: YES (/home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean/tools/git-setup.sh)
Running command: /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean/tools/git-setup.sh
--- stdout ---

--- stderr ---


Program tools/meson-build.sh found: YES (/home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean/tools/meson-build.sh)
-----------
Detecting compiler via: `/usr/bin/c++ -fPIC -g --version` -> 0
stdout:
c++ (GCC) 14.2.1 20240910
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
-----------
Running command: -cpp -x c++ -E -dM -
-----
-----------
Detecting linker via: `/usr/bin/c++ -fPIC -g -Wl,--version -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib` -> 0
stdout:
GNU ld (GNU Binutils) 2.43.0
Copyright (C) 2024 Free Software Foundation, Inc.
This program is free software; you may redistribute it under the terms of
the GNU General Public License version 3 or (at your option) a later version.
This program has absolutely no warranty.
-----------
stderr:
collect2 version 14.2.1 20240910
/usr/bin/ld -plugin /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/lto-wrapper -plugin-opt=-fresolution=/tmp/ccZZrVfx.res -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc --build-id --eh-frame-hdr --hash-style=gnu -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/Scrt1.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/crti.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/crtbeginS.o -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1 -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib -L/lib/../lib -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../.. --version -lstdc++ -lm -lgcc_s -lgcc -lc -lgcc_s -lgcc /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/crtendS.o /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/crtn.o
-----------
Sanity testing C++ compiler: /usr/bin/c++ -fPIC -g
Is cross compiler: False.
Sanity check compiler command line: /usr/bin/c++ -fPIC -g sanitycheckcpp.cc -o sanitycheckcpp.exe -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include -D_FILE_OFFSET_BITS=64 -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib
Sanity check compile stdout:

-----
Sanity check compile stderr:

-----
Running test binary command:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/sanitycheckcpp.exe
C++ compiler for the host machine: /usr/bin/c++ -fPIC -g (gcc 14.2.1 "c++ (GCC) 14.2.1 20240910")
C++ linker for the host machine: /usr/bin/c++ -fPIC -g ld.bfd 2.43.0
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpwtgcmj5v
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpwtgcmj5v/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpwtgcmj5v/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Wmissing-field-initializers -Wno-missing-field-initializers` -> 0
Compiler for C supports arguments -Wno-missing-field-initializers: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmprz_4225t
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmprz_4225t/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmprz_4225t/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Wunused-parameter -Wno-unused-parameter` -> 0
Compiler for C supports arguments -Wno-unused-parameter: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpd4zrr9r3
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpd4zrr9r3/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpd4zrr9r3/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Wnonnull-compare -Wno-nonnull-compare` -> 0
Compiler for C supports arguments -Wno-nonnull-compare: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp0o27_fxa
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp0o27_fxa/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp0o27_fxa/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Warray-bounds` -> 0
Compiler for C supports arguments -Warray-bounds: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpxubdyasw
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpxubdyasw/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpxubdyasw/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Warray-bounds=2` -> 0
Compiler for C supports arguments -Warray-bounds=2: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp75r2eg0p
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp75r2eg0p/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp75r2eg0p/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Wdate-time` -> 0
Compiler for C supports arguments -Wdate-time: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpqaifn2jd
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpqaifn2jd/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpqaifn2jd/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Wendif-labels` -> 0
Compiler for C supports arguments -Wendif-labels: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp2p3amc9k
Code:
extern int i;
int i;

-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp2p3amc9k/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp2p3amc9k/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -Werror=format=2` -> 0
Compiler for C supports arguments -Werror=format=2: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp3lc8krps
Code:
extern int i;
int i;

-----------
...
Skipped 2395 lines
...
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp3ldyk29r
Code:

        #ifdef __has_include
         #if !__has_include("sys/auxv.h")
          #error "Header 'sys/auxv.h' could not be found"
         #endif
        #else
         #include <sys/auxv.h>
        #endif
-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp3ldyk29r/testfile.c -E -P -fPIC -g -D_FILE_OFFSET_BITS=64 -P -O0 -std=gnu11` -> 0
Has header "sys/auxv.h" : YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpqjhf95uu
Code:

        #ifdef __has_include
         #if !__has_include("sys/sdt.h")
          #error "Header 'sys/sdt.h' could not be found"
         #endif
        #else
         #include <sys/sdt.h>
        #endif
-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpqjhf95uu/testfile.c -E -P -fPIC -g -D_FILE_OFFSET_BITS=64 -P -O0 -std=gnu11` -> 0
Has header "sys/sdt.h" : YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpaslqfc5s
Code:

        #ifdef __has_include
         #if !__has_include("threads.h")
          #error "Header 'threads.h' could not be found"
         #endif
        #else
         #include <threads.h>
        #endif
-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpaslqfc5s/testfile.c -E -P -fPIC -g -D_FILE_OFFSET_BITS=64 -P -O0 -std=gnu11` -> 0
Has header "threads.h" : YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp7o4tbb8w
Code:

        #ifdef __has_include
         #if !__has_include("valgrind/memcheck.h")
          #error "Header 'valgrind/memcheck.h' could not be found"
         #endif
        #else
         #include <valgrind/memcheck.h>
        #endif
-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp7o4tbb8w/testfile.c -E -P -fPIC -g -D_FILE_OFFSET_BITS=64 -P -O0 -std=gnu11` -> 0
Has header "valgrind/memcheck.h" : YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmphi2xcvk5
Code:

        #ifdef __has_include
         #if !__has_include("valgrind/valgrind.h")
          #error "Header 'valgrind/valgrind.h' could not be found"
         #endif
        #else
         #include <valgrind/valgrind.h>
        #endif
-----------
Command line: `/usr/bin/cc -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmphi2xcvk5/testfile.c -E -P -fPIC -g -D_FILE_OFFSET_BITS=64 -P -O0 -std=gnu11` -> 0
Has header "valgrind/valgrind.h" : YES 
Running command: /usr/bin/sh -c 'echo "$SOURCE_DATE_EPOCH"'
--- stdout ---


--- stderr ---


Running command: /usr/bin/stat -c %Y /home/justine/.local/share/vcpkg/buildtrees/libsystemd/src/v256.4-825386a66f.clean/NEWS
--- stdout ---
1721825379

--- stderr ---


Running command: /usr/bin/awk '/^\s*SYS_UID_MIN\s+/ { uid=$2 } END { print uid }' /etc/login.defs
--- stdout ---
500

--- stderr ---


Running command: /usr/bin/awk '/^\s*SYS_UID_MAX\s+/ { uid=$2 } END { print uid }' /etc/login.defs
--- stdout ---
999

--- stderr ---


Running command: /usr/bin/awk '/^\s*SYS_GID_MIN\s+/ { uid=$2 } END { print uid }' /etc/login.defs
--- stdout ---
500

--- stderr ---


Running command: /usr/bin/awk '/^\s*SYS_GID_MAX\s+/ { uid=$2 } END { print uid }' /etc/login.defs
--- stdout ---
999

--- stderr ---


Running command: /usr/bin/getent passwd 65534
--- stdout ---
nobody:x:65534:65534:Kernel Overflow User:/:/usr/bin/nologin

--- stderr ---


Running command: /usr/bin/id -u nobody
--- stdout ---
65534

--- stderr ---


Running command: /usr/bin/getent group 65534
--- stdout ---
nobody:x:65534:

--- stderr ---


Running command: /usr/bin/id -g nobody
--- stdout ---
65534

--- stderr ---


Run-time dependency threads found: YES
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpitoeaz2e
Code:
int main(void) { return 0; }

-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpitoeaz2e/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpitoeaz2e/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -lrt -Wl,--allow-shlib-undefined` -> 0
Library rt found: YES
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpbmt5uylo
Code:
int main(void) { return 0; }

-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpbmt5uylo/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpbmt5uylo/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -lm -Wl,--allow-shlib-undefined` -> 0
Library m found: YES
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp644190da
Code:
int main(void) { return 0; }

-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp644190da/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp644190da/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -ldl -Wl,--allow-shlib-undefined` -> 0
Library dl found: YES
Pkg-config binary for host machine specified from cross file, native file, or env var as ['/bin/pkg-config']
Found pkg-config: YES (/bin/pkg-config) 2.1.1
Determining dependency 'libcrypt' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion libcrypt` -> 0
stdout:
4.4.36
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags libcrypt` -> 0
stdout:
-I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libcrypt` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lcrypt
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libcrypt` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lcrypt
-----------
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp7js7p3b6
Code:

        #include<stddef.h>
        #include<stdio.h>
        int main(void) {
            printf("%ld\n", (long)(sizeof(void *)));
            return 0;
        }
-----------
Command line: `/usr/bin/c++ -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp7js7p3b6/testfile.cpp -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmp7js7p3b6/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -fpermissive` -> 0
Program stdout:

8

Program stderr:


Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpigt5vm_q
Code:

-----------
Command line: `/usr/bin/c++ -fPIC -g -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpigt5vm_q/testfile.cpp -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpigt5vm_q/output.obj -c -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -fpermissive --print-search-dirs` -> 0
stdout:
install: /usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/
programs: =/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/:/usr/lib/gcc/x86_64-pc-linux-gnu/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/:/usr/lib/gcc/x86_64-pc-linux-gnu/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu/14.2.1/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/bin/
libraries: =/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/lib/x86_64-pc-linux-gnu/14.2.1/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/lib/../lib/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../x86_64-pc-linux-gnu/14.2.1/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../lib/:/lib/x86_64-pc-linux-gnu/14.2.1/:/lib/../lib/:/usr/lib/x86_64-pc-linux-gnu/14.2.1/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/lib/:/usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1/../../../:/lib/:/usr/lib/
-----------
Run-time dependency libcrypt found: YES 4.4.36
Determining dependency 'libcap' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion libcap` -> 0
stdout:
2.70
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags libcap` -> 0
stdout:
-I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libcap` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lcap
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libcap` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lcap
-----------
Run-time dependency libcap found: YES 2.70
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpvbhheqw7
Code:
int main(int argc, char **argv) { return 0; }
-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpvbhheqw7/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpvbhheqw7/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -std=gnu11 -latomic` -> 0
Checking if "libatomic" : links: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmprkf1lbvz
Code:
#include <crypt.h>
#include <limits.h>

        #if defined __stub_crypt_ra || defined __stub___crypt_ra
        fail fail fail this function is not going to work
        #endif
        
int main(void) {
            void *a = (void*) &crypt_ra;
            long long b = (long long) a;
            return (int) b;
        }
-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmprkf1lbvz/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmprkf1lbvz/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -std=gnu11 -D_GNU_SOURCE /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib/libcrypt.a` -> 0
Checking for function "crypt_ra" with dependency libcrypt: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucrwqjq9
Code:
#include <crypt.h>
#include <limits.h>

        #if defined __stub_crypt_preferred_method || defined __stub___crypt_preferred_method
        fail fail fail this function is not going to work
        #endif
        
int main(void) {
            void *a = (void*) &crypt_preferred_method;
            long long b = (long long) a;
            return (int) b;
        }
-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucrwqjq9/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucrwqjq9/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -std=gnu11 -D_GNU_SOURCE /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib/libcrypt.a` -> 0
Checking for function "crypt_preferred_method" with dependency libcrypt: YES 
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpvr9beldi
Code:
#include <crypt.h>
#include <limits.h>

        #if defined __stub_crypt_gensalt_ra || defined __stub___crypt_gensalt_ra
        fail fail fail this function is not going to work
        #endif
        
int main(void) {
            void *a = (void*) &crypt_gensalt_ra;
            long long b = (long long) a;
            return (int) b;
        }
-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpvr9beldi/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpvr9beldi/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -std=gnu11 -D_GNU_SOURCE /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib/libcrypt.a` -> 0
Checking for function "crypt_gensalt_ra" with dependency libcrypt: YES 
Dependency libbpf skipped: feature bpf-framework disabled
Determining dependency 'mount' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion mount` -> 0
stdout:
2.40.0
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags mount` -> 0
stdout:
-I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include/libmount -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include/blkid
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs mount` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lmount -lblkid
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs mount` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lmount -lblkid
-----------
Run-time dependency mount found: YES 2.40.0
Dependency fdisk skipped: feature fdisk disabled
Dependency pwquality skipped: feature pwquality disabled
Dependency passwdqc skipped: feature passwdqc disabled
Dependency libseccomp skipped: feature seccomp disabled
Dependency libselinux skipped: feature selinux disabled
Dependency libapparmor skipped: feature apparmor disabled
Dependency polkit-gobject-1 skipped: feature polkit disabled
Dependency libacl skipped: feature acl disabled
Dependency audit skipped: feature audit disabled
Dependency blkid skipped: feature blkid disabled
Dependency libkmod skipped: feature kmod disabled
Dependency xencontrol skipped: feature xenctrl disabled
Dependency pam skipped: feature pam disabled
Library pam skipped: feature pam disabled
Dependency pam_misc skipped: feature pam disabled
Library pam_misc skipped: feature pam disabled
Dependency libmicrohttpd skipped: feature microhttpd disabled
Dependency libcryptsetup skipped: feature libcryptsetup disabled
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpg368gu3_
Code:
#include <libcryptsetup.h>
#include <limits.h>

        #if defined __stub_crypt_activate_by_token_pin || defined __stub___crypt_activate_by_token_pin
        fail fail fail this function is not going to work
        #endif
        
int main(void) {
            void *a = (void*) &crypt_activate_by_token_pin;
            long long b = (long long) a;
            return (int) b;
        }
-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpg368gu3_/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpg368gu3_/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -std=gnu11` -> 1
stderr:
/usr/bin/ld: /tmp/ccCw1kL7.o: in function `main':
/home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpg368gu3_/testfile.c:9:(.text+0x7): undefined reference to `crypt_activate_by_token_pin'
collect2: error: ld returned 1 exit status
-----------
Running compile:
Working directory:  /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucsy0ba0
Code:
#include <libcryptsetup.h>
        int main(void) {

        /* With some toolchains (MSYS2/mingw for example) the compiler
         * provides various builtins which are not really implemented and
         * fall back to the stdlib where they aren't provided and fail at
         * build/link time. In case the user provides a header, including
         * the header didn't lead to the function being defined, and the
         * function we are checking isn't a builtin itself we assume the
         * builtin is not functional and we just error out. */
        #if !0 && !defined(crypt_activate_by_token_pin) && !0
            #error "No definition for __builtin_crypt_activate_by_token_pin found in the prefix"
        #endif

        #ifdef __has_builtin
            #if !__has_builtin(__builtin_crypt_activate_by_token_pin)
                #error "__builtin_crypt_activate_by_token_pin not found"
            #endif
        #elif ! defined(crypt_activate_by_token_pin)
            __builtin_crypt_activate_by_token_pin;
        #endif
        return 0;
        }
-----------
Command line: `/usr/bin/cc -fPIC -g -L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib -I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/include /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucsy0ba0/testfile.c -o /home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucsy0ba0/output.exe -fPIC -g -D_FILE_OFFSET_BITS=64 -O0 -std=gnu11` -> 1
stderr:
/home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucsy0ba0/testfile.c: In function 'main':
/home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucsy0ba0/testfile.c:12:14: error: #error "No definition for __builtin_crypt_activate_by_token_pin found in the prefix"
   12 |             #error "No definition for __builtin_crypt_activate_by_token_pin found in the prefix"
      |              ^~~~~
/home/justine/.local/share/vcpkg/buildtrees/libsystemd/x64-linux-dbg/meson-private/tmpucsy0ba0/testfile.c:17:18: error: #error "__builtin_crypt_activate_by_token_pin not found"
   17 |                 #error "__builtin_crypt_activate_by_token_pin not found"
      |                  ^~~~~
-----------
Checking for function "crypt_activate_by_token_pin" with dependency libcryptsetup: NO 
Dependency libcurl skipped: feature libcurl disabled
Dependency libidn2 skipped: feature libidn2 disabled
Dependency libidn skipped: feature libidn disabled
Dependency libiptc skipped: feature libiptc disabled
Determining dependency 'libqrencode' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion libqrencode` -> 0
stdout:
4.1.1
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags libqrencode` -> 0
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libqrencode` -> 0
stdout:
-L/usr/lib -lqrencode
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libqrencode` -> 0
stdout:
-lqrencode
-----------
Run-time dependency libqrencode found: YES 4.1.1
Dependency libgcrypt skipped: feature gcrypt disabled
Dependency gpg-error skipped: feature gcrypt disabled
Library gpg-error skipped: feature gcrypt disabled
Dependency gnutls skipped: feature gnutls disabled
Dependency openssl skipped: feature openssl disabled
Dependency p11-kit-1 skipped: feature p11kit disabled
Dependency libfido2 skipped: feature libfido2 disabled
Dependency tss2-esys tss2-rc tss2-mu tss2-tcti-device skipped: feature tpm2 disabled
Dependency libdw skipped: feature elfutils disabled
Dependency zlib skipped: feature zlib disabled
Dependency bzip2 skipped: feature bzip2 disabled
Library bz2 skipped: feature bzip2 disabled
Determining dependency 'liblzma' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion liblzma` -> 0
stdout:
5.6.2
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags liblzma` -> 0
stdout:
-I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include -DLZMA_API_STATIC
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs liblzma` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -llzma -pthread
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs liblzma` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -llzma -pthread
-----------
Run-time dependency liblzma found: YES 5.6.2
Determining dependency 'liblz4' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion liblz4` -> 0
stdout:
1.10.0
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags liblz4` -> 0
stdout:
-I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs liblz4` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -llz4d
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs liblz4` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -llz4d
-----------
Run-time dependency liblz4 found: YES 1.10.0
Determining dependency 'libzstd' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion libzstd` -> 0
stdout:
1.5.6
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags libzstd` -> 0
stdout:
-I/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../../include
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libzstd` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lzstd
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libzstd` -> 0
stdout:
-L/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig/../../lib -lzstd
-----------
Run-time dependency libzstd found: YES 1.5.6
Determining dependency 'libarchive' with pkg-config executable '/bin/pkg-config'
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --modversion libarchive` -> 0
stdout:
3.7.7
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --cflags libarchive` -> 0
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_ALLOW_SYSTEM_LIBS]: 1
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libarchive` -> 0
stdout:
-L/usr/lib -larchive
-----------
env[PKG_CONFIG]: /bin/pkg-config
env[PKG_CONFIG_PATH]: /home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/debug/lib/pkgconfig:/home/justine/Documents/delivery/R-Type/vcpkg_installed/x64-linux/share/pkgconfig
-----------
Called: `/bin/pkg-config --libs libarchive` -> 0
stdout:
-larchive
-----------
Run-time dependency libarchive found: YES 3.7.7
Dependency xkbcommon skipped: feature xkbcommon disabled
Dependency libpcre2-8 skipped: feature pcre2 disabled
Dependency glib-2.0 skipped: feature glib disabled
Dependency gobject-2.0 skipped: feature glib disabled
Dependency gio-2.0 skipped: feature glib disabled
Dependency dbus-1 skipped: feature dbus disabled
Message: default-dnssec cannot be set to yes or allow-downgrade openssl and gcrypt are disabled. Setting default-dnssec to no.
Program python3 (jinja2) found: NO

../src/v256.4-825386a66f.clean/meson.build:1725:15: ERROR: python3 is missing modules: jinja2
```
</details>

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "asio",
    "imgui-sfml",
    "luabridge",
    "nlohmann-json",
    "sdl2",
    "sdl2-image",
    "sdl2-mixer",
    "sfml",
    "sdl2-ttf"
  ]
}

```
</details>
