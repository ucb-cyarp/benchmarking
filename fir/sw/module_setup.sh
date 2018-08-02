# Module Setup
export modules_shell=bash
module() { eval `/usr/bin/modulecmd $modules_shell $*`; }
#module() { eval `/usr/bin/modulecmd $modules_shell $*`; }
# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

export MODULEPATH=/home/cyarp/toolchain-intel/PSXE_2018_1/modulefiles:${MODULEPATH}
