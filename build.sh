#!/bin/bash

# Define the root of your project.
# This script assumes it's placed in the project's root directory.
PROJECT_ROOT=$(dirname "$0")

# --- Function Definitions ---

# Function to display the main architecture selection menu
display_main_menu() {
    clear
    echo "========================================"
    echo " ARK Kernel Build System Selector "
    echo "========================================"
    echo " Please choose an architecture:"
    echo "----------------------------------------"
    echo " 1. ARM64 (ARK Kernel - RPi Zero 2 W)"
    echo " 2. i386 (X86 Kernel)"
    echo " 3. Exit"
    echo "----------------------------------------"
    echo -n " Enter your choice: "
}

# Function to display the submenu for a selected architecture
display_arch_menu() {
    clear
    echo "========================================"
    echo " $1 Options " # $1 will be "ARM64 (ARK Kernel)" or "i386 (X86 Kernel)"
    echo "========================================"
    echo " 1. Build"
    echo " 2. Clean"
    echo " 3. Run in QEMU"
    echo " 4. Back to Main Menu"
    echo -n " Enter your choice: "
}

# --- ARM64 Specific Functions ---

build_arm64() {
    echo "Building ARM64 (ARK Kernel)..."
    echo "Running command from root: make build arch=arm64"
    (cd "${PROJECT_ROOT}" && make build arch=arm64) || { echo "ERROR: ARM64 build failed. Ensure top-level Makefile supports 'make build arch=arm64'."; return 1; }
    echo "ARM64 build complete!"
    return 0
}

clean_arm64() {
    echo "Cleaning ARM64 (ARK Kernel)..."
    echo "Running command from root: make clean arch=arm64"
    (cd "${PROJECT_ROOT}" && make clean arch=arm64) || { echo "ERROR: ARM64 clean failed. Ensure top-level Makefile supports 'make clean arch=arm64'."; return 1; }
    echo "ARM64 clean complete!"
    return 0
}

qemu_arm64() {
    echo "Running ARM64 (ARK Kernel) in QEMU..."
    echo "Running command from root: make run arch=arm64"
    (cd "${PROJECT_ROOT}" && make run arch=arm64) || { echo "ERROR: QEMU failed for ARM64. Ensure top-level Makefile supports 'make run arch=arm64'."; return 1; }
    echo "ARM64 QEMU session complete."
    return 0
}

# --- i386 Specific Functions ---

build_i386() {
    echo "Building i386 (X86 Kernel)..."
    echo "Running command from root: make build arch=i386"
    (cd "${PROJECT_ROOT}" && make build arch=i386) || { echo "ERROR: i386 build failed."; return 1; }
    echo "i386 build complete!"
    return 0
}

clean_i386() {
    echo "Cleaning i386 (X86 Kernel)..."
    echo "Running command from root: make clean arch=i386"
    (cd "${PROJECT_ROOT}" && make clean arch=i386) || { echo "ERROR: i386 clean failed. (If 'make clean arch=i386' is not valid, adjust the build.sh script or your Makefile)"; return 1; }
    echo "i386 clean complete!"
    return 0
}

qemu_i386() {
    echo "Building and running i386 (X86 Kernel) in QEMU (preffered method)..."
    echo "Running command from root: make run arch=i386"
    (cd "${PROJECT_ROOT}" && make run arch=i386) || { echo "ERROR: i386 build and run failed."; return 1; }
    echo "i386 QEMU session complete."
    return 0
}


# --- Main Script Logic ---

while true; do
    display_main_menu
    read -r main_choice

    case $main_choice in
        1) # ARM64 selected
            while true; do
                display_arch_menu "ARM64 (ARK Kernel)"
                read -r arm_choice

                case $arm_choice in
                    1) build_arm64 ;;
                    2) clean_arm64 ;;
                    3) qemu_arm64 ;;
                    4) break ;; # Back to main menu
                    *) echo "Invalid choice. Please try again." ;;
                esac
                read -n 1 -s -r -p "Press any key to continue..."
            done
            ;;
        2) # i386 selected
            while true; do
                display_arch_menu "i386 (X86 Kernel)"
                read -r x86_choice

                case $x86_choice in
                    1) build_i386 ;;
                    2) clean_i386 ;;
                    3) qemu_i386 ;;
                    4) break ;; # Back to main menu
                    *) echo "Invalid choice. Please try again." ;;
                esac
                read -n 1 -s -r -p "Press any key to continue..."
            done
            ;;
        3) # Exit
            echo "Exiting the Build System Selector."
            exit 0
            ;;
        *) # Invalid Main Menu choice
            echo "Invalid choice. Please try again."
            read -n 1 -s -r -p "Press any key to continue..."
            ;;
    esac
done
